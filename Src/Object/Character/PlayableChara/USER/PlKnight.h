#pragma once

#include"../../../JobManagerBase.h"
class Knight;
class PlKnight :public JobManagerBase
{
public:
	//スキル2の防御アニメーションの止めフレーム
	static constexpr float GUARD_ANIM_STEP = 10.0f;
	PlKnight(const SceneManager::CNTL _cntl);
	PlKnight(const InputManager::JOYPAD_NO _padNum);
	~PlKnight(void) = default;

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

	//スキル初期化
	void SkillOneInit(void);
	void SkillTwoInit(void);
};