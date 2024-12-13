#pragma once


class InputController
{
public:



	enum class KEY_ACTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE,
		CANCEL,
		SKILL_ONE,
		SKILL_TWO,
		
		MAX_ACTION
	};

	InputController(void);

	~InputController(void);

	void Init(void);
	void Update(void);
	void Release(void);



	void SetInput(void);

private:

	KEY_ACTION keyAction_;
};

