#pragma once
#include "SceneBase.h"
#include "../Object/Character/PlayableChara/Other/OverPlayers.h"

class OverPlayers;

class GameOverScene : public SceneBase
{
public:

	//���
	enum class STATE
	{
		DOWN,		//�v���C���[���|���
		DISPLAY,	//���b�Z�[�W�̕\��
	};

	GameOverScene();
	~GameOverScene() = default;

	//���b�Z�[�W�ʒu
	static constexpr int MES_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	static constexpr int MES2_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES2_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 +100;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	//�A���t�@�l�ő�
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//��ԑJ�ڕb��
	static constexpr float CHANGE_SECOND = 2.0f;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//���
	STATE state_;

	//�摜
	int imgGameOver_;
	int imgBack_;
	int imgMes_;

	//�X�V�p
	float step_;

	//�A���t�@�l
	float alpha_;
	float alphaAdd_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// ��ԊǗ�
	std::function<void(void)> stateUpdate_;	//�X�V

	//��ԕύX
	void ChangeState(const STATE state);
	void ChangeStateDown();
	void ChangeStateDisplay();

	//�e��X�V����
	void UpdateDown(void);
	void UpdateDisplay(void);
	
	//�v���C���[
	std::unique_ptr<OverPlayers> player_;
};

