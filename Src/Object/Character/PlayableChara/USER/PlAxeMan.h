#pragma once

#include"../../../JobManagerBase.h"
class AxeMan;
class PlAxe :public JobManagerBase
{
public:
	//スキル１の長押ししていなかった時の攻撃発生
	static constexpr float SKILL_ONE_START_NOCHARGE = 2.0f;


	PlAxe(const SceneManager::CNTL _cntl);
	PlAxe(const InputManager::JOYPAD_NO _padNum);
	~PlAxe(void) = default;


	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	void InitSkill(PlayerBase::ATK_ACT _act);

	//攻撃入力
	void AtkInput(void)override;

	//スキル1入力
	void SkillOneInput(void)override;

	//スキル2入力
	void SkillTwoInput(void)override;
};
