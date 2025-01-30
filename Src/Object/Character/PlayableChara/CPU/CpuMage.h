#pragma once
#include "../../../JobManagerBase.h"
class CpuMage :
    public JobManagerBase
{
public:
	CpuMage(void);
	~CpuMage(void);
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
};

