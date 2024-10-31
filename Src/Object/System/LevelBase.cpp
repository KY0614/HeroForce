#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Fader.h"
#include "LevelBase.h"
#include "LevelupNotice.h"
#include "PowerSelect.h"

LevelBase::LevelBase(void)
{
	fader_ = nullptr;
	notice_ = nullptr;
	power_ = nullptr;
	exp_ = 0.0f;
	nowLevel_ = 0;
	gauge_ = 0.0f;
	state_ = STATE::NONE;
}

LevelBase::~LevelBase(void)
{
}

void LevelBase::Destroy(void)
{
}

void LevelBase::Init(void)
{
	//�C���X�^���X�ݒ�
	Load();

	//������Ԃ̐ݒ�
	gauge_ = 30;
}

void LevelBase::Update(void)
{
	switch (state_)
	{
	case STATE::NONE:
		Debag();		//�f�o�b�O����(B�Ōo���l�𓾂�)
		CheckExp();		//�o���l�̊m�F
		break;

	case STATE::NOTICE:
		NoticeUpdate();	//�ʒm����
		break;

	case STATE::SELECT:
		PowerUpdate();	//�I������
		break;

	};
	
}

void LevelBase::NoticeUpdate(void)
{
	//Fader�̍X�V����
	fader_->Update();
	
	//�����̊g��	
	notice_->Update();						

	if (notice_->GetState() == LevelupNotice::STATE::FIN)
	{
		state_ = STATE::SELECT;					//�����̃X�e�[�g��I���ɂ���
		power_->SetState					
		(PowerSelect::STATE::EXPANSION);		//�I�������̃X�e�[�g��ς��Ƃ�
	}
}

void LevelBase::PowerUpdate(void)
{
	power_->Update();						//�v���C���[�̋����̑I������

	if (power_->GetState() == PowerSelect::STATE::FIN)
	{
		state_ = STATE::NONE;
		notice_->Reset();
		power_->Reset();
	}
}

void LevelBase::Draw(void)
{
	Vector2 pos = { 0,0 };

	//�������̓t�F�[�h���s��
	if(state_ != STATE::NONE){ FaderDraw(); }

	switch (state_)
	{
	case STATE::NONE:
	DrawFormatString
	(pos.x,pos.y,0xffffff,"���݂̌o���l%2f", exp_);
	pos = { 0,16 };
	DrawFormatString
	(pos.x,pos.y,0xffffff,"���݂̃��x��%d", nowLevel_);
		break;

	case STATE::NOTICE:
		notice_->Draw();
		break;	
	
	case STATE::SELECT:
		power_->Draw();
		break;	
	};
}

void LevelBase::Release(void)
{
	delete power_;
	delete notice_;
	delete fader_;
}

void LevelBase::Load(void)
{
	fader_ = new Fader();
	fader_->Init();

	notice_ = new LevelupNotice();
	notice_->Init();

	power_ = new PowerSelect();
	power_->Init();
}

void LevelBase::SetExp(const float value)
{
	exp_ += value;
}

void LevelBase::SetGage(const int level)
{
	//�G�̌o���l�ʂ����߂ďC���\��
	gauge_ = level * CONSTANT_GAGE;
}

void LevelBase::SetState(const STATE state)
{
	state_ = state;
}

void LevelBase::CheckExp()
{
	//�o���l�����̃��x�������܂ŒB������
	if (exp_ >= gauge_)
	{
		//���x�����グ��
		nowLevel_++;	
		
		//�o���l�̏�����
		exp_ = 0;					

		//�摜�̃t�F�[�h�����̐ݒ�;						
		notice_->SetState
		(LevelupNotice::STATE::FADE_IN);

		//��ʑS�̂̃t�F�[�h�����̐ݒ�
		fader_->SetFade
		(Fader::STATE::FADE_NOTICE);
		
		//�ʒm�Ɉڂ�			
		SetState(STATE::NOTICE);
	}
}

void LevelBase::FaderDraw()
{
	fader_->Draw();
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
