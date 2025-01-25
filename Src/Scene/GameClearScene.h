#pragma once
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Decoration/EffectManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Object/Character/PlayableChara/Other/ClearPlayers.h"
#include "SceneBase.h"

class SkyDome;
class StageManager;
class ClearChicken;
class ClearPlayers;
class GameClearScene : public SceneBase
{
public:

	//�`�L��������
	static constexpr int CHICKEN_CREATES = 4;

	//���b�Z�[�W�ʒu
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	static constexpr VECTOR CHICKEN_POS_1 = { -200.0f, -50.0f, 20.0f };
	static constexpr VECTOR CHICKEN_POS_2 = { -120.0f, -50.0f, 30.0f };
	static constexpr VECTOR CHICKEN_POS_3 = { 150.0f, -50.0f, 40.0f };
	static constexpr VECTOR CHICKEN_POS_4 = { 230.0f, -50.0f, 40.0f };

	//�G�t�F�N�g�Đ��ʒu
	static constexpr VECTOR EFFECT_POS = { 0.0f, -100.0f, 100.0f };		

	//�G�t�F�N�g�T�C�Y
	static constexpr float EFFECT_SIZE = 20.0f;

	GameClearScene();
	~GameClearScene() = default;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�摜
	int imgMes_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> sky_;
	std::unique_ptr<ClearChicken> chickens_[CHICKEN_CREATES];
	std::unique_ptr<ClearPlayers> player_;
	std::unique_ptr<StageManager> stage_;

	void DebagPlay();

};