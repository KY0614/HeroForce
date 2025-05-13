#include<DxLib.h>
#include<cmath>
#include"../../../Application.h"
#include"../../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& _spawnPos)
{
	trans_.pos = _spawnPos;
	state_ = STATE::MAX;
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	walkSpeed_ = 0.0f;
	runSpeed_ = 0.0f;
	atkAct_ = ATK_ACT::MAX;
	lastAtk_ = nullptr;
	isEndAlert_ = false;
	isEndAllAtkSign_ = false;
	isEndAllAtk_ = false;
	nowSkillPreAnim_ = ANIM::NONE;
	nowSkillAnim_ = ANIM::NONE;
	localCenterPos_ = AsoUtility::VECTOR_ZERO;
	colPos_ = AsoUtility::VECTOR_ZERO;
	searchState_ = SEARCH_STATE::MAX;
	preTargetPos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	searchRange_ = 0.0f;
	atkStartRange_ = 0.0f;
	isColStage_ = false;
	colStageCnt_ = 0.0f;
	startCnt_ = 0.0f;
	exp_ = 0.0f;

	for (int i = 0 ; i < SQUARE_VERTEX_NUM ; i++)
	{
		alertVertex_[i].dif = GetColorU8(0, 0, 0, 0);
		alertVertex_[i].norm = AsoUtility::VECTOR_ZERO;
		alertVertex_[i].pos = AsoUtility::VECTOR_ZERO;
		alertVertex_[i].spc = GetColorU8(0, 0, 0, 0);
		alertVertex_[i].su = 0.0f;
		alertVertex_[i].sv = 0.0f;
		alertVertex_[i].u = 0.0f;
		alertVertex_[i].v = 0.0f;
	}
}

void Enemy::Destroy(void)
{
	//各削除処理
	animNum_.clear();
	stateChanges_.clear();
	changeSpeedAnim_.clear();
	skills_.clear();
	nowSkill_.clear();
	alertSkills_.clear();
	changeSkill_.clear();
	skillPreAnims_.clear();
	skillAnims_.clear();
	SearchStateInfo_.clear();
}

void Enemy::Init(void)
{
	//状態管理
	stateChanges_.emplace(STATE::NORMAL, std::bind(&Enemy::ChangeStateNormal, this));
	stateChanges_.emplace(STATE::ALERT, std::bind(&Enemy::ChangeStateAlert, this));
	stateChanges_.emplace(STATE::ATTACK, std::bind(&Enemy::ChangeStateAtk, this));
	stateChanges_.emplace(STATE::BREAK, std::bind(&Enemy::ChangeStateBreak, this));

	//探索状態管理
	SearchStateInfo_.emplace(SEARCH_STATE::CHICKEN_SEARCH, std::bind(&Enemy::SearchMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::CHICKEN_FOUND, std::bind(&Enemy::FoundMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::PLAYER_FOUND, std::bind(&Enemy::FoundMoveInfo, this));

	//キャラ固有設定
	SetParam();

	//アニメーション番号の初期化
	InitAnim();

	//エフェクトの初期化
	InitEffect();

	//共通の変数の初期化
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	ChangeState(STATE::NORMAL);
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	atkAct_ = ATK_ACT::MAX;
	isEndAlert_ = false;
	isEndAllAtkSign_ = false;
	isEndAllAtk_ = false;
	isColStage_ = false;
	colStageCnt_ = 0.0f;
	startCnt_ = 0.0f;
	damage_ = 0.0f;
	targetPos_ = preTargetPos_ = AsoUtility::VECTOR_ZERO;
	ChangeSearchState(SEARCH_STATE::CHICKEN_SEARCH);

	//攻撃情報の初期化
	InitSkill();
	atk_.ResetCnt();

	//モデル情報の初期更新
	trans_.Update();

	//敵のUI
	ui_ = std::make_unique<EnemyHpBar>();
	ui_->Init();
}

void Enemy::Update(void)
{
	//移動前座標
	prePos_ = trans_.pos;

	//アニメーション
	Anim();

#ifdef DEBUG_ENEMY	//デバッグ処理
	//入力用
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY

	//HP減少処理
	SubHp();

	//やられているなら何もしない
	if (!IsAlive()) 
	{
		//やられたら死亡アニメーション
		ResetAnim(ANIM::DEATH, changeSpeedAnim_[ANIM::DEATH]);
		return;
	}

	//当たり判定座標の更新
	colPos_ = VAdd(trans_.pos, localCenterPos_);

	//状態ごとのUpdate
	stateUpdate_();


	//モデル制御
	trans_.Update();
}

bool Enemy::IsFinishAnim(const ANIM _anim)
{
	//引数が指定されていない　又は　指定されたアニメーション番号と現在のアニメーションが同じ
	if (_anim == ANIM::NONE || anim_ == _anim)
	{
		if (stepAnim_ > animTotalTime_)
		{
			return true;
		}
	}

	return false;
}

void Enemy::ChangeSearchState(const SEARCH_STATE _searchState)
{
	//状態遷移
	searchState_ = _searchState;

	//状態による情報更新
	SearchStateInfo_[searchState_]();
}

void Enemy::ChangeState(const STATE _state)
{
	//状態遷移
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void Enemy::KeepCollStageDistance(void)
{
	//ターゲットまでのベクトルを測って、x,zの値が大きい方向に距離をとる
	VECTOR targetVec = GetMoveVec(trans_.pos, targetPos_);

	if (fabsf(targetVec.x) > fabsf(targetVec.z))
	{
		SetPreTargetPos(VSub(prePos_, { 0.0f,0.0f,targetVec.z * KEEP_COL_STAGE_POS }));
	}
	else
	{
		SetPreTargetPos(VSub(prePos_, { targetVec.x * KEEP_COL_STAGE_POS,0.0f,0.0f }));
	}

	//ステージに当たった
	isColStage_ = true;
}

void Enemy::ChangeStateNormal(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateNml, this);
}

void Enemy::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateAlert, this);

	//予兆範囲を初期化
	ResetAlertVertex();

	//向きを改めて設定
	trans_.quaRot = trans_.quaRot.LookRotation(GetMoveVec(trans_.pos,targetPos_));

	//ランダムで攻撃情報を生成
	RandSkill();

	//予備動作アニメーション
	ResetAnim(nowSkillPreAnim_, changeSpeedAnim_[nowSkillPreAnim_]);

	//警告カウンタ初期化
	alertCnt_ = 0.0f;
}

void Enemy::ChangeStateAtk(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateAtk, this);

	//攻撃生成
	lastAtk_ = &createSkill_();
}

void Enemy::ChangeStateBreak(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateBreak, this);

	//攻撃休憩時間の初期化
	breakCnt_ = 0;
}

void Enemy::ParamLoad(CharacterParamData::UNIT_TYPE type)
{
	//キャラクターのパラメータデータ取得
	auto& data = CharacterParamData::GetInstance().GetParamData(type);

	//共通パラメータ
	UnitBase::ParamLoad(type);

	//歩きの速度
	walkSpeed_ = data.speed_;

	//走り速度
	runSpeed_ = data.speed_ * RUN_SPEED_MULTI;
	
	//経験値
	exp_ = data.exp_;
}

void Enemy::InitAnim()
{
#pragma region 共通アニメーション設定

	//立ち
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	//歩き
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	//走り
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	//ダメージ
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	//死亡
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	//出現
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);

#pragma endregion

#pragma region アニメーション速度設定

	//立ち
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM);
	//歩き
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	//走り
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM);
	//ダメージ
	changeSpeedAnim_.emplace(ANIM::DAMAGE, SPEED_ANIM);
	//死亡
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM);
	//出現
	changeSpeedAnim_.emplace(ANIM::ENTRY, SPEED_ANIM);

#pragma endregion
}

void Enemy::InitEffect(void)
{
}

void Enemy::Alert(void)
{
	//警告
	alertNowSkill_();

	//クールダウンカウンタ
	CntUp(alertCnt_);

	//警告終了条件
	if (!IsAlertTime())isEndAlert_ = true;
}

void Enemy::Attack(void)
{
	//対応スキル発動
	processSkill_();

	//各攻撃のカウント
	for (auto& nowSkill : nowSkill_)
	{
		//攻撃のカウント
		CntUp(nowSkill.cnt_);
	}
}

void Enemy::UpdateNml(void)
{
#pragma region 終了処理

	/*ゲームシーンの当たり判定でやっています*/

#pragma endregion

#pragma region アニメーション条件

	//待機アニメーション
	if (moveSpeed_ == 0.0)
		ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
	//歩きアニメーション
	else if (runSpeed_ > moveSpeed_ > 0.0f)
		ResetAnim(ANIM::WALK, changeSpeedAnim_[ANIM::WALK]);
	//走りアニメーション
	else if (moveSpeed_ >= runSpeed_)
		ResetAnim(ANIM::RUN, changeSpeedAnim_[ANIM::RUN]);

#pragma endregion
	
#pragma region 索敵

	//最初のみ補完する
	if (startCnt_ < START_CNT)CntUp(startCnt_);

	//ステージに当たったなら
	if (isColStage_)CntUp(colStageCnt_);
	if (colStageCnt_ >= COL_STAGE_CNT)
	{
		isColStage_ = false;
		colStageCnt_ = 0.0f;
	}

	//まだ見つけ切れていないなら
	if (searchState_ == SEARCH_STATE::CHICKEN_SEARCH)
		//予定している目標に向かう
		targetPos_ = preTargetPos_;

	//移動処理
	Move();

#pragma endregion
}

void Enemy::UpdateAlert(void)
{
#pragma region 終了処理

	//警告カウンタが終わったなら攻撃開始
	if (isEndAlert_)
	{
		//警告終了判定の初期化
		ResetAlertJudge();

		//攻撃状態に遷移
		ChangeState(STATE::ATTACK);

		return;
	}

#pragma endregion

#pragma region 警告処理

	//警告
	Alert();

#pragma endregion
}

void Enemy::UpdateAtk(void)
{
#pragma region 終了処理

	//攻撃が終わっているなら状態遷移
	if (isEndAllAtk_)
	{
		//攻撃終了判定の初期化
		ResetAtkJudge();

		//休憩状態に遷移
		ChangeState(STATE::BREAK);
		return;
	}

#pragma endregion

#pragma region 攻撃処理

	//攻撃処理
	Attack();

	//攻撃アニメーション
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);

#pragma endregion
}

void Enemy::UpdateBreak(void)
{
#pragma region 終了処理

	//休憩時間が終わったら
	if (!IsBreak())
	{
		//通常状態に遷移
		ChangeState(STATE::NORMAL);
		return;
	}

#pragma endregion

#pragma region 待機処理

	//待機アニメーション
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);

	//攻撃休憩時間カウンタ
	CntUp(breakCnt_);

#pragma endregion
}

void Enemy::DrawDebug(void)
{
	//デバッグ描画
	DrawFormatString(0, Application::SCREEN_SIZE_Y - 16, 0xffffff, "EnemyHP = %d", hp_);
	int statePos = Application::SCREEN_SIZE_Y - 32;
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:NORMAL");
		break;

	case Enemy::STATE::ALERT:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:BREAK");
		break;
	}
	//敵の当たり判定
	DrawSphere3D(trans_.pos, radius_, 4, 0xffff00, 0xffff00, false);
	//敵の索敵判定
	DrawSphere3D(trans_.pos, searchRange_, 2, searchState_ != SEARCH_STATE::CHICKEN_SEARCH ? 0xff0000 : 0xffffff, searchState_ != SEARCH_STATE::CHICKEN_SEARCH ? 0xff0000 : 0xffffff, false);
	//敵の索敵判定
	DrawSphere3D(trans_.pos, atkStartRange_, 2, state_ == STATE::ALERT ? 0xff0000 : 0xffffff, state_ == STATE::ALERT ? 0x0000ff : 0xffffff, false);
}

void Enemy::SearchMoveInfo(void)
{
	//移動速度更新
	moveSpeed_ = walkSpeed_;
}

void Enemy::FoundMoveInfo(void)
{
	//移動速度更新
	moveSpeed_ = runSpeed_;
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
	
	//デバッグ
	DrawDebug();

#endif // DEBUG_ENEMY

	//UIの描画
	ui_->Draw(*this);

	//敵が生きている　又は　やられモーション中なら描画する
	if (IsAlive() || anim_ == ANIM::DEATH && animTotalTime_ >= stepAnim_)
	{
		//敵モデルの描画
		MV1DrawModel(trans_.modelId);
		
		//攻撃予兆の描画
		if (state_ == STATE::ALERT)
		{
			//予兆用ポリゴン描画
			DrawPolygon3D(alertVertex_, SQUARE_POLYGON_NUM, DX_NONE_GRAPH, false);
		}
	}

}

const VECTOR Enemy::GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed)
{
	//標的への方向ベクトルを取得
	VECTOR targetVec = VSub(_goal, _start);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//移動量を求める
	VECTOR ret = VScale(targetVec, _speed);

	//移動(方向)ベクトルを返す
	return ret;
}

void Enemy::Move(void)
{
	//移動ベクトル取得
	VECTOR targetVec = GetMoveVec(trans_.pos, targetPos_, moveSpeed_);

	//向き回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//移動
	trans_.pos = VAdd(trans_.pos, targetVec);

	//移動量の初期化
	moveSpeed_ = 0.0f;
}

Enemy::ATK& Enemy::CreateSkill(ATK_ACT _atkAct)
{
#pragma region 使い終わった攻撃がある場合

	//使い終わった攻撃に上書き
	for (auto& nowSkill : nowSkill_)
	{
		//攻撃が使い終わっているなら
		if (nowSkill.IsFinishMotion())
		{
			//スキル上書き
			nowSkill = skills_[_atkAct];

			//カウンタの初期化
			nowSkill.ResetCnt();

			//ヒット判定の初期化
			nowSkill.isHit_ = false;

			//処理終了
			return nowSkill;
		}
	}

#pragma endregion

#pragma region ない場合

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	nowSkill_.emplace_back(skills_[_atkAct]);

	//カウンタの初期化
	nowSkill_.back().ResetCnt();

	//ヒット判定の初期化
	nowSkill_.back().isHit_ = false;

	//処理終了
	return nowSkill_.back();

#pragma endregion
}

void Enemy::FinishAnim(void)
{
	//アニメーション判定
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::RUN:
		//ループ再生
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::SKILL_1:
	case UnitBase::ANIM::SKILL_2:
	case UnitBase::ANIM::DAMAGE:
	case UnitBase::ANIM::DEATH:
	case UnitBase::ANIM::ENTRY:
		//ループしない
		break;
	}
}

void Enemy::ResetAlertJudge(void)
{
	//終了判定初期化
	isEndAlert_ = false;
}

void Enemy::ResetAlertVertex(void)
{
	for (auto& ver : alertVertex_)
	{
		ver.pos = AsoUtility::VECTOR_ZERO;
		ver.u = 0.0f;
		ver.v = 0.0f;
		ver.norm = { 0.0f,0.0f,0.0f };
		ver.dif = GetColorU8(0, 0, 0, 0);
		ver.spc = GetColorU8(0, 0, 0, 0);
		ver.su = 0;
		ver.sv = 0;
	}
}

void Enemy::ResetAtkJudge(void)
{
	//攻撃終了判定の初期化
	isEndAllAtk_ = false;
}

void Enemy::RandSkill(void)
{
	//スキルの数
	size_t size = skills_.size();

	//スキルの数分からランダムを取る
	int rand = GetRand(size - 1);

	//スキル
	atkAct_ = static_cast<ATK_ACT>(rand);

	//スキル生成準備
	SetUpSkill(atkAct_);

	//スキルに対応した予備動作アニメーションの記録
	nowSkillPreAnim_ = skillPreAnims_[static_cast<int>(atkAct_)];

	//スキルに対応したアニメーションの記録
	nowSkillAnim_ = skillAnims_[static_cast<int>(atkAct_)];

	//スキル生成
	createSkill_ = std::bind(&Enemy::CreateSkill, this, atkAct_);
}

void Enemy::SetUpSkill(ATK_ACT _atkAct)
{
	//警告情報をセット
	alertNowSkill_ = alertSkills_[_atkAct];

	//攻撃情報をセット
	processSkill_ = changeSkill_[_atkAct];
}

void Enemy::CreateAlert(const VECTOR& _pos, const float _widthX, const float _widthZ)
{
	//半径
	float radX = _widthX / 2;
	float radZ = _widthZ / 2;

	//右上の頂点
	alertVertex_[0].pos = { _pos.x + radX,0.0f,_pos.z + radZ };
	alertVertex_[0].u = _widthX;
	alertVertex_[0].v = 0.0f;

	//左下の頂点
	alertVertex_[1].pos = { _pos.x - radX,0.0f,_pos.z - radZ };
	alertVertex_[1].u = 0.0f;
	alertVertex_[1].v = _widthX;

	//左上の頂点
	alertVertex_[2].pos = { _pos.x - radX,0.0f,_pos.z + radZ };
	alertVertex_[2].u = 0.0f;
	alertVertex_[2].v = 0.0f;

	//右上の頂点
	alertVertex_[3].pos = { _pos.x + radX,0.0f,_pos.z + radZ };
	alertVertex_[3].u = _widthX;
	alertVertex_[3].v = 0.0f;

	//右下の頂点
	alertVertex_[4].pos = { _pos.x + radX,0.0f,_pos.z - radZ };
	alertVertex_[4].u = _widthX;
	alertVertex_[4].v = _widthZ;

	//左下の頂点
	alertVertex_[5].pos = { _pos.x - radX,0.0f,_pos.z - radZ };
	alertVertex_[5].u = 0.0f;
	alertVertex_[5].v = _widthZ;

	//四角形の描画準備
	for (auto& ver : alertVertex_)
	{
		//回転するために位置を(0,0)にする
		ver.pos = VSub(ver.pos, _pos);

		//回転
		ver.pos = Quaternion::PosAxis(trans_.quaRot, ver.pos);
		
		//位置を戻す
		ver.pos = VAdd(ver.pos, _pos);
		
		//表示関係
		ver.norm = ALERT_NORM;
		ver.dif = GetColorU8(COLOR_MAX, 0, 0, ALPHA_MAX);
		ver.spc = GetColorU8(0, 0, 0, 0);
		ver.su = 0;
		ver.sv = 0;
	}
}
