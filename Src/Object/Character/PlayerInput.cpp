#include "PlayerInput.h"
#include"PlayableChara/PlayerBase.h"
#include"../../Utility/AsoUtility.h"

PlayerInput* playerInput_ = nullptr;
void PlayerInput::CreateInstance(void)
{
	if (playerInput_ == nullptr)
	{
		playerInput_ = new PlayerInput();
	}
	//actCntl_ = SceneManager::CNTL::NONE;
}

PlayerInput& PlayerInput::GetInstance(void)
{
	return *playerInput_;
}

void PlayerInput::Update(PlayerBase* _player, InputManager::JOYPAD_NO _padNum,SceneManager::CNTL _cntl)
{
	switch (_cntl)
	{
	case SceneManager::CNTL::NONE:
		break;
	case SceneManager::CNTL::KEYBOARD:
		InputKeyBoard(_player);
		break;
	case SceneManager::CNTL::PAD:
		InputPad(_player, _padNum);
		break;
	default:
		break;
	}
}

void PlayerInput::InputKeyBoard(PlayerBase* _player)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = PlayerBase::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//移動(いずれかの移動キーを押していたら)
	if (ins.IsNew(MOVE_FRONT_KEY) || ins.IsNew(MOVE_BACK_KEY) || ins.IsNew(MOVE_LEFT_KEY) || ins.IsNew(MOVE_RIGHT_KEY)) { actCntl_ = ACT_CNTL::MOVE; }

	//移動角度を決める
	if (ins.IsNew(PlayerInput::MOVE_FRONT_KEY)) { moveDeg_ = 0.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_LEFT_KEY)) { moveDeg_ = 270.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_BACK_KEY)) { moveDeg_ = 180.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_RIGHT_KEY)) { moveDeg_ = 90.0f; }

	//通常攻撃
	if (ins.IsTrgDown(ATK_KEY) && !_player->GetIsCool(ATK_ACT::ATK) && !_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::NMLATK; }

	//スキル短押し
	if (ins.IsTrgDown(SKILL_KEY) && !_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_DOWN; }

	//長押し
	else if (ins.IsNew(SKILL_KEY)&& _player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_KEEP; }

	//ボタン離されたら
	if (ins.IsTrgUp(SKILL_KEY)&& _player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_UP; }

	//回避
	if (ins.IsTrgDown(DODGE_KEY)&&!_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::DODGE; }

	//スキル変更キー
	if (ins.IsTrgDown(SKILL_CHANGE_KEY)&&!_player->GetIsSkill()) { actCntl_ = ACT_CNTL::CHANGE_SKILL; }
}

void PlayerInput::InputPad(PlayerBase* _player, InputManager::JOYPAD_NO _padNum)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = PlayerBase::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(_padNum).AKeyLX;
	//縦軸
	leftStickY_ = ins.GetJPadInputState(_padNum).AKeyLY;
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));


	if (leftStickX_ != 0.0f || leftStickY_ != 0.0f)
	{ 
		actCntl_ = ACT_CNTL::MOVE; }

	//スティックの角度によって移動方向を決める
	moveDeg_ = stickDeg_;


	if (ins.IsPadBtnTrgDown(_padNum, ATK_BTN) && !_player->GetIsCool(ATK_ACT::ATK) &&!_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::NMLATK; }
	if(ins.IsPadBtnTrgDown(_padNum, SKILL_BTN) && !_player->GetIsAtk() && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::SKILL_DOWN; }
	else if(ins.IsPadBtnNew(_padNum, SKILL_BTN) && _player->GetIsSkill()&& !_player->GetIsAtk()){ actCntl_ = ACT_CNTL::SKILL_KEEP; }
	if( ins.IsPadBtnTrgUp(_padNum, SKILL_BTN) && _player->GetIsSkill()&&!_player->GetIsAtk()){ actCntl_ = ACT_CNTL::SKILL_UP; }
	if(ins.IsPadBtnTrgDown(_padNum, DODGE_BTN) && !_player->GetIsAtk() && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::DODGE; }
	if(ins.IsPadBtnTrgDown(_padNum, SKILL_CHANGE_BTN) && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::CHANGE_SKILL; }
}

PlayerInput::PlayerInput(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
}