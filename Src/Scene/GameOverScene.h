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
	//ƒ^ƒCƒgƒ‹‚Ö–ß‚é‰æ‘œ
	int toTitleImg_;
};

