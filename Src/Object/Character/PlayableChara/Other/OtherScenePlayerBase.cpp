#include"../../../../Manager/GameSystem/DataBank.h"
#include"../../../../Utility/AsoUtility.h"
#include "OtherScenePlayerBase.h"

using S_ROLE = SceneManager::ROLE;

OtherScenePlayerBase::OtherScenePlayerBase()
{
	for (int i = 0; i < PLAYERS; i++) { role_[i] = S_ROLE::NONE; }
}

void OtherScenePlayerBase::Init()
{
	//役職設定
	for (int i = 0; i < PLAYERS; i++)
	{
		role_[i] = DataBank::GetInstance().Output(i + 1).role_;
	}

	//3Dモデル初期化
	Init3DModel();

	//アニメーションの初期化
	InitAnim();
}

void OtherScenePlayerBase::Update()
{
	//アニメーション
	Animation();

	for (auto& tran_ : transArray_)
	{
		//モデルの初期化
		tran_.Update();
	}
}

void OtherScenePlayerBase::Draw()
{
	for (int i = 0; i < PLAYERS; i++)
	{
		MV1DrawModel(transArray_[i].modelId);
	}
}

void OtherScenePlayerBase::Init3DModel()
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

		//モデルの設定
		transArray_[i].pos = PLAYER_POS[i];
		transArray_[i].scl = SCALE;
		transArray_[i].quaRot = Quaternion();
		transArray_[i].quaRotLocal = Quaternion::Euler(
			0.0f,
			AsoUtility::Deg2RadF(PLAYER_ROT[i]),
			0.0f);
		transArray_[i].Update();
	}
}

void OtherScenePlayerBase::Animation()
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
	for (auto& t : transArray_)
	{
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OtherScenePlayerBase::ResetAnim(const ANIM _anim, const float _speed)
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