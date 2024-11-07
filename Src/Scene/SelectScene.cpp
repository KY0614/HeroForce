#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{

}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//人数選択から
	ChangeSelect(SELECT::NUMBER);

	KeyKonfigSetting();

	pos_ = { 0.0f,0.0f,0.0f };

	key = KEY_CONFIG::NONE;
}

void SelectScene::Update(void)
{
	KeyKonfigSetting();

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//選択中の種類ごとの更新処理
	switch (select_)
	{
	case SELECT::NUMBER:
		NumberUpdate();
		break;

	case SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();
	InputManager::JOYPAD_IN_STATE pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	Vector2 mPos = ins.GetMousePos();

	DrawString(0, 0,"select",0xFFFFFF);
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key);

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:
		DrawString(Application::SCREEN_SIZE_X/2, 0, "number", 0xFFFFFF);
		break;
	case SelectScene::SELECT::ROLE:
		DrawString(Application::SCREEN_SIZE_X / 2, 0, "role", 0xFFFFFF);
		break;
	default:
		break;
	}

	DrawCircle(mPos.x, mPos.y, 20, 0xFF0000, true);
	DrawCircle(pos_.x, pos_.y, 20, 0xFF00FF, true);

}

void SelectScene::Release(void)
{
}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsClickMouseLeft())
	{
		ChangeSelect(SELECT::ROLE);
	}

	switch (key)
	{
	case SelectScene::KEY_CONFIG::NONE:
		break;
	
	case SelectScene::KEY_CONFIG::UP:
		Move({ 0.0f,-10.0f,0.0f });
		break;
	
	case SelectScene::KEY_CONFIG::DOWN:
		Move({ 0.0f,10.0f,0.0f });
		break;
	
	case SelectScene::KEY_CONFIG::LEFT:
		break;
	
	case SelectScene::KEY_CONFIG::RIGHT:
		break;
	
	case SelectScene::KEY_CONFIG::DECIDE:
		break;
	
	default:
		break;
	}
}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsClickMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void SelectScene::DrawDebug(void)
{
	
}

void SelectScene::ChangeSelect(SELECT select)
{
	select_ = select;
}

void SelectScene::ProcessSelect(void)
{
}

void SelectScene::KeyKonfigSetting(void)
{
	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0 && CheckHitKeyAll() == 0)
	{
		key = KEY_CONFIG::NONE;
	}



	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	if (ins.IsNew(KEY_INPUT_UP) ||
		leftStickY_ < -1)
	{
		key = KEY_CONFIG::UP;
	}

	if (ins.IsNew(KEY_INPUT_DOWN) ||
		leftStickY_ > 1)
	{
		key = KEY_CONFIG::DOWN;
	}

	if (ins.IsNew(KEY_INPUT_LEFT) ||
		leftStickX_ < -1)
	{
		key = KEY_CONFIG::LEFT;
	}

	if (ins.IsNew(KEY_INPUT_RIGHT) ||
		leftStickX_ > 1)
	{
		key = KEY_CONFIG::RIGHT;
	}
}

void SelectScene::Move(VECTOR pos)
{
	pos_.x += pos.x;
	pos_.y += pos.y;
	pos_.z += pos.z;
	//speedMove_ = SPEED_MOVE;
	//if (!IsDodge() && !IsAtkAction() && !IsSkill(SKILL::ONE))
	//{
	//	ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	//	Turn(_deg, _axis);
	//	VECTOR dir = trans_.GetForward();
	//	//移動方向
	//	VECTOR movePow = VScale(dir, speedMove_);
	//	//移動処理
	//	trans_.pos = VAdd(trans_.pos, movePow);
	//}
}