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
	//�ʒu���
	Transform trans_;
	//�U���Ώ�
	VECTOR targetPos_;
	//�U����
	float atkPow_;
	//���x
	float speed_;

};

