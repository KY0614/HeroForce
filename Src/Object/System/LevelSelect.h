<<<<<<< HEAD:Src/Object/System/LevelupSelect.h
#pragma once
#include <vector>
#include <string>
#include<memory>
#include "../../Common/Vector2.h"
#include "LevelScreenManager.h"

class Carsor;
class LevelScreenManager;

class LevelupSelect
{
public:

	//���
	enum class STATE
	{
		NONE,
		EXPANSION,
		SELECT,
		FIN, 
	};

	//�������ډ摜�T�C�Y
	static constexpr int ELE_IMG_SIZE_X = 340;
	static constexpr int ELE_IMG_SIZE_Y = 250;

	//�I�����ڐ�
	static constexpr int SELECT_ELEMENT = 4;

	//�g�嗦
	static constexpr float SCALE_RATE = 0.01f;

	//�g��ő�
	static constexpr float SCALE_MAX = 0.7f;//Application::SCREEN_SIZE_X * 1.5f / Application::DEFA_SCREEN_SIZE_X;

	//�t�H���g�֘A
	static constexpr int FONT_MES_SIZE = 36;//Application::SCREEN_SIZE_X * 72 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_MES_THICK = 5;
	static constexpr int FONT_EXP_SIZE = 24;//Application::SCREEN_SIZE_X * 36 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_EXP_THICK = 3;

	//���b�Z�[�W�`��ʒu
	static constexpr int MES_TEXT_POS_X = Application::SCREEN_SIZE_X / 2 + 30;
	static constexpr int MES_TEXT_POS_Y = 48;

	//�����v�f�ʒu
	static constexpr int INTERVEL = 100;
	static constexpr int ELEMENT_POS_LU_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_RU_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_LD_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	static constexpr int ELEMENT_POS_RD_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	LevelupSelect();
	~LevelupSelect();

	void Init();
	void Update();
	void Draw();
	void Release();

	//�ϐ��̏�����
	void Reset();

	//�ǂݍ���
	void Load();

	//��Ԃ̎󂯎��
	STATE GetState() const { return state_; }

	//��ނ̎󂯎��
	inline LevelScreenManager::TYPE GetType(const int plNum) { return selectTypes_[plNum]; }
	
	//��ނ̃����_������
	inline LevelScreenManager::TYPE GetRandType();

private:

	//�f�o�b�O�p�v���C���[�l��
	int plNum_;

	//�摜
	int img_;
	int imgSelects_[LevelScreenManager::TYPE_MAX];
	int *imgCarsors_;
	int imgBack_;

	//�����v�f��
	int element_;

	//�t�H���g
	int fontMes_;
	int fontExp_;

	//���荀��
	std::vector<LevelScreenManager::TYPE> selectTypes_;

	//��������
	struct Element
	{
		//�g�嗦
		float scl_;

		//���W
		Vector2 pos_;

		//�T�C�Y
		Vector2 size_;

		//���
		LevelScreenManager::TYPE type_;

		//�e�L�X�g�̕\��
		bool isText_;
	};

	//�G�������g
	std::vector<Element> ele_;

	//���
	STATE state_;

	//�e�L�X�g
	std::string mesText_;
	std::string expTexts_[LevelScreenManager::TYPE_MAX];

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// ��ԊǗ�
	std::function<void(void)> stateUpdate_;	//�X�V

	//�J�[�\��
	std::vector<std::unique_ptr<Carsor>> carsors_;

	//�������W�̐ݒ�
	void SetFirstPos();

	//�t�H���g����
	void CreateFonts();

	//�Փ˔���
	bool IsCollisionBoxCircle(Vector2 pos1, Vector2 size1, Vector2 pos2, float radius2);	//�v�Z����
	Vector2 V2Sub(Vector2 value1, Vector2 value2);

	//��ԕύX����
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStateExpansion();
	void ChangeStateSelect();
	void ChangeStateFin();

	//��ԕʍX�V����
	void UpdateNone();
	void UpdateExpansion();
	void UpdateSelect();
	void UpdateFin();

	void DebagDraw();

};
=======
#pragma once
#include <vector>
#include <string>
#include<memory>
#include "../../Common/Vector2.h"
#include "Cursor.h"

class Cursor;

class LevelSelect
{
public:

	//���
	enum class STATE
	{
		NONE,
		EXPANSION,
		SELECT,
		FIN, 
	};

	//�������ډ摜�T�C�Y
	static constexpr int ELE_IMG_SIZE_X = 340;
	static constexpr int ELE_IMG_SIZE_Y = 250;

	//�I�����ڐ�
	static constexpr int SELECT_ELEMENT = 4;

	//�g�嗦
	static constexpr float SCALE_RATE = 0.01f;

	//�g��ő�
	static constexpr float SCALE_MAX = 0.7f;

	//�t�H���g�֘A
	static constexpr int FONT_MES_SIZE = 36;
	static constexpr int FONT_MES_THICK = 5;
	static constexpr int FONT_EXP_SIZE = 24;
	static constexpr int FONT_EXP_THICK = 3;

	//���b�Z�[�W�`��ʒu
	static constexpr int MES_TEXT_POS_X = Application::SCREEN_SIZE_X / 2 + 30;
	static constexpr int MES_TEXT_POS_Y = 48;

	//�����v�f�ʒu
	static constexpr int INTERVEL = 100;
	static constexpr int ELEMENT_POS_LU_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_RU_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_LD_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	static constexpr int ELEMENT_POS_RD_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	//�w�i�A���t�@�l
	static constexpr int BACK_ALPHA = 150;

	//�p�[�Z���g�p����
	static constexpr int POW = 30;

	//�e�L�X�g�ʒu�����p
	static constexpr int TEXT_OFFSET_POS_Y = 90;

	//�R���X�g���N�^
	LevelSelect();

	//�f�X�g���N�^
	~LevelSelect() = default;

	//������
	void Init();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//���
	void Release();

	//�ϐ��̏�����
	void Reset();

	//�ǂݍ���
	void Load();

	/// <summary>
	/// ��Ԃ�Ԃ�
	/// </summary>
	/// <returns></returns>���
	inline const STATE GetState() const { return state_; }

	/// <summary>
	/// ��ނ�Ԃ�
	/// </summary>
	/// <param name="plNum"></param>�v���C���[�ԍ�
	/// <returns></returns>�I���������
	inline LevelScreenManager::TYPE GetType(const int _plNum) { return selectTypes_[_plNum]; }
	
	//�X�L�b�v�p����
	void SetSkipState();

private:

	//�f�o�b�O�p�v���C���[�l��
	int plNum_;

	//�摜
	int img_;
	int imgSelects_[LevelScreenManager::TYPE_MAX];
	int *imgCursors_;
	int imgBack_;

	//�����v�f��
	int element_;

	//�t�H���g
	int fontMes_;
	int fontExp_;

	//���荀��
	std::vector<LevelScreenManager::TYPE> selectTypes_;

	//��������
	struct Element
	{
		//�g�嗦
		float scl_ = 0.0f;

		//���W
		Vector2 pos_ = {};

		//�T�C�Y
		Vector2 size_ = {};

		//���
		LevelScreenManager::TYPE type_ = LevelScreenManager::TYPE::MAX;

		//�e�L�X�g�̕\��
		bool isText_ = false;
	};

	//�G�������g
	std::vector<Element> ele_;

	//���
	STATE state_;

	//�e�L�X�g
	std::string mesText_;
	std::string expTexts_[LevelScreenManager::TYPE_MAX];

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	// ��ԊǗ�
	std::function<void()> stateUpdate_;	//�X�V

	//�J�[�\��
	std::vector<std::unique_ptr<Cursor>> cursors_;

	//�������W�̐ݒ�
	void SetFirstPos();

	//�t�H���g����
	void CreateFonts();	
	
	//��ނ̃����_������
	LevelScreenManager::TYPE GetRandType();

	//��ԕύX����
	void ChangeState(const STATE _state);
	void ChangeStateNone();
	void ChangeStateExpansion();
	void ChangeStateSelect();
	void ChangeStateFin();

	//��ԕʍX�V����
	void UpdateNone();
	void UpdateExpansion();
	void UpdateSelect();
	void UpdateFin();

	void DebagDraw();

};
>>>>>>> Data2:Src/Object/System/LevelSelect.h

