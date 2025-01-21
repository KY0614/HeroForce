#pragma once

#include"../../../JobManagerBase.h"
class Knight;
class PlKnight :public JobManagerBase
{
public:
	PlKnight(const SceneManager::CNTL _cntl);
	PlKnight(const InputManager::JOYPAD_NO _padNum);
	~PlKnight(void) = default;

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
};