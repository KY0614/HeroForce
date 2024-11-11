#pragma once


class InputController
{
public:



	enum class KeyAction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE,
		CANCEL,
		ATK,
		
		MAX_ACTION
	};

	InputController(void);

	~InputController(void);

	void Init(void);
	void Update(void);
	void Release(void);

	void SetInput(void);

private:
};

