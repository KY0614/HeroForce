#pragma once
#include"Common/Transform.h"

class Arrow
{
public:

	//定数
	static constexpr VECTOR ARROW_LOCAL_POS = { 0.0f,5.0f,0.0f };	//打ち始めに違和感がないように調整
	static constexpr float GRAVITY = 0.2f;		//重力

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	//モデルID、発生位置、行き先,方向
	Arrow(const int _mdlId,const Transform& _trans);	
	~Arrow(void) = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

private:
	//位置情報
	Transform trans_;
	//攻撃対象
	VECTOR targetPos_;
	//状態
	STATE state_;
	//攻撃力
	float atkPow_;
	//速度
	float speed_;
	//生存判定
	bool isAlive_;

	//移動処理
	void Move(void);
	//状況変位
	void ChangeState(STATE _state) { state_ = _state; }
};

