#pragma once
#include <functional>
#include <vector>
#include "../Character/PlayableChara/PlayerBase.h"
#include "../../Common/Fader.h"

class LevelupNotice;
class LevelupSelect;
class Fader;

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
	static constexpr float CONSTANT_GAGE = 1200.0f;

	//ゲージUI拡大率
	static constexpr float GAGE_SCALE_RATE = Application::SCREEN_SIZE_X * 1.8f / Application::DEFA_SCREEN_SIZE_X;

	//ゲージ画像サイズ
	static constexpr int GAGE_IMG_SIZE = 128 * GAGE_SCALE_RATE;

	//アルファ値最大
	static constexpr float ALPHA_MAX = 150.0f;

	//透過スピード
	static constexpr float ALPHA_SPEED = 3.0f;

	//種類最大
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//エフェクトサイズ
	static constexpr float EFFECT_SCALE = 30.0f;

	//ゲージ座標
	static constexpr int GAGE_POS_X = 20;
	static constexpr int GAGE_POS_Y = 20;

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

	//効果反映
	void Reflection(PlayerBase &player,const int playerNum);

	//ステートの設定
	void ChangeState(const STATE state);
	
	//経験値の状態確認
	void CheckExp();

	void EffectSyne(PlayerBase& player, const int playerNum);

	//ゲッター
	inline float GetExp(void)const { return exp_; };
	inline STATE GetState(void)const { return state_; };
	inline TYPE GetType(const int playerNum)const;
	TYPE GetPreType(const int playerNum)const;
	const bool IsLevelUp(void) { return !(state_ == STATE::NONE || state_ == STATE::END); }

private:

	//画像
	int imgGage_;
	int imgGageExp_;
	int *imgNumbers_;

	//プレイヤー人数
	int playerNum_;

	//状態
	STATE state_;

	//種類
	std::vector<TYPE> selectTypes_;

	//現在のレベル
	int nowLevel_;

	//経験値
	float exp_;
	float restExp_;

	//ゲージ(次のレベルまでの経験値量)
	float gauge_;

	//アルファ値
	float alpha_;

	//フェード処理
	bool isFader_;

	//前状態
	std::vector<TYPE> preTypeData_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理
	std::function<void(void)> stateUpdate_;	//更新
	std::function<void(void)> stateDraw_;	//描画

	//インスタンス
	std::unique_ptr<LevelupNotice> notice_;
	std::unique_ptr<LevelupSelect> select_;
	std::unique_ptr<Fader> fader_;

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
						
	//UI描画
	void DrawLevelUI();

	//暗転
	void FaderDraw();

	//フェードの切り替え処理
	void Fade();

	//デバッグ機能
	void DebagUpdate();
	void DebagDraw();
};