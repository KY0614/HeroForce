#pragma once
#include "SceneBase.h"
class GameOverScene :
    public SceneBase
{
public:
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
private:
	//�^�C�g���֖߂�摜
	int toTitleImg_;
};

