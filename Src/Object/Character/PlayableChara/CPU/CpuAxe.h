#pragma once
#include "../../../JobManagerBase.h"
class CpuAxe :
    public JobManagerBase
{
public:
	CpuAxe(void);
	~CpuAxe(void);
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

