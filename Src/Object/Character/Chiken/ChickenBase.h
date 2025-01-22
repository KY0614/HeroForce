#pragma once
#include <functional>
#include <memory>
#include <map>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
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

	enum class ALIVE_MOVE
	{
		IDLE,		//立ち止まる
		ROTATION,	//その場を逃げ回る
		CALL,		//助けを呼ぶ
		MAX		
	};

	//キャラクターサイズ
	static constexpr VECTOR SCALE = { 1.3f,1.3f, 1.3f };

	//パラメーター関連
	static constexpr int DEFAULT_LIFE = 300;
	static constexpr int DEFAULT_ATK = 0;
	static constexpr int DEFAULT_DEF = 50;
	static constexpr float DEFAULT_SPEED = 3.0f;

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
	static constexpr int IS_HELP_CNT = 3 * Application::DEFAULT_FPS;

	//HELP相対位置
	static constexpr VECTOR LOCAL_HELP_POS = { 0,180,0 };

	//HP描画
	static constexpr VECTOR LOCAL_HP_POS = { 0, 120, 0 };

	ChickenBase();
	~ChickenBase();

	virtual void Create(VECTOR& pos);	//生成位置とターゲットのトランスフォームをもらう
	virtual void Update(void)override;
	void Draw(void)override;
	
	//画像表示の設定
	void SetIsHelp();

	//ターゲットの座標設定
	void SetTarget(const VECTOR pos);

	//ダメージを与える
	void SetDamage(const int damage);

	//状態を返す
	STATE GetState() const;

protected:

	//画像
	int imgHelp_;

	//移動スピード
	float moveSpeed_;

	//フェード用ステップ
	float fadeStep_;

	//画像の表示
	bool isHelp_;
	int isHelpCnt_;

	//ターゲット用情報
	VECTOR targetPos_;

	//状態
	STATE state_;

	//生存時の状態
	ALIVE_MOVE aliveState_;
	
	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	std::map<ALIVE_MOVE, std::function<void(void)>> aliveChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// 状態管理(描画)
	std::function<void(void)> stateDraw_;

	// 生存時状態管理
	std::function<void(void)> stateAliveUpdate_;

	//UIインスタンス生成
	std::unique_ptr<CpuHpBar> hpUi_;

	//モデル設定
	void ModelSet();

	//画像読み込み
	void LoadImages();

	//パラメーターの設定
	virtual void SetParam();

	//アニメーション番号の初期化
	virtual void InitAnimNum(void);

	//UI設定
	void SetUiParam();

	//状態変更
	void ChangeState(STATE state);
	void ChangeStateNone();
	void ChangeStateAlive();
	void ChangeStateDamage();
	void ChangeStateDeath();
	void ChangeStateEnd();

	void ChangeAliveState(ALIVE_MOVE state);
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

