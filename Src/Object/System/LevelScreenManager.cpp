#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Fader.h"
#include "LevelScreenManager.h"
#include "LevelupNotice.h"
#include "LevelupSelect.h"

LevelScreenManager::LevelScreenManager(void)
{
	fader_ = nullptr;
	notice_ = nullptr;
	select_ = nullptr;
	exp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;
	type_ = TYPE::MAX;
}

LevelScreenManager::~LevelScreenManager(void)
{
}

void LevelScreenManager::Init(void)
{
	//�C���X�^���X�ݒ�
	Load();

	//����������
	Reset();
}

void LevelScreenManager::Update(void)
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

void LevelScreenManager::NoticeUpdate(void)
{
	//Fader�̍X�V����
	fader_->Update();
	
	//�����̊g��	
	notice_->Update();	

	//��ʂ̓��ߏ���
	alpha_+= ALPHA_SPEED;
	if (alpha_ >= ALPHA_MAX)
	{
		alpha_ = ALPHA_MAX;
	}

	//��ԕύX�̊m�F
	if (notice_->GetState() == LevelupNotice::STATE::FIN)
	{
		state_ = STATE::SELECT;					//�����̃X�e�[�g��I���ɂ���
		select_->SetState					
		(LevelupSelect::STATE::EXPANSION);		//�I�������̃X�e�[�g��ς��Ƃ�
	}
}

void LevelScreenManager::PowerUpdate(void)
{
	select_->Update();						//�v���C���[�̋����̑I������

	if (select_->GetState() == LevelupSelect::STATE::FIN)
	{
		type_ = select_->GetType();	//�����v�f�̎擾
		state_ = STATE::FIN;		//�������f�̏����ֈڂ�
		notice_->Reset();			//�ʒm�����̃��Z�b�g
		select_->Reset();			//�I�������̃��Z�b�g
		alpha_ = 0.0f;
	}
}

void LevelScreenManager::Draw(void)
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
		select_->Draw();
		break;	
	};
}

void LevelScreenManager::Release(void)
{
	delete select_;
	delete notice_;
	delete fader_;
}

void LevelScreenManager::Load(void)
{
	//�e�C���X�^���X�ǂݍ���
	fader_ = new Fader();
	fader_->Init();
	fader_->SetAlpha(Fader::LITTLE_ALPHA);

	notice_ = new LevelupNotice();
	notice_->Init();

	select_ = new LevelupSelect();
	select_->Init();
}

void LevelScreenManager::Reset()
{
	//������Ԃ̐ݒ�
	gauge_ = 30;
	exp_ = 0.0f;
	alpha_ = 0.0f;
}

void LevelScreenManager::SetExp(const float value)
{
	exp_ += value;
}

void LevelScreenManager::SetGage(const int level)
{
	//�G�̌o���l�ʂ����߂ďC���\��
	gauge_ = level * CONSTANT_GAGE;
}

void LevelScreenManager::SetState(const STATE state)
{
	state_ = state;
}

void LevelScreenManager::CheckExp()
{
	//�o���l�����̃��x�������܂ŒB������
	if (exp_ >= gauge_)
	{
		//���x�����グ��
		nowLevel_++;	
		
		//�o���l�̏�����
		exp_ = 0.0f;					

		//�摜�̃t�F�[�h�����̐ݒ�;						
		notice_->SetState
		(LevelupNotice::STATE::FADE_IN);

		//��ʑS�̂̃t�F�[�h�����̐ݒ�
		fader_->SetFade
		(Fader::STATE::SET_FADE_OUT);
		
		//�ʒm�Ɉڂ�			
		SetState(STATE::NOTICE);
	}
}

void LevelScreenManager::FaderDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelScreenManager::Debag()
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_B))
	{
		SetExp(1);
	}
}
