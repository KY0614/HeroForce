#pragma once
#include <functional>
#include <memory>
#include <map>
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Decoration/EffectManager.h"
#include "../../../Manager/Decoration/SoundManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../System/GameUi/CpuHpBar.h"
#include "../../UnitBase.h"

class CpuHpBar;

class ChickenBase : public UnitBase
{
public:	
	
	//状態
	enum class STATE
	{
		NONE,	//なし
		ALIVE,	//生存
		DAMAGE, //ダメージ
		DEATH,	//撃破
		END		//終了
	};

	//生存時の行動状態
	enum class ALIVE_MOVE
	{
		IDLE,		//立ち止まる
		ROTATION,	//その場を逃げ回る
		CALL,		//助けを呼ぶ
		MAX		
	};

	//キャラクターサイズ
	static constexpr VECTOR SCALE = { 1.3f,1.3f, 1.3f };

	//アニメーション速度
	static constexpr float DEFAULT_SPEED_ANIM = 50.0f;
	
	//アニメーション番号
	static constexpr int ANIM_IDLE = 1;
	static constexpr int ANIM_WALK = 10;
	static constexpr int ANIM_DAMAGE = 9;
	static constexpr int ANIM_DEATH =11;
	static constexpr int ANIM_CALL = 6;

	//生存状態時の状態種類
	static constexpr int ALIVE_MOVE_MAX = static_cast<int>(ALIVE_MOVE::MAX);

	//衝突判定用球体半径
	static constexpr float RADIUS = 50.0f;

	//フェード時間
	static constexpr float TIME_FADE = 4.0f;

	//フェード開始・終了色
	static constexpr COLOR_F FADE_C_FROM = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr COLOR_F FADE_C_TO = { 0.8f, 0.1f, 0.1f, 0.0f };

	//HELP画像表示時間
	static constexpr float IS_HELP_CNT = static_cast<float>(3 * Application::DEFAULT_FPS);

	//HELP相対位置
	static constexpr VECTOR LOCAL_HELP_POS = { 0,180,0 };

	//HP描画
	static constexpr VECTOR LOCAL_HP_POS = { 0, 120, 0 };

	//煙ローカル座標
	static constexpr VECTOR LOCAL_SMOKE_POS = { 0,40,0 };

	//煙スピード
	static constexpr float SMOKE_SPEED = 20.0f;

	//煙サイズ
	static constexpr float SMOKE_SCALE = 100.0f;

	//煙アニメーション数
	static constexpr int SMOKE_NUM = ResourceManager::SMOKE_NUM_X * ResourceManager::SMOKE_NUM_Y;

	//ダメージエフェクト
	static constexpr float DAMAGE_EFE_SCALE = 30.0f;

	//エフェクトの生成範囲
	static constexpr VECTOR EFC_CREATE_RANGE = { 50, 50, 0 };

	//煙中心位置
	static constexpr float SMOKE_CENTER_POS = 0.5f;

	//コンストラクタ
	ChickenBase();

	//デストラクタ
	~ChickenBase();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="pos"></param>生成位置
	virtual void Create(const VECTOR& _pos);

	//更新
	virtual void Update()override;

	//描画
	void Draw()override;
	
	//画像表示の設定
	void SetIsHelp();

	/// <summary>
	/// ターゲットの座標を受け取る
	/// </summary>
	/// <param name="pos"></param>ターゲット位置
	inline void SetTarget(const VECTOR _pos) { targetPos_ = _pos; }

	//状態を返す
	inline const STATE GetState() const { return state_; }

protected:

	//画像
	int imgHelp_;
	int* imgSmoke_;

	//移動スピード
	//float moveSpeed_;

	//フェード用ステップ
	float fadeStep_;

	//画像の表示
	bool isHelp_;
	float isHelpCnt_;

	//ターゲット用情報
	VECTOR targetPos_;

	//状態
	STATE state_;

	//生存時の状態
	ALIVE_MOVE aliveState_;

	//煙アニメーション
	int smokeNum_;
	float smokeStep_;
	float efeSpeed_;
	
	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void()>> stateChanges_;

	std::map<ALIVE_MOVE, std::function<void()>> aliveChanges_;

	// 状態管理(更新ステップ)
	std::function<void()> stateUpdate_;

	// 状態管理(描画)
	std::function<void()> stateDraw_;

	// 生存時状態管理
	std::function<void()> stateAliveUpdate_;

	//UIインスタンス生成
	std::unique_ptr<CpuHpBar> hpUi_;

	//モデル設定
	void ModelSet();

	//リソースの読み込み
	void Load();

	//パラメーターの設定
	virtual void SetParam();

	//アニメーション番号の初期化
	virtual void InitAnimNum(void);

	//UI設定
	void SetUiParam();

	//状態変更
	void ChangeState(const STATE _state);
	void ChangeStateNone();
	void ChangeStateAlive();
	void ChangeStateDamage();
	void ChangeStateDeath();
	void ChangeStateEnd();

	void ChangeAliveState(const ALIVE_MOVE _state);
	void ChangeAliveIdle();
	void ChangeAliveWalk();
	void ChangeAliveCall();

	//状態別更新
	void UpdateNone();
	void UpdateAlive();
	void UpdateDamage();
	void UpdateDeath();
	void UpdateEnd();

	//状態別描画
	void DrawNone();
	void DrawAlive();
	void DrawDamage();
	void DrawDeath();
	void DrawEnd();

	//生存状態での状態別更新
	void AliveIdle();
	void AliveRotation();
	void AliveCall();

	//ターゲットを見る処理
	void LookTarget();

	//アニメーションの終了処理
	void FinishAnim() override;

	//画像の表示確認
	void CheckIsHelp();

	//ヘルプ描画
	void DrawHelp();

	//デバッグ
	void DebagUpdate();
	void DebagDraw();
};

