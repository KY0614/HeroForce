#pragma once
#include "Enemy.h"

class EneMage : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_SKILL_ONE = 77;	//スキル1アニメーション
	static constexpr int ANIM_CHARGE = 79;		//溜めアニメーション(固有アニメーション)

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標

	//フレーム(ボーン)番号
	static constexpr int FRAME_ROD = 18;	//杖

	//攻撃関係
	static constexpr float ALERT_TIME = 1.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 3.0f;	//攻撃の休憩時間

	//範囲関係
	static constexpr float SEARCH_RANGE = 2500.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 1000.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの射程
	static constexpr float SKILL_ONE_RANGE = 40.0f;			//スキル１の射程

	//スキルの当たり判定半径
	static constexpr float SKILL_ONE_COL_RADIUS = 40.0f;	//スキル１の当たり判定半径

	//スキルの攻撃力
	static constexpr float SKILL_ONE_POW = 1.0f;			//スキル１の攻撃力

	//スキルの持続時間
	static constexpr float SKILL_ONE_DURATION = 3.0f;		//スキル１の持続時間

	//スキルの後隙
	static constexpr float SKILL_ONE_BACKLASH = 1.0f;		//スキル１の後隙

	//スキルの速度
	static constexpr float SKILL_ONE_SPEED = 4.0f;			//スキル1の速度

	//攻撃回数
	static constexpr int SKILL_ONE_MAX_CNT = 5;				//スキル1の攻撃回数

	//追尾時間
	static constexpr float SKILL_ONE_SHOT_DELAY = 1.0f;		//スキル１の攻撃ディレイ	

	//スキルの全体時間
	static constexpr float SKILL_ONE_ALL_TIME =				//スキル１の全体時間
		SKILL_ONE_MAX_CNT * SKILL_ONE_SHOT_DELAY
		+ SKILL_ONE_DURATION + SKILL_ONE_BACKLASH
		+ 0.5f;

	//スキル関係
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//スキル１の要素
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//スキルエフェクト
	static constexpr float SKILL_ONE_EFF_SIZE = 10.0f;	//スキル１エフェクトの大きさ

	//コンストラクタ
	EneMage(const VECTOR& _spawnPos);

	//解放
	void Destroy(void)override;

private:

	//****************************************************************
	//変数
	//****************************************************************

	VECTOR skillOneShot_;		//スキル１の参照座標用
	float skillOneDelayCnt_;	//スキル１の発生ディレイ用
	int skillOneShotCnt_;		//スキル１のカウント用

	float skillAllCnt_;			//スキルの全体カウント用

	//****************************************************************
	//関数
	//****************************************************************

	//キャラ固有設定
	void SetParam(void)override;

	//アニメーション番号の初期化
	void InitAnim(void)override;

	//エフェクトの初期化
	void InitEffect(void)override;

	//スキルの初期化
	void InitSkill(void)override;

	//警告時間中かどうかを返す
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//休憩時間中かどうかを返す
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//スキル1の警告
	void AlertSkill_One(void)override;

	//攻撃
	void Attack(void)override;

	//スキル1
	void Skill_One(void)override;

	//描画(※デバッグ)
	void DrawDebug(void)override;

	//アニメーション終了時の動き
	void FinishAnim(void)override;

	//攻撃判定の初期化
	void ResetAtkJudge(void)override;

	//状態遷移(攻撃)
	void ChangeStateAtk(void)override;
};

