#pragma once
#include"Common/Transform.h"

class Arrow
{
public:

	//�萔
	static constexpr VECTOR ARROW_LOCAL_POS = { 0.0f,5.0f,0.0f };	//�ł��n�߂Ɉ�a�����Ȃ��悤�ɒ���
	static constexpr float GRAVITY = 0.2f;		//�d��

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	//���f��ID�A�����ʒu�A�s����,����
	Arrow(const int _mdlId,const Transform& _trans);	
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
	//���
	STATE state_;
	//�U����
	float atkPow_;
	//���x
	float speed_;
	//��������
	bool isAlive_;

	//�ړ�����
	void Move(void);
	//�󋵕ψ�
	void ChangeState(STATE _state) { state_ = _state; }
};

