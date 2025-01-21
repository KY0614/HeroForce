#include "../Manager/Generic/InputManager.h"
#include "InputController.h"

InputController* InputController::instance_ = nullptr;

void InputController::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputController();
	}
	instance_->Init();
}

InputController& InputController::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputController::CreateInstance();
	}
	return *instance_;
}

void InputController::Init(void)
{
}

void InputController::Update(void)
{
}

void InputController::Destroy(void)
{
	delete instance_;
}

void InputController::SetInput(void)
{
	InputManager& ins = InputManager::GetInstance();

	
}
