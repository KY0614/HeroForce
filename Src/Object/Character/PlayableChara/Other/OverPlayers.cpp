#include "OverPlayers.h"

using S_ROLE = SceneManager::ROLE;

OverPlayers::OverPlayers(void)
{
	for (int i = 0; i < PLAYERS; i++) { role_[i] = S_ROLE::NONE; }
}

void OverPlayers::Init(void)
{
	//役職設定
	for (int i = 0; i < PLAYERS; i++)
	{
		role_[i] = DataBank::GetInstance().Output(i + 1).role_;
	}

	//3Dモデル初期化
	Init3DModel();

	//アニメーションの設定
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	animNum_.emplace(ANIM::DEATH, DEATH_ANIM);
	ResetAnim(ANIM::DEATH, ANIM_SPEED);
}

void OverPlayers::Update(void)
{
	Animation();

	for (auto& tran_ : transArray_)
	{
		//モデルの初期化
		tran_.Update();
	}
}

void OverPlayers::Draw(void)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		MV1DrawModel(transArray_[i].modelId);

		//モデルの初期化
		transArray_[i].Update();
	}
}

void OverPlayers::Init3DModel(void)
{
	//役職モデルの設定
	for (int i = 0; i < PLAYERS; i++) {

		switch (role_[i])
		{
		case S_ROLE::KNIGHT:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
			break;

		case S_ROLE::AXEMAN:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
			break;

		case S_ROLE::MAGE:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
			break;

		case S_ROLE::ARCHER:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
			break;

		default:
			break;
		}

		//オブジェクトごとに座標位置の設定
		float deg = -1;
		if (i == 0) { transArray_[i].pos = PLAYER_POS_1; deg = PLAYER_ROT_1; }
		else if (i == 1) { transArray_[i].pos = PLAYER_POS_2; deg = PLAYER_ROT_2; }
		else if (i == 2) { transArray_[i].pos = PLAYER_POS_3; deg = PLAYER_ROT_3; }
		else if (i == 3) { transArray_[i].pos = PLAYER_POS_4; deg = PLAYER_ROT_4; }

		//その他設定
		transArray_[i].scl = SCALE;
		transArray_[i].quaRot = Quaternion();
		transArray_[i].quaRotLocal = Quaternion::Euler(
			0.0f,
			AsoUtility::Deg2RadF(deg),
			0.0f);
		transArray_[i].Update();
	}
}

void OverPlayers::Animation()
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		//アニメーション終了時処理（継承先で行動を決めておく）
		FinishAnim();
	}
	// 再生するアニメーション時間の設定
	for (auto& t : transArray_) {
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OverPlayers::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//アニメーションスピードの変更
	speedAnim_ = _speed;

	//アニメーションステート設定
	anim_ = _anim;
	stepAnim_ = 0.0f;

	for (auto& t : transArray_)
	{
		//デタッチ
		//実質atcAnimの初期化
		MV1DetachAnim(t.modelId, atcAnim_);

		//アタッチ
		//実質atcAnimの代入
		atcAnim_ = MV1AttachAnim(t.modelId, animNum_[anim_]);

		animTotalTime_ = MV1GetAttachAnimTotalTime(t.modelId, atcAnim_);


		// 再生するアニメーション時間の設定
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OverPlayers::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
		//ループ再生
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DEATH:
		ResetAnim(ANIM::IDLE, ANIM_SPEED);
		break;
	}
}
