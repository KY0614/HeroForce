#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"

class SkyDome
{
public:

	static constexpr float ROT_SPEED = 0.05f;

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// コンストラクタ
	SkyDome();

	// デストラクタ
	~SkyDome(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

	//自身のトランスフォームを返す
	const Transform& GetTransform() const;

private:	
	
	// 状態
	STATE state_;

	// モデル制御の基本情報
	Transform transform_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
};

