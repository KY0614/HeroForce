#pragma once
#include"../Manager/GameSystem/DataBank.h"
#include"Character/PlayableChara/PlayerBase.h"
class PlayerBase;

class JobManagerBase
{
public:

	JobManagerBase(void);
	~JobManagerBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	PlayerBase* GetPlayer(void) { return obj_; }

protected:

	PlayerBase* obj_;	//使用するインスタンス格納
	DataBank::PLAYER_INFO info_;
	InputManager::JOYPAD_NO padNum_;		//パッド番号
	virtual void AtkInput(void)=0;
	virtual void SkillOneInput(void)=0;
	virtual void SkillTwoInput(void)=0;
};

