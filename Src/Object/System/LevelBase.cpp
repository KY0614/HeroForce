#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "LevelBase.h"

LevelBase::LevelBase(void)
{
	exp_ = 0.0f;
	nowLevel_ = 0;
	gauge_ = 0.0f;
}

LevelBase::~LevelBase(void)
{
}

void LevelBase::Destroy(void)
{
}

void LevelBase::Init(void)
{
	//�摜�̓ǂݍ���
	Load();

	//������Ԃ̐ݒ�
	gauge_ = 30;
}

void LevelBase::Update(void)
{
	Debag();

	CheckExp();
}

void LevelBase::Draw(void)
{
	Vector2 pos = { 0,0 };
	DrawFormatString(pos.x,pos.y,0xffffff,"���݂̌o���l%2f", exp_);

	pos = { 0,16 };
	DrawFormatString(pos.x,pos.y,0xffffff,"���݂̃��x��%d", nowLevel_);
}

void LevelBase::Load(void)
{
}

void LevelBase::SetExp(float value)
{
	exp_ += value;
}

void LevelBase::SetGage(int level)
{
	//�G�̌o���l�ʂ����߂ďC���\��
	gauge_ = level * CONSTANT_GAGE;
}

void LevelBase::CheckExp()
{
	//�o���l�����̃��x�������܂ŒB������
	if (exp_ >= gauge_)
	{
		nowLevel_++;	//���x�����グ��
		exp_ = 0;		//�o���l�̏�����
	}
}

void LevelBase::Debag()
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_B))
	{
		SetExp(1);
	}
}
