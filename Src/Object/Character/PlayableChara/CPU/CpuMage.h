#pragma once
#include "../../../JobManagerBase.h"
class CpuMage :
    public JobManagerBase
{
public:

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
};

