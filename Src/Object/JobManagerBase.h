#pragma once

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
};

