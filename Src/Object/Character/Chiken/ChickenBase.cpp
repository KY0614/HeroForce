#include "ChickenBase.h"

ChickenBase::ChickenBase()
{
	imgHelp_ = -1;
	moveSpeed_ = -1.0f;
	fadeStep_ = -1.0f;
	state_ = STATE::NONE;
	aliveState_ = ALIVE_MOVE::MAX;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	isHelp_ = false;
	isHelpCnt_ = 0.0f;
	smokeNum_ = -1;
	smokeStep_ = -1.0f;
	efeSpeed_ = -1.0f;

	int i = -1;
	imgSmoke_ = &i;

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ALIVE , std::bind(&ChickenBase::ChangeStateAlive, this));
	stateChanges_.emplace(STATE::DAMAGE , std::bind(&ChickenBase::ChangeStateDamage, this));
	stateChanges_.emplace(STATE::DEATH , std::bind(&ChickenBase::ChangeStateDeath, this));
	stateChanges_.emplace(STATE::END , std::bind(&ChickenBase::ChangeStateEnd, this));

	// 生存状態管理
	aliveChanges_.emplace(ALIVE_MOVE::IDLE, std::bind(&ChickenBase::ChangeAliveIdle, this));
	aliveChanges_.emplace(ALIVE_MOVE::ROTATION, std::bind(&ChickenBase::ChangeAliveWalk, this));
	aliveChanges_.emplace(ALIVE_MOVE::CALL, std::bind(&ChickenBase::ChangeAliveCall, this));
}

ChickenBase::~ChickenBase()
{
}

void ChickenBase::Create(const VECTOR &_pos)
{
	//情報の受け取り
	trans_.pos = _pos;
	
	//モデル設定
	ModelSet();

	//リソースの読み込み
	Load();

	//アニメーション管理番号の初期化
	InitAnimNum();

	//パラメーター設定
	SetParam();

	//HPUIの生成
	hpUi_ = std::make_unique<CpuHpBar>();
	hpUi_->Init();
}

void ChickenBase::Update()
{
	//バックアップ
	prePos_ = trans_.pos;	

	//デバッグ処理
	//DebagUpdate();

	//アニメーションカウント
	Anim();

	// 更新ステップ
 	stateUpdate_();

	//残量HPの処理
	SubHp();

	//HPが0以下の場合
	if (hp_ <= 0 && state_ != STATE::END) { ChangeState(STATE::DEATH); }

	//画像表示確認
	CheckIsHelp();

	//UI設定
	SetUiParam();

	//トランスフォーム更新
	trans_.Update();
}

void ChickenBase::Draw()
{
	stateDraw_();

	//デバッグ描画
	//DebagDraw();

	//ビルボード描画
	DrawHelp();
}

void ChickenBase::SetIsHelp()
{
	//表示
	isHelp_ = true;

	//表示時間の設定
	isHelpCnt_ = IS_HELP_CNT;
}

void ChickenBase::ModelSet()
{
	//モデルの初期化
	trans_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CHICKEN));
	trans_.scl = SCALE;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f);

	trans_.Update();
}

void ChickenBase::Load()
{
	auto& res = ResourceManager::GetInstance();

	//ヘルプ画像
	imgHelp_ = res.Load(ResourceManager::SRC::HELP).handleId_;

	//スモーク画像
	imgSmoke_ = res.Load(ResourceManager::SRC::SMOKE).handleIds_;

	//エフェクト追加
	EffectManager::GetInstance().Add(EffectManager::EFFECT::DAMAGE, 
		res.Load(ResourceManager::SRC::DAMAGE_EFE).handleId_);
}

void ChickenBase::SetParam()
{
	//ステータス設定
	ParamLoad(CharacterParamData::UNIT_TYPE::CHICKEN);

	// フェード時間
	fadeStep_ = TIME_FADE;

	//画像表示
	isHelp_ = false; 
	isHelpCnt_ = 0.0f;

	//煙エフェクト
	smokeNum_ = 0;
	smokeStep_ =0.0f;
	efeSpeed_ = SMOKE_SPEED;
	
	//生存時の行動をランダムで決める
	aliveState_ = static_cast<ALIVE_MOVE>(GetRand(ALIVE_MOVE_MAX - 1));

	//初期状態(※生存時の状態を決めてから行う)
    ChangeState(STATE::ALIVE);

	//アニメーション状態
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::InitAnimNum()
{
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CALL);
}

void ChickenBase::SetUiParam()
{
	//座標設定
	VECTOR pos = VAdd(trans_.pos, LOCAL_HP_POS);
	hpUi_->SetPos(pos);

	//HP設定
	hpUi_->SetHP(hp_);
}

void ChickenBase::ChangeState(const STATE _state)
{
	// 状態変更
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void ChickenBase::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateNone, this);
	stateDraw_ = std::bind(&ChickenBase::DrawNone, this);
}

void ChickenBase::ChangeStateAlive()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateAlive, this);
	stateDraw_ = std::bind(&ChickenBase::DrawAlive, this);

	//生存時の行動状態の反映
	ChangeAliveState(aliveState_);
}

void ChickenBase::ChangeStateDamage()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDamage, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDamage, this);

	//画像表示
	SetIsHelp();

	//エフェクト再生
	VECTOR localPos = { 
		GetRand(static_cast<int>(EFC_CREATE_RANGE.x)),
		GetRand(static_cast<int>(EFC_CREATE_RANGE.y)),
		GetRand(static_cast<int>(EFC_CREATE_RANGE.z)) };
	VECTOR pos = VAdd(trans_.pos, localPos);
	EffectManager::GetInstance().Play(
		EffectManager::EFFECT::DAMAGE,
		pos,
		Quaternion(),
		DAMAGE_EFE_SCALE,
		SoundManager::SOUND::NONE);
}

void ChickenBase::ChangeStateDeath()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDeath, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDeath, this);
}

void ChickenBase::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateEnd, this);
	stateDraw_ = std::bind(&ChickenBase::DrawEnd, this);
}

void ChickenBase::ChangeAliveState(const ALIVE_MOVE _state)
{
	// 状態変更
	aliveState_ = _state;

	// 各状態遷移の初期処理
	aliveChanges_[aliveState_]();
}

void ChickenBase::ChangeAliveIdle()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveIdle , this);
}

void ChickenBase::ChangeAliveWalk()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveRotation, this);
}

void ChickenBase::ChangeAliveCall()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveCall, this);
}

void ChickenBase::UpdateNone()
{
	//NONEの時は何も行わない
}

void ChickenBase::UpdateAlive()
{
   	stateAliveUpdate_();
}

void ChickenBase::UpdateDamage()
{
	//アニメーションを変更
	ResetAnim(ANIM::DAMAGE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::UpdateDeath()
{	
	//ステップ更新
	float value = SceneManager::GetInstance().GetDeltaTime();
	fadeStep_ -= value;

	if (fadeStep_ < 0.0f)
	{
		//エフェクトアニメーション番号
		smokeStep_ += value;
		smokeNum_ = static_cast<int>(smokeStep_ * efeSpeed_);
		if (smokeNum_ >= SMOKE_NUM)
		{
			ChangeState(STATE::END);
		}
		return;
	}

	//アニメーションを変更
	ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM);
}

void ChickenBase::UpdateEnd()
{
	//ENDのときは何も行わない
}

void ChickenBase::DrawNone()
{
	//描画を行わない
}

void ChickenBase::DrawAlive()
{
	MV1DrawModel(trans_.modelId);

	//HPUI表示
	hpUi_->Draw();
}

void ChickenBase::DrawDamage()
{
	MV1DrawModel(trans_.modelId);

	//HPUI表示
	hpUi_->Draw();
}

void ChickenBase::DrawDeath()
{
	// 時間による色の線形補間
	float diff = TIME_FADE - fadeStep_;
	auto c = AsoUtility::Lerp(FADE_C_FROM, FADE_C_TO, (diff / TIME_FADE));
	// モデルのマテリアルを取得
	int num = MV1GetMaterialNum(trans_.modelId);
	for (int i = 0; i < num; i++)
	{
		// モデルのディフューズカラーを変更
		MV1SetMaterialDifColor(trans_.modelId, i, c);
	}
	// モデルの描画
	MV1DrawModel(trans_.modelId);

	//HPUI表示
	hpUi_->Draw();

	//煙エフェクト2D
	if (fadeStep_ < 0.0f) {
		VECTOR pos = VAdd(trans_.pos, LOCAL_SMOKE_POS);
		DrawBillboard3D(
			pos,
			SMOKE_CENTER_POS, SMOKE_CENTER_POS,
			SMOKE_SCALE,
			0.0f,
			imgSmoke_[smokeNum_],
			true);
	}
}

void ChickenBase::DrawEnd()
{
	//描画を行わない
}

void ChickenBase::AliveIdle()
{
	//ターゲットを狙う
	LookTarget();

	//アニメーションを初期化
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::AliveRotation()
{
	ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);

	VECTOR addAxis = AsoUtility::VECTOR_ZERO;
	addAxis.y = 0.8f;

	if (!AsoUtility::EqualsVZero(addAxis))
	{
		//今回回転させたい回転量をクォータニオンで作る
		Quaternion rotPow = Quaternion();
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.x),
				AsoUtility::AXIS_X));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.y),
				AsoUtility::AXIS_Y));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.z),
				AsoUtility::AXIS_Z));

		// 回転量を加える(合成)
		trans_.quaRot = trans_.quaRot.Mult(rotPow);
	}

	// 前方向を取得
	VECTOR forward = trans_.GetForward();

	// 移動
	trans_.pos = VAdd(trans_.pos, VScale(forward, moveSpeed_));
}

void ChickenBase::AliveCall()
{
	//ターゲットを注目
	LookTarget();

	ResetAnim(ANIM::UNIQUE_1, DEFAULT_SPEED_ANIM);
}

void ChickenBase::LookTarget()
{
	//標的への方向ベクトルを取得						※TODO:ベクトルはSceneGameからもらう
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
}

void ChickenBase::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::UNIQUE_1:
		//ループ再生
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DAMAGE:
		ChangeState(STATE::ALIVE);
		break;

	case UnitBase::ANIM::DEATH:
		break;
	}
}

void ChickenBase::CheckIsHelp()
{
	//画像が非表示の場合処理しない
	if (!isHelp_) { return; }

	isHelpCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	if (isHelpCnt_ <= 0.0f)
	{
		isHelp_ = false;
	}

}

void ChickenBase::DrawHelp()
{
	//3Dから2Dへ座標変換
	VECTOR pos = VAdd(trans_.pos, LOCAL_HELP_POS);
	pos = ConvWorldPosToScreenPos(pos);

	//画像表示
	if (isHelp_ &&
		(state_ == STATE::ALIVE ||
			state_ == STATE::DAMAGE)) {

		DrawRotaGraph(
			pos.x,
			pos.y,
			1.0f,
			0.0f,
			imgHelp_,
			true);
	}
}

void ChickenBase::DebagUpdate()
{
	auto& ins = InputManager::GetInstance();

	//倒れる処理の確認
	if (ins.IsTrgDown(KEY_INPUT_K))
	{
		ChangeState(STATE::DEATH);
	}
	//ダメージ処理の確認
	else if (ins.IsTrgDown(KEY_INPUT_O))
	{
		ChangeState(STATE::DAMAGE);
		//SetDamage(50,);
	}
}

void ChickenBase::DebagDraw()
{
	constexpr int DIV_NUM = 20;
	constexpr int INTERVAL = 16;
	int cnt = 0;
	bool fill = false;
	Vector2 pos = { 0,Application::SCREEN_SIZE_Y - INTERVAL };

	//パラメーターの描画
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "攻撃力%d", atkPow_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "防御力%d", def_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "スピード%d", moveSpeed_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "体力%d", hp_);

	//当たり判定の描画
	DrawSphere3D(trans_.pos, radius_, DIV_NUM, AsoUtility::RED, AsoUtility::RED, fill);
}
