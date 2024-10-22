#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::Init(void)
{
	dodgeCdt_ = DODGE_CDT_MAX;
	color_ = 0xffffff;

	
	
	speedMove_ = 0.0f;


	//モデルの初期化
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	//アニメーション初期化
	animNum_[ANIM::IDLE] = IDLE_NUM;
	animNum_[ANIM::WALK] = WALK_NUM;
	animNum_[ANIM::DODGE] = DODGE_NUM;
	animNum_[ANIM::UNIQUE_1] = ATK_NUM;
	animNum_[ANIM::SKILL_1] = SKILL_NUM;
	ResetAnim(ANIM::IDLE);

	atk_.cnt_ = 0;
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.backlash_ = FRAME_ATK_BACKRASH;
	atk_.pow_ = 0;

	//スキル
	

	trans_.Update();

}

void PlayerBase::Update(void)
{
	//アニメーションの更新
	Anim();
	//モデルの更新
	trans_.Update();

	//停止状態の時のアニメーション
	if (!IsMove() && !IsDodge() && !IsAtkAction() && anim_ != ANIM::IDLE)
	{
		ResetAnim(ANIM::IDLE);
		speedMove_ = 0.0f;
	}

	//回避
	Dodge();


	//コントロール系
	KeyBoardControl();

	//攻撃中か毎フレーム判定する
	Attack();





}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (anim_!=ANIM::WALK&&!IsDodge()&&!IsAtkAction())
	{
		ResetAnim(ANIM::WALK);
		speedMove_ = SPEED_MOVE;
	}
	
	if (!atk_.IsAttack()&&!IsDodge() && !IsAtkAction())
	{
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, speedMove_);
		//移動処理
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//前
	if (ins.IsNew(KEY_INPUT_W))
	{
		Move(0.0f, AsoUtility::AXIS_Y);
	}
	//右
	else if (ins.IsNew(KEY_INPUT_D))
	{
		Move(90.0f, AsoUtility::AXIS_Y);
	}
	//下
	else if (ins.IsNew(KEY_INPUT_S))
	{
		Move(180.0f, AsoUtility::AXIS_Y);
	}
	//左
	else if (ins.IsNew(KEY_INPUT_A))
	{
		Move(270.0f, AsoUtility::AXIS_Y);
	}
	else
	{
		speedMove_ = 0.0f;
	}

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsTrgDown(KEY_INPUT_E)/*&&!atk_.IsAttack()*/)
	{
		if (!atk_.IsAttack()&&!IsDodge()&&anim_!=ANIM::UNIQUE_1)
		{
			//アニメーション
			ResetAnim(ANIM::UNIQUE_1);
			//カウンタ開始
			atk_.cnt_+=SceneManager::GetInstance().GetDeltaTime();
		}

	}

	if (ins.IsTrgDown(KEY_INPUT_Q)&&!IsAtkAction()&&!IsSkill(SKILL::TWO))
	{
		Skill_1();
		ResetAnim(ANIM::SKILL_1);
	}

	//回避
	if (ins.IsTrgDown(KEY_INPUT_N) && !IsDodge() && !IsAtkAction()&&!IsCoolDodge())
	{
		ResetAnim(ANIM::DODGE);
		frameDodge_+=SceneManager::GetInstance().GetDeltaTime();
	}

}

void PlayerBase::DrawDebug(void)
{
	//球体
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//値見る用
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%d)", atk_.cnt_, atk_.IsAttack(),atk_.IsBacklash_(), frameDodge_);
}



void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (atk_.IsAttack()||atk_.IsBacklash_())
	{
		atk_.cnt_+=SceneManager::GetInstance().GetDeltaTime();
		color_ = 0xff0000;
	}
	else 
	{
		atk_.ResetCnt();
		color_ = 0xffffff;
	}
	
}

void PlayerBase::Dodge(void)
{
	//ドッジフラグがtrueになったら
	if (IsDodge()&&!IsCoolDodge())
	{
		frameDodge_+=SceneManager::GetInstance().GetDeltaTime();
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = trans_.GetForward();
			//移動方向
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//移動処理
			trans_.pos = VAdd(trans_.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0.0f;
		}
	}
	else
	{
		dodgeCdt_ +=SceneManager::GetInstance().GetDeltaTime();
		ResetDodgeFrame();
		color_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
	skillNum_ = "Skill_1";
	skillCnt_[SKILL::ONE] += SceneManager::GetInstance().GetDeltaTime();
}

void PlayerBase::Skill_2(void)
{
	skillNum_ = "Skill_2";
}

