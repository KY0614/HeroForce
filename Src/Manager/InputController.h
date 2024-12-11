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

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputController& GetInstance(void);

	void Init(void);
	void Update(void);

	// リソースの破棄
	void Destroy(void);

	void SetInput(void);

private:

	// シングルトン用インスタンス
	static InputController* instance_;

	KEY_ACTION keyAction_;
};

