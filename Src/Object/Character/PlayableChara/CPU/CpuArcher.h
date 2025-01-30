#pragma once
#include "../Object/JobManagerBase.h"
class CpuArcher :
    public JobManagerBase
{
public:
	CpuArcher(void);
	~CpuArcher(void);
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

