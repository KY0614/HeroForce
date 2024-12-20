#include "ChickenBase.h"

ChickenBase::ChickenBase()
{
	moveSpeed_ = -1.0f;
	fadeStep_ = -1.0f;
	state_ = STATE::NONE;
	aliveState_ = ALIVE_MOVE::MAX;
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ALIVE , std::bind(&ChickenBase::ChangeStateAlive, this));
	stateChanges_.emplace(STATE::DAMAGE , std::bind(&ChickenBase::ChangeStateDamage, this));
	stateChanges_.emplace(STATE::DEATH , std::bind(&ChickenBase::ChangeStateDeath, this));

	// 生存状態管理
	aliveChanges_.emplace(ALIVE_MOVE::IDLE, std::bind(&ChickenBase::ChangeAliveIdle, this));
	aliveChanges_.emplace(ALIVE_MOVE::ROTATION, std::bind(&ChickenBase::ChangeAliveWalk, this));
	aliveChanges_.emplace(ALIVE_MOVE::CALL, std::bind(&ChickenBase::ChangeAliveCall, this));
}

ChickenBase::~ChickenBase()
{
}

void ChickenBase::Create(VECTOR &pos)
{
	//情報の受け取り
	trans_.pos = pos;		//生成位置
	
	//モデル設定
	ModelSet();

	//アニメーション管理番号の初期化
	InitAnimNum();

	//パラメーター設定
	SetParam();
}

void ChickenBase::Update(void)
{
	//デバッグ処理
	DebagUpdate();

	//アニメーションカウント
	Anim();

	// 更新ステップ
 	stateUpdate_();

	//トランスフォーム更新
	trans_.Update();
}

void ChickenBase::Draw(void)
{
	stateDraw_();

	//デバッグ描画
	DebagDraw();
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

void ChickenBase::SetParam()
{
	//移動スピード
	moveSpeed_ = DEFAULT_SPEED;

	//衝突判定用半径
	radius_ = RADIUS;

	// フェード時間
	fadeStep_ = TIME_FADE;
	
	//生存時の行動をランダムで決める
	aliveState_ = static_cast<ALIVE_MOVE>(GetRand(ALIVE_MOVE_MAX - 1));

	//初期状態(※生存時の状態を決めてから行う)
    ChangeState(STATE::ALIVE);

	//アニメーション状態
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CALL);
}

void ChickenBase::SetTarget(const VECTOR pos)
{
	targetPos_ = pos;
}

void ChickenBase::SetDamage(const int damage)
{
	hp_ -= damage;
}

void ChickenBase::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

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
}

void ChickenBase::ChangeStateDeath()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDeath, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDeath, this);
}

void ChickenBase::ChangeAliveState(ALIVE_MOVE state)
{
	// 状態変更
	aliveState_ = state;

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
	//("#'ψ')~~することなし
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
	fadeStep_ -= SceneManager::GetInstance().GetDeltaTime();
	if (fadeStep_ < 0.0f)
	{
		return;
	}

	//アニメーションを変更
	ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM);
}

void ChickenBase::DrawNone()
{

}

void ChickenBase::DrawAlive()
{
	MV1DrawModel(trans_.modelId);
}

void ChickenBase::DrawDamage()
{
	MV1DrawModel(trans_.modelId);
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
}

void ChickenBase::AliveIdle()
{
	LookTarget();

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

void ChickenBase::FinishAnim(void)
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
		//1回で終了
		break;
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
	}
}

void ChickenBase::DebagDraw()
{
	int divNum = 20;
	int color = 0xff00ff;
	bool fill = false;

	//当たり判定の描画
	DrawSphere3D(trans_.pos, radius_, divNum, color, color, fill);
}
