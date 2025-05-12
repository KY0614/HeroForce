#pragma once
#include <memory>
#include <functional>
#include <vector>
#include "../Character/PlayableChara/PlayerBase.h"
#include "../../Common/Fader.h"

class LevelSelect;
class LevelNotice;
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
		MAX
	};

	//初期レベル
	static constexpr int DEFAULT_LEVEL = 1;

	//経験値獲得速度
	static constexpr float EXP_SPEED = 5.0f;
	
	//ゲージ最大値
	static constexpr float CONSTANT_GAGE = 1000.f;

	//ゲージUI拡大率
	static constexpr float GAGE_SCALE_RATE = 1.0f;
	static constexpr float NUM_SCALE_RATE = 0.75f;

	//ゲージ画像サイズ
	static constexpr int GAGE_IMG_SIZE = static_cast<int>(128 * GAGE_SCALE_RATE);

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

	//レベルアップ時のパラメーター追加量
	static constexpr float ADD_ATK_PER = 30.0f;
	static constexpr float ADD_DEF_PER = 30.0f;
	static constexpr float ADD_SPEED_PER = 30.0f;
	static constexpr int ADD_LIFE = 30;

	//デバッグ用経験値
	static constexpr int DEBAG_EXP = 1000;

	//右のローカル座標X
	static constexpr int RIGHT_LOCAL_POS_X = static_cast<int>(64 / 1.8f);

	//左のローカル座標X
	static constexpr int LEFT_LOCAL_POS_X = static_cast<int>(32 * 1.2f);

	//レベル計算用
	static constexpr int LEVEL_DIV = 10;

	//コンストラクタ
	LevelScreenManager();

	//デストラクタ
	~LevelScreenManager();

	void Init();
	void Update();
	void Draw();
	void Release();

	//読み込み
	void Load();

	//初期化処理
	void Reset();

	/// <summary>
	/// 経験値を加える
	/// </summary>
	/// <param name="_value"></param>追加経験値量
	inline void AddExp(const float _value) { restExp_ += _value; }

	/// <summary>
	/// 経験値ゲージの設定
	/// </summary>
	/// <param name="_level"></param>レベル
	void SetGage(const int _level);

	/// <summary>
	/// レベルアップ効果反映
	/// </summary>
	/// <param name="_player"></param>プレイヤー
	/// <param name="_playerNum"></param>プレイヤー番号
	void Reflection(PlayerBase &_player,const int _playerNum);

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>状態
	void ChangeState(const STATE _state);
	
	/// <summary>
	/// エフェクトを追従させる
	/// </summary>
	/// <param name="_player"></param>プレイヤー
	/// <param name="_playerNum"></param>プレイヤー番号
	void EffectSyne(PlayerBase& _player, const int _playerNum);

	/// <summary>
	/// 経験値量を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>経験値
	inline const float GetExp()const { return exp_; };

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const STATE GetState()const { return state_; };

	/// <summary>
	/// 種類を返す
	/// </summary>
	/// <param name="_playerNum"></param>プレイヤー番号
	/// <returns></returns>種類を返す
	inline TYPE GetType(const int _playerNum)const { return selectTypes_[_playerNum]; }

	/// <summary>
	/// 前の種類を返す
	/// </summary>
	/// <param name="_playerNum"></param>プレイヤー番号
	/// <returns></returns>前の種類
	inline const TYPE GetPreType(const int _playerNum)const { return preTypeData_[_playerNum]; }

	/// <summary>
	/// レベルアップ判定
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>レベルアップ処理を行う場合は,true,そうではない場合は,false
	inline const bool IsLevelUp()const  { return !(state_ == STATE::NONE || state_ == STATE::END); }

private:	
	
	//通知
	std::unique_ptr<LevelNotice> notice_;

	//選択
	std::unique_ptr<LevelSelect> select_;

	//フェーダー
	std::unique_ptr<Fader> fader_;

	//画像
	int imgGage_;		//ゲージ
	int imgGageExp_;	//経験値ゲージ
	int *imgNumbers_;	//数字

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
	std::map<STATE, std::function<void()>> stateChanges_;

	// 状態管理
	std::function<void()> stateUpdate_;	//更新
	std::function<void()> stateDraw_;	//描画



	//状態変更
	void ChangeStateNone();
	void ChangeStateNotice();
	void ChangeStateSelect();
	void ChangeStateEnd();

	//各種更新処理
	void UpdateNone();
	void UpdateNotice();		//通知
	void UpdateSelect();		//強化選択
	void UpdateEnd();			//終了

	//各種描画処理
	void DrawNone();
	void DrawNotice();
	void DrawSelect();
	void DrawEnd();	
	
	//経験値の状態確認
	void CheckExp();
						
	//UI描画
	void DrawLevelUI();

	//暗転
	void FaderDraw();

	//フェードの切り替え処理
	void Fade();

	//スキップ状態
	void SkipState(const STATE& _nextState);

	//デバッグ機能
	void DebagUpdate();
	void DebagDraw();

};