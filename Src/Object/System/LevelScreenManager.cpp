#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Decoration/EffectManager.h"
#include "../../Manager/GameSystem/DataBank.h"
#include "../../Common/Vector2.h"
#include "../../Common/Fader.h"
#include "../../Common/ShaderFade.h"
#include "LevelupNotice.h"
#include "LevelupSelect.h"

LevelScreenManager::LevelScreenManager(void)
{
	notice_ = nullptr;
	select_ = nullptr;
	fader_ = nullptr;
	exp_ = -1.0f;
	restExp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;
	isFader_ = false;

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelScreenManager::ChangeStateNone, this));
	stateChanges_.emplace(STATE::NOTICE, std::bind(&LevelScreenManager::ChangeStateNotice, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelScreenManager::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::END, std::bind(&LevelScreenManager::ChangeStateEnd, this));
}

LevelScreenManager::~LevelScreenManager(void)
{
}

void LevelScreenManager::Init(void)
{
	//���x���̏����ݒ�
	nowLevel_ = 1;
	restExp_ = 0;

	//�C���X�^���X�ݒ�
	Load();

	//����������
	Reset();

	//�O�f�[�^�̕ێ�
	preTypeData_.resize(playerNum_, TYPE::MAX);
}

void LevelScreenManager::Update(void)
{
	//�t�F�[�h�X�V����
	fader_->Update();

	stateUpdate_();
}

void LevelScreenManager::ChangeState(const STATE state)
{
	// ��Ԏ󂯎��
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void LevelScreenManager::ChangeStateNone()
{	
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNone, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNone, this);
}

void LevelScreenManager::ChangeStateNotice()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNotice, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNotice, this);
}

void LevelScreenManager::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateSelect, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawSelect, this);
}

void LevelScreenManager::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateEnd, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawEnd, this);
}

void LevelScreenManager::UpdateNone(void)
{
	//�f�o�b�O����(B�Ōo���l�𓾂�)
	DebagUpdate();

	//�o���l�̊m�F
	CheckExp();
}

void LevelScreenManager::UpdateNotice(void)
{
	if (!isFader_) {
		//�����̊g��
		notice_->Update();

		//�w�i��ʂ̓��ߏ���
		alpha_ += ALPHA_SPEED;
		if (alpha_ >= ALPHA_MAX)
		{
			alpha_ = ALPHA_MAX;
		}

		//�����̏I���m�F
		if (notice_->GetState() == LevelupNotice::STATE::FIN)
		{
			isFader_ = true;
			fader_->SetFade(Fader::STATE::FADE_OUT);
		}
	}
	else
	{
		Fade();
	}
}

void LevelScreenManager::UpdateSelect(void)
{
	if (!isFader_)
	{
		Fade();
	}
	else
	{
		//�v���C���[�̋����̑I������
		select_->Update();

		//�����̏I���m�F
		if (select_->GetState() == LevelupSelect::STATE::FIN)
		{
			ChangeState(STATE::END);
		}
	}
}

void LevelScreenManager::UpdateEnd(void)
{
	//�����v�f�ݒ�
	for (int i = 0; i < playerNum_; i++) {
		selectTypes_[i] = select_->GetType(i);
		preTypeData_[i] = selectTypes_[i];
	}

	//�e�탊�Z�b�g
	ChangeState(STATE::NONE);	//��ԕω�
	notice_->Reset();			//�ʒm�����̃��Z�b�g
	select_->Reset();			//�I�������̃��Z�b�g
	alpha_ = 0.0f;
}

void LevelScreenManager::Draw(void)
{
	Vector2 pos = { 0,0 };

	DrawLevelUI();
	
	//�������̓t�F�[�h���s��
	if(state_ != STATE::NONE){ FaderDraw(); }

	//��ԕʕ`�揈��
	stateDraw_();

	//�f�o�b�O�`��
	//DebagDraw();

	//�t�F�[�_�[�`��
	fader_->Draw();
}

void LevelScreenManager::DrawNone()
{
	//~(���ց�)~
}

void LevelScreenManager::DrawNotice()
{
	notice_->Draw();
}

void LevelScreenManager::DrawSelect()
{
	select_->Draw();
}

void LevelScreenManager::DrawEnd()
{
}

void LevelScreenManager::DrawLevelUI()
{
	Vector2 pos = {
		GAGE_IMG_SIZE / 2 + GAGE_POS_X,
		GAGE_IMG_SIZE / 2 + GAGE_POS_Y };
	float percent = 100.0f / gauge_ * exp_;

	//�o���l�Q�[�W
	DrawRotaGraph(pos.x, pos.y,
		GAGE_SCALE_RATE, 0.0f, imgGage_, true, false);

	DrawCircleGauge(pos.x, pos.y,
		percent, imgGageExp_, 0.0f, GAGE_SCALE_RATE);

	//���݃��x��
	if (nowLevel_ < 10) {	//1���̏ꍇ
		DrawRotaGraph(pos.x, pos.y, GAGE_SCALE_RATE, 0.0f, imgNumbers_[nowLevel_], true, false);
	}
	else {	//2���̏ꍇ
		int leftNum = nowLevel_ / 10;
		int rightNum = nowLevel_ % 10;

		//������
		Vector2 leftPos = pos;
		leftPos.x -= 64 / 1.5;
		DrawRotaGraph(leftPos.x, leftPos.y, GAGE_SCALE_RATE, 0.0f, imgNumbers_[leftNum], true, false);
		//�E����
		Vector2 rightPos = pos;
		rightPos.x += 32 * 1.5f;
		DrawRotaGraph(rightPos.x, rightPos.y, GAGE_SCALE_RATE, 0.0f, imgNumbers_[rightNum], true, false);
	}
}

void LevelScreenManager::Release(void)
{
	notice_->Release();
	select_->Release();
}

void LevelScreenManager::Load(void)
{
	//�e�C���X�^���X�ǂݍ���
	notice_ = std::make_unique<LevelupNotice>();
	notice_->Init();

	select_ = std::make_unique<LevelupSelect>();
	select_->Init();

	fader_ = std::make_unique<ShaderFade>();
	fader_->Init();

	//�摜
	imgGage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_GAGE).handleId_;
	imgGageExp_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_EXP_GAGE).handleId_;
	imgNumbers_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBERS).handleIds_;

	//�G�t�F�N�g�֌W
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::ATTACK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ATTACK_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::DEFENCE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::DEFENCE_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::SPEED_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEED_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::LIFE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::LIFE_UP_EFE).handleId_);
}

void LevelScreenManager::Reset()
{
	//�e�v���C���[���Ƃ̋����v�f������
	playerNum_ = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	selectTypes_.resize(playerNum_, TYPE::MAX);

	//�o���l�Q�[�W��
	gauge_ = CONSTANT_GAGE;

	//�����o���l
	exp_ = 0.0f;

	//��ʓ��ߒl
	alpha_ = 0.0f;

	//���
	ChangeState(STATE::NONE);
}

void LevelScreenManager::AddExp(const float value)
{
	restExp_ += value;
}

void LevelScreenManager::SetGage(const int level)
{
	gauge_ = ((level * 10) + 100) * CONSTANT_GAGE;
}

void LevelScreenManager::Reflection(PlayerBase& player, const int playerNum)
{
	//���f����
	TYPE type = selectTypes_[playerNum];

	switch (type)
	{
	case TYPE::ATTACK:
		player.SetAttack(3.0f);
		//�����̒l�͏㏸%�̒l
		//���݂̃X�e�[�^�X��%�ŏ㏸������
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::ATTACK_UP,
			player.GetPos(),
			Quaternion(),
			EFFECT_SCALE,SoundManager::SOUND::NONE);
		break;

	case TYPE::DEFENSE:
		player.SetDefense(3.0f);
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::DEFENCE_UP,
			player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::NONE);
		break;

	case TYPE::LIFE:
		player.SetHpMax(3);
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::LIFE_UP,
			player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::NONE);

		break;

	case TYPE::SPEED:
		player.SetSpeed(3.0f);
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::SPEED_UP,
			player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::NONE);
		break;

	default:
		break;
	}

	selectTypes_[playerNum] = TYPE::MAX;

}

void LevelScreenManager::CheckExp()
{
	//�o���l��1�����₷
	if (restExp_ > 0)
	{
		restExp_--;
		exp_++;
	}

	//�o���l�����̃��x�������܂ŒB������
	if (exp_ >= gauge_)
	{
		//���x�����グ��
		nowLevel_++;	
		
		//�o���l�̏�����
		exp_ = 0.0f;					
		
		//�ʒm�Ɉڂ�			
		ChangeState(STATE::NOTICE);

		//���x���̎擾
		notice_->SetNewLevel(nowLevel_);
	}
}

void LevelScreenManager::EffectSyne(PlayerBase& player, const int playerNum)
{
	//���f����
	TYPE type = preTypeData_[playerNum];
	EffectManager::EFFECT effectType = EffectManager::EFFECT::NONE;
	switch (type)
	{
	case TYPE::ATTACK:
		effectType = EffectManager::EFFECT::ATTACK_UP;
		break;

	case TYPE::DEFENSE:
		effectType = EffectManager::EFFECT::DEFENCE_UP;
		break;

	case TYPE::LIFE:
		effectType = EffectManager::EFFECT::LIFE_UP;
		break;

	case TYPE::SPEED:
		effectType = EffectManager::EFFECT::SPEED_UP;
		break;

	default:
		break;
	}

	//�G�t�F�N�g�ݒ�
	EffectManager::GetInstance().SyncEffect(
		effectType,
		player.GetPos(),
		Quaternion(),
		EFFECT_SCALE);
}

inline LevelScreenManager::TYPE LevelScreenManager::GetType(const int playerNum) const
{
	return selectTypes_[playerNum];
}

LevelScreenManager::TYPE LevelScreenManager::GetPreType(const int playerNum) const
{
	return preTypeData_[playerNum];
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

void LevelScreenManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isFader_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);

			//��ԕύX
			ChangeState(STATE::SELECT);
		}
		break;
	}
}

void LevelScreenManager::DebagUpdate()
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_B))
	{
		AddExp(30);
	}
}

void LevelScreenManager::DebagDraw()
{
	Vector2 pos = { 0,0 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "���݂̌o���l%2f", exp_);
	pos = { 0,16 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "���݂̃��x��%d", nowLevel_);
	pos = { 0,32 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "���݂̃X�e�[�g%d", static_cast<int>(state_));
}