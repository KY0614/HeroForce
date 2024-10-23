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
	Arrow(void);	
	~Arrow(void) = default;

	void Init(const int _mdlId, const Transform& _trans);
	void Update(void);
	void Draw(void);
	void Release();
	void Destroy(void);

	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

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
	void ChangeState(const STATE _state) { state_ = _state; }
	//�����t���O�ݒ�
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }
};

