#pragma once
#include"Common/Transform.h"

class Arrow
{
public:
	Arrow(void);
	~Arrow(void) = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

private:
	//ˆÊ’uî•ñ
	Transform trans_;
	//UŒ‚‘ÎÛ
	VECTOR targetPos_;
	//UŒ‚—Í
	float atkPow_;
	//‘¬“x
	float speed_;

};

