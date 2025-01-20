#include "PlayerInput.h"
#include"../../Utility/AsoUtility.h"

PlayerInput* playerInput_ = nullptr;
void PlayerInput::CreateInstance(void)
{
	if (playerInput_ == nullptr)
	{
		playerInput_ = new PlayerInput();
	}
}

PlayerInput& PlayerInput::GetInstance(void)
{
	return *playerInput_;
}

void PlayerInput::Update(InputManager::JOYPAD_NO _padNum)
{
	auto& ins = InputManager::GetInstance();
	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(_padNum).AKeyLX;
	//縦軸
	leftStickY_ = ins.GetJPadInputState(_padNum).AKeyLY;

	//方向決め
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
	actCntl_ = ACT_CNTL::NONE;
	if (ins.IsNew(MOVE_FRONT_KEY) || ins.IsNew(MOVE_BACK_KEY) || ins.IsNew(MOVE_LEFT_KEY) || ins.IsNew(MOVE_RIGHT_KEY)) { actCntl_ = ACT_CNTL::MOVE; }
	if (leftStickX_>0||leftStickY_>0) { actCntl_ = ACT_CNTL::MOVE; }
	if (ins.IsTrgDown(ATK_KEY) || ins.IsPadBtnTrgDown(_padNum, ATK_BTN)) { actCntl_ = ACT_CNTL::NMLATK; }
	if (ins.IsTrgDown(SKILL_KEY) || ins.IsPadBtnTrgDown(_padNum, SKILL_BTN)) { actCntl_ = ACT_CNTL::SKILL_DOWN; }
	else if (ins.IsNew(SKILL_KEY) || ins.IsPadBtnNew(_padNum, SKILL_BTN)) { actCntl_ = ACT_CNTL::SKILL_KEEP; }
	if (ins.IsTrgUp(SKILL_KEY) || ins.IsPadBtnTrgUp(_padNum, SKILL_BTN)) { actCntl_ = ACT_CNTL::SKILL_UP; }
	if (ins.IsTrgDown(DODGE_KEY) || ins.IsPadBtnTrgDown(_padNum, DODGE_BTN)) { actCntl_ = ACT_CNTL::DODGE; }
	if (ins.IsTrgDown(SKILL_CHANGE_KEY) || ins.IsPadBtnTrgDown(_padNum, SKILL_CHANGE_BTN)) { actCntl_ = ACT_CNTL::CHANGE_SKILL; }
}

PlayerInput::PlayerInput(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
}