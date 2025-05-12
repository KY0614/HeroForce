#include"../Application.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Utility/AsoUtility.h"
#include "Arrow.h"

Arrow::Arrow(void)
{
}

void Arrow::Init(const int _mdlId, const Transform& _trans, const float _speed)
{
	//引数の引継ぎ
	trans_ = _trans;
	trans_.modelId = _mdlId;
	speed_ = _speed;

	//諸々モデルの初期化
	VECTOR localPos = trans_.quaRot.PosAxis(ARROW_LOCAL_POS);
	trans_.pos = VAdd(trans_.pos, localPos);

	trans_.scl = { 1.0f,1.0f,1.0f };
	//trans_.quaRot = trans_.quaRot.AngleAxis(START_UP_ANGLE, AsoUtility::AXIS_X);
	trans_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);

	trans_.Update();

	ChangeState(STATE::NONE);
	SetIsAlive(true);
}

void Arrow::Update(UnitBase::ATK& _atk)
{
	switch (state_)
	{
	case Arrow::STATE::NONE:
		break;
	case Arrow::STATE::SHOT:
		//移動処理
		Move();
		break;
	case Arrow::STATE::DESTROY:
		break;
	case Arrow::STATE::END:
		break;
	default:
		break;
	}

	// モデル制御の基本情報更新
	trans_.Update();

	//対応した攻撃の情報更新
	_atk.pos_ = trans_.pos;
}

void Arrow::Draw(void)
{
	//ショット状態のときにのみ描画する
	if (state_ == STATE::SHOT)//DrawSphere3D(trans_.pos, 5.0f, 20, 0x00ff00, 0x00ff00, true);
	MV1DrawModel(trans_.modelId);
}

void Arrow::Release()
{
}

void Arrow::Destroy(void)
{
	ChangeState(STATE::DESTROY);
	SetIsAlive(false);
}

void Arrow::Move(void)
{
	// 前方向を取得
	VECTOR forward = trans_.GetForward();
	//下方向の取得
	VECTOR downward = trans_.GetDown();

	//横ベクトル
	VECTOR widthMovePow = VScale(forward, speed_);

	// 移動
	//前方
	trans_.pos =
		VAdd(trans_.pos, widthMovePow);
	//下方
	trans_.pos =
		VAdd(trans_.pos, VScale(downward, GRAVITY));
}
