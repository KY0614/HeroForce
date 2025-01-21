#pragma once
#include"Common/Transform.h"
#include"UnitBase.h"
#include"Character/PlayableChara/PlayerBase.h"

class JobManagerBase
{
public:

	JobManagerBase(void);
	~JobManagerBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	PlayerBase* GetPlayer(void) { return obj_; }

	//Transform��������
	const Transform& GetTransform(void)const { return obj_->GetTransform(); }

	//�����蔻��
	const float GetRadius(void)const { return obj_->GetRadius(); }

	//�ʒu
	const VECTOR GetPos(void)const { return obj_->GetPos(); }

	//�h���
	const float GetDef(void)const { return obj_->GetDef(); }
	//�U���֌W
	const UnitBase::ATK GetAtk(void)const { return obj_->GetAtk(); }

	//�O�̃t���[���ʒu�ɒu��
	void SetPrePos(void) { obj_->SetPrePos(); }

protected:

	PlayerBase* obj_;	//�g�p����C���X�^���X�i�[
};

