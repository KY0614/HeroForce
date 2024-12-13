#pragma once
#include <string>
#include <memory>
#include <functional>
#include "../../Common/Fader.h"
#include "LevelScreenManager.h"

class LevelScreenManager;

class LevelupNotice
{
public:

	//�摜�Ɋւ���`��̏��
	enum class STATE
	{
		NONE,
		FADE_IN,	//�t�F�[�h�C��
		MAINTAIN,	//�ێ�
		FIN,		//�I��
	};

	//�g�嗦
	static constexpr float EXPANSION_RATE = 3.0f;

	//�g��ő�
	static constexpr float EXPANSION_MAX = 1.2f;

	//�ێ��b��
	static constexpr int MAINTAIN_SECONDS = 1.5 * Application::DEFAULT_FPS;

	//�t�H���g�֘A
	static constexpr int FONT_TEXT_SIZE = 128;
	static constexpr int FONT_LEVEL_SIZE = 156;
	static constexpr int FONT_THICK = 3;

	//���ߊJ�n����(���x���e�L�X�g)
	static constexpr int START_ALPHA_LEVEL = 2 * Application::DEFAULT_FPS;
	
	//UI�ʒu
	static constexpr int MES_TEXT_POS_X = FONT_TEXT_SIZE /2;
	static constexpr int MES_TEXT_POS_Y = 100;
	static constexpr int LEVEL_TEXT_POS_X = 8;
	static constexpr int LEVEL_TEXT_POS_Y = 70;


	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();
	void Release();
	
	//�ǂݍ���
	void Load();

	//�ϐ��̏�����
	void Reset();

	//��Ԃ̐ݒ�
	void ChangeState(const STATE state);

	//���x���̐ݒ�
	void SetNewLevel(const int& newLevel);

	//��Ԃ�Ԃ�
	inline STATE GetState() const { return state_; }

private:

	//���
	STATE state_;

	//�摜
	int img_;

	//�J�E���g
	int cnt_;

	//���x��
	int newLevel_;

	//�傫��
	float scl_;
	
	//�A���t�@�l
	float alphaMes_;
	float alphaLevel_;

	//�t�H���g
	int fontMes_;
	int fontLevel_;

	//�e�L�X�g
	std::string text_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// ��ԊǗ�
	std::function<void(void)> stateUpdate_;	//�X�V

	//��ԕύX����
	void ChangeStateNone();
	void ChangeStateFade();
	void ChangeStateMaintain();
	void ChangeStateFin();

	//�X�V����
	void UpdateNone();
	void UpdateFade();
	void UpdateMaintain();
	void UpdateFin();

	//�`��
	void DrawMessage();
	void DrawLevel();

};

