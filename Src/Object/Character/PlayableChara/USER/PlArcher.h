#pragma once
#include"../Manager//\Generic/SceneManager.h"
#include"../Manager//\Generic/InputManager.h"

#include"../../../JobManagerBase.h"

class PlArcher :public JobManagerBase
{
public:
	//定数
	static constexpr float SKILL_ONE_START_NOCHARGE = 0.2;


	PlArcher(const SceneManager::CNTL _cntl);
	PlArcher(const InputManager::JOYPAD_NO _padNum);
	~PlArcher(void) = default;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	//攻撃入力
	void AtkInput(void)override;

	//スキル1入力
	void SkillOneInput(void)override;

	//スキル2入力
	void SkillTwoInput(void)override;

	//通常攻撃初期化
	void AtkInit(void);

	//スキル1初期化
	void SkillOneInit(void);

	//スキル2初期化
	void SkillTwoInit(void);
};
