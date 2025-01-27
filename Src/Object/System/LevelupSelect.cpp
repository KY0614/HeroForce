#include <locale>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/GameSystem/TextManager.h"
#include "../../Manager/GameSystem/DataBank.h"
#include "LevelupSelect.h"
#include "Carsor.h"

LevelupSelect::LevelupSelect()
{
	fontMes_ = -1;
	fontExp_ = -1;
	plNum_ = -1;
	img_ = -1;
	imgBack_ = -1;
	element_ = -1;
	state_ = STATE::NONE;

	//�J�[�\���摜������
	int value = -1;
	imgCarsors_ = &value;

	//�摜
	for (int i = 0; i < LevelScreenManager::TYPE_MAX; i++) {
		imgSelects_[i] = -1;
	}

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelupSelect::ChangeStateNone, this));
	stateChanges_.emplace(STATE::EXPANSION, std::bind(&LevelupSelect::ChangeStateExpansion, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelupSelect::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::FIN, std::bind(&LevelupSelect::ChangeStateFin, this));
}

LevelupSelect::~LevelupSelect()
{
}

void LevelupSelect::Init()
{
	//���v���C���[�l��
	plNum_ = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);

	//�ǂݍ���
	Load();
	
	//�v�f���̌���
	element_ = SELECT_ELEMENT;
	ele_.resize(element_);

	//�J�[�\���̏�����
	carsors_.resize(plNum_);
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i] = std::make_unique<Carsor>();
		carsors_[i]->Init(i, imgCarsors_[i]);
		carsors_[i]->SetContllorKey(
			KEY_INPUT_RIGHT,
			KEY_INPUT_LEFT,
			KEY_INPUT_UP,
			KEY_INPUT_DOWN,
			KEY_INPUT_RETURN);
	}

	//�ϐ��̐ݒ�
	Reset();

	//�v�f�̈ʒu��ݒ�
	SetFirstPos();

	//�t�H���g����
	CreateFonts();
}

void LevelupSelect::Update()
{
	stateUpdate_();
}

void LevelupSelect::Draw()
{
	//�e�L�X�g�p�ϐ�
	int color = 0;
	int length = 0;
	Vector2 pos = { 0,0 };

	//�w�i
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgBack_,
		false);

	for (auto ele : ele_)
	{
		int type = static_cast<int>(ele.type_);

		//�摜�`��
		DrawRotaGraph(
			ele.pos_.x, ele.pos_.y,
			ele.scl_,
			0.0f,
			imgSelects_[type],
			true,
			false);

		if (state_ == STATE::SELECT)
		{
			if (ele.isText_) {

				//�e�L�X�g�`��
				int pow = 30;
				color = 0xffffff;
				length = expTexts_[type].length();
				pos = ele.pos_;
				pos.x -= length * FONT_EXP_SIZE / 4;
				pos.y += 150;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
				DrawBox(
					pos.x, pos.y,
					pos.x + length * FONT_EXP_SIZE / 2.3,
					pos.y + FONT_EXP_SIZE,
					0x000000, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				DrawFormatStringToHandle(
					pos.x, pos.y,
					color,
					fontExp_,
					expTexts_[type].c_str(),
					pow);
			}
		}
	}

	//�e�L�X�g�p�ϐ�
	color = 0xffffff;
	length = mesText_.length() / 2;
	pos = { MES_TEXT_POS_X,MES_TEXT_POS_Y };
	pos.x -= length * FONT_MES_SIZE / 2;

	//���b�Z�[�W��`��
	DrawFormatStringToHandle(
		pos.x, pos.y,
		color,
		fontMes_,
		mesText_.c_str());

	if (state_ == STATE::SELECT)
	{
		for (int i = 0; i < carsors_.size(); i++)
		{
			carsors_[i]->Draw();
		}
	}
}

void LevelupSelect::Release()
{
	DeleteFontToHandle(fontExp_);
	DeleteFontToHandle(fontMes_);
}

void LevelupSelect::Reset()
{
	//������Ԃ̐ݒ�
	ChangeState(LevelupSelect::STATE::NONE);

	//�����v�f�̎��
	ele_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)].type_ = LevelScreenManager::TYPE::ATTACK;
	ele_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)].type_ = LevelScreenManager::TYPE::DEFENSE;
	ele_[static_cast<int>(LevelScreenManager::TYPE::LIFE)].type_ = LevelScreenManager::TYPE::LIFE;
	ele_[static_cast<int>(LevelScreenManager::TYPE::SPEED)].type_ = LevelScreenManager::TYPE::SPEED;

	//�����v�f�֌W�̏�����
	for (int i = 0; i < ele_.size(); i++)
	{
		//�����g��l
		ele_[i].scl_ = 0.00f;

		//�����v�f�̎��
		//ele_[i].type_ = GetRandType();

		//�T�C�Y
		ele_[i].size_ = { static_cast<int>(ELE_IMG_SIZE_X * SCALE_MAX),
			static_cast<int>(ELE_IMG_SIZE_Y * SCALE_MAX) };

		//�e�L�X�g���\���ɂ��Ă���
		ele_[i].isText_ = false;
	}

	//�J�[�\��������
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i]->Reset();
	}

	//�e�v���C���[���Ƃ̋����v�f������
	selectTypes_.resize(plNum_);
	for (int i = 0; i < plNum_; i++) {
		selectTypes_[i] = LevelScreenManager::TYPE::MAX;
	}
}

void LevelupSelect::Load()
{
	//�摜
	auto& res = ResourceManager::GetInstance();
	img_ = res.Load(ResourceManager::SRC::SELECT_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)] = res.Load(ResourceManager::SRC::ATTACK_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)] = res.Load(ResourceManager::SRC::DEFENCE_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::LIFE)] = res.Load(ResourceManager::SRC::LIFE_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::SPEED)] = res.Load(ResourceManager::SRC::SPEED_UP_UI).handleId_;
	imgCarsors_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CARSOLS).handleIds_;
	imgBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP_BACK).handleId_;
	
	//�e�L�X�g�֌W
	auto& text_m = TextManager::GetInstance();
	mesText_ = text_m.TextLoad(TextManager::TEXTS::LV_UP_SELECT_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)] = text_m.TextLoad(TextManager::TEXTS::LV_ATK_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)] = text_m.TextLoad(TextManager::TEXTS::LV_DEF_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::SPEED)] = text_m.TextLoad(TextManager::TEXTS::LV_SPEED_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::LIFE)] = text_m.TextLoad(TextManager::TEXTS::LV_LIFE_MES);
}

void LevelupSelect::SetFirstPos()
{
	Vector2 pos;
	int cnt = 0;
	int div = 4;
	int center= 2;

	switch (ele_.size())
	{
	case 2:
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / 2 };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div * 3, Application::SCREEN_SIZE_Y / 2 };

	case 3:
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div * 3};
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / div * 3};
		break;

	case 4:
		ele_[cnt].pos_ = { ELEMENT_POS_LU_X,ELEMENT_POS_LU_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_RU_X,ELEMENT_POS_RU_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_LD_X,ELEMENT_POS_LD_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_RD_X,ELEMENT_POS_RD_Y };
		break;
	default:
		return;
		break;
	}
}

void LevelupSelect::CreateFonts()
{
	auto& text_m = TextManager::GetInstance();
	
	fontMes_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_MES_SIZE,
		FONT_MES_THICK);

	//�����p�e�L�X�g�t�H���g
	fontExp_ =CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_EXP_SIZE,
		FONT_EXP_THICK);
}

inline LevelScreenManager::TYPE LevelupSelect::GetRandType()
{
	int rand = GetRand(LevelScreenManager::TYPE_MAX - 1);
	LevelScreenManager::TYPE type = static_cast<LevelScreenManager::TYPE>(rand);
	return type;
}

void LevelupSelect::ChangeState(const STATE state)
{
	//��Ԏ󂯎��
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void LevelupSelect::ChangeStateNone()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateNone, this);
}

void LevelupSelect::ChangeStateExpansion()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateExpansion, this);
}

void LevelupSelect::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateSelect, this);
}

void LevelupSelect::ChangeStateFin()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateFin, this);
}

void LevelupSelect::UpdateNone()
{
	//�g�又���Ɉڂ�
	ChangeState(STATE::EXPANSION);
}

void LevelupSelect::UpdateExpansion()
{
	float scl = SCALE_RATE;
	float max = SCALE_MAX;

	for (int i = 0; i < ele_.size(); i++)
	{
		//�摜�̊g��
		ele_[i].scl_ += scl;

		//�ő�l�ɒB������
		if (ele_[i].scl_ >= max)
		{
			ele_[i].scl_ = max;

			//��ԕύX
			ChangeState(STATE::SELECT);
		}
	}
}

void LevelupSelect::UpdateSelect()
{
	//��ԕύX
	bool isChange = true;

	//�e�L�X�g��S�Ĕ�\���ɂ��Ă���
	for (int j = 0; j < ele_.size(); j++) {
		ele_[j].isText_ = false;
	}

	for (int i = 0; i < carsors_.size(); i++)
	{
		//�J�[�\���̍X�V����
		carsors_[i]->Update();

		for (int j = 0; j < ele_.size(); j++)
		{
			//�v���C���[�̋����v�f�����܂��Ă���ꍇ
			if (selectTypes_[i] != LevelScreenManager::TYPE::MAX)
			{
				//���̃v���C���[�̏������I������
				continue;
			}

			//�Փ˔���(�������ڂƃJ�[�\�����Փ˂��Ă�ꍇ)
			if (IsCollisionBoxCircle(
				ele_[j].pos_,
				ele_[j].size_,
				carsors_[i]->GetPos(),
				Carsor::RADIUS))
			{
				//�����v�f�̐�����\��
				ele_[j].isText_ = true;

				//�J�[�\��������̏ꍇ
				if (carsors_[i]->IsDecide()) 
				{
					//�I�����Ă���v�f���i�[
					selectTypes_[i] = ele_[j].type_;
				}
				else
				{
					//��ԕύX���s��Ȃ�
					isChange = false;
				}
			}
			//�������ĂȂ��ꍇ
			else
			{
				//��ԕύX���s��Ȃ�
				isChange = false;
			}
		}
		//����ς݂��ǂ���
		if (selectTypes_[i] == LevelScreenManager::TYPE::MAX)
		{
			//�J�[�\���𖢌���ɖ߂�
			carsors_[i]->SetDecide(false);
		}
	}
	//��ԑJ��
	if (isChange)
	{
		ChangeState(STATE::FIN);
	}
}

void LevelupSelect::UpdateFin()
{
}

void LevelupSelect::DebagDraw()
{
	for (auto ele : ele_)
	{
		Vector2 LUpos{ ele.pos_.x - ele.size_.x/2,
			ele.pos_.y - ele.size_.y/2 };
		Vector2 RDpos{ ele.pos_.x + ele.size_.x/2,
			ele.pos_.y + ele.size_.y/2 };

		DrawBox(LUpos.x ,LUpos.y, RDpos.x,RDpos.y,0xff00ff,true);
	}
}

bool LevelupSelect::IsCollisionBoxCircle(Vector2 pos1, Vector2 size1, Vector2 pos2, float radius2)
{
	Vector2 righttop = pos1;
	righttop.x += size1.x / 2;
	righttop.y -= size1.y / 2;
	righttop = V2Sub(pos2, righttop);
	int rightTop = std::hypot(righttop.x, righttop.y);

	Vector2 rightdown = pos1;
	rightdown.x += size1.x / 2;
	rightdown.y += size1.y / 2;
	rightdown = V2Sub(pos2, rightdown);
	int rightDown = std::hypot(rightdown.x, rightdown.y);

	Vector2 lefttop = pos1;
	lefttop.x -= size1.x / 2;
	lefttop.y -= size1.y / 2;
	lefttop = V2Sub(pos2, lefttop);
	int leftTop = std::hypot(lefttop.x, lefttop.y);

	Vector2 leftdown = pos1;
	leftdown.x -= size1.x / 2;
	leftdown.y += size1.y / 2;
	leftdown = V2Sub(pos2, leftdown);
	int leftDown = std::hypot(leftdown.x, leftdown.y);

	//-----------------------------------------------------------

	auto diffX = (pos1.x - pos2.x);/////�I�_����n�_������///////
	auto diffY = (pos1.y - pos2.y);/////�I�_����n�_������///////

	if (rightTop <= radius2 ||
		rightDown <= radius2 ||
		leftTop <= radius2 ||
		leftDown <= radius2 ||
		(fabsf(diffX) < radius2 && fabsf(diffY) < radius2 + size1.y / 2) ||
		(fabsf(diffX) < radius2 + size1.x / 2 && fabsf(diffY) < size1.y / 2))
	{
		//�Փ˂��Ă���
		return true;
	}
	
	return false;
}

Vector2 LevelupSelect::V2Sub(Vector2 pos1, Vector2 pos2)
{
	Vector2 pos;
	pos.x = pos1.x - pos2.x;
	pos.y = pos1.y - pos2.y;
	return pos;
}