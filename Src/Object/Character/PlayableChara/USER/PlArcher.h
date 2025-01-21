#pragma once
#include"../Manager//\Generic/SceneManager.h"
#include"../Manager//\Generic/InputManager.h"

#include"../../../JobManagerBase.h"

class PlArcher :public JobManagerBase
{
public:
	PlArcher(const SceneManager::CNTL _cntl);
	PlArcher(const InputManager::JOYPAD_NO _padNum);
	~PlArcher(void) = default;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
};
