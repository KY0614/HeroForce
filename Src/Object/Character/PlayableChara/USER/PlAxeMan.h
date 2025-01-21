#pragma once

#include"../../../JobManagerBase.h"
class AxeMan;
class PlAxe :public JobManagerBase
{
public:
	PlAxe(const SceneManager::CNTL _cntl);
	PlAxe(const InputManager::JOYPAD_NO _padNum);
	~PlAxe(void) = default;


	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	
};
