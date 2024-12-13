#pragma once
#include <functional>
#include <vector>
class LevelupNotice;
class LevelupSelect;

class LevelScreenManager
{
public:

	//状態管理
	enum class STATE
	{
		NONE,		//なし
		NOTICE,		//通知
		SELECT,		//選択
		END			//終了
	};	

	//強化要素の種類
	enum class TYPE
	{
		ATTACK,		//攻撃力の上昇
		DEFENSE,	//防御力の上昇
		SPEED,		//移動速度の上昇
		LIFE,		//最大体力の上昇
	  /*TRAP_FLOOR,
		TRAP_CANNON,
		TRAP_POW_UP_F,
		TRAP_POW_UP_C,*/
		MAX
	};

	//初期レベル
	static constexpr int DEFAULT_LEVEL = 1;
	
	//ゲージ最大値
	static constexpr float CONSTANT_GAGE = 100.0f;

	//アルファ値最大
	static constexpr float ALPHA_MAX = 150.0f;

	//透過スピード
	static constexpr float ALPHA_SPEED = 3.0f;

	//種類最大
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	LevelScreenManager(void);
	~LevelScreenManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//読み込み
	void Load(void);

	//初期化処理
	void Reset();

	//経験値の増加
	void AddExp(const float value);

	//ゲージの設定
	void SetGage(const int level);

	//ステートの設定
	void ChangeState(const STATE state);
	
	//経験値の状態確認
	void CheckExp();

	//ゲッター
	inline float GetExp(void)const { return exp_; };
	inline STATE GetState(void)const { return state_; };
	inline TYPE GetType(const int playerNum)const;

private:

	int playerNum_;

	//状態
	STATE state_;

	//種類
	std::vector<TYPE> selectTypes_;

	//現在のレベル
	int nowLevel_;

	//経験値
	float exp_;

	//ゲージ(次のレベルまでの経験値量)
	float gauge_;

	//アルファ値
	float alpha_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理
	std::function<void(void)> stateUpdate_;	//更新
	std::function<void(void)> stateDraw_;	//描画

	//インスタンス
	std::unique_ptr<LevelupNotice> notice_;
	std::unique_ptr<LevelupSelect> select_;

	//状態変更
	void ChangeStateNone();
	void ChangeStateNotice();
	void ChangeStateSelect();
	void ChangeStateEnd();

	//各種更新処理
	void UpdateNone(void);
	void UpdateNotice(void);		//通知
	void UpdateSelect(void);		//強化選択
	void UpdateEnd(void);			//終了

	//各種描画処理
	void DrawNone();
	void DrawNotice();
	void DrawSelect();
	void DrawEnd();
								
	//暗転
	void FaderDraw();

	//デバッグ機能
	void DebagUpdate();
	void DebagDraw();
};