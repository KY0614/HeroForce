#include"../Utility/AsoUtility.h"
#include "Arrow.h"

Arrow::Arrow(const int _mdlId, const Transform& _trans)
{
	//引数の引継ぎ
	trans_ = _trans;
	trans_.modelId = _mdlId;

	//諸々モデルの初期化
	trans_.pos = VAdd(trans_.pos, ARROW_LOCAL_POS);
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.quaRot = Quaternion::LookRotation(trans_.rot);
	trans_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);

	trans_.Update();

	ChangeState(STATE::SHOT);
	atkPow_ = 0.0f;
	speed_ = 1.0f;
}

void Arrow::Init(void)
{
	
}

void Arrow::Update(void)
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
}

void Arrow::Draw(void)
{
	//ショット状態のときにのみ描画する
	if (state_ == STATE::SHOT)MV1DrawModel(trans_.modelId);
}

void Arrow::Destroy(void)
{
}

void Arrow::Move(void)
{
	// 前方向を取得
	VECTOR forward = trans_.GetForward();
	//下方向の取得
	VECTOR downward = trans_.GetDown();

	// 移動
	//前方
	trans_.pos =
		VAdd(trans_.pos, VScale(forward, speed_));
	//下方
	trans_.pos =
		VAdd(trans_.pos, VScale(downward, GRAVITY));
}
