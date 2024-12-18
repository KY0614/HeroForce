#pragma once

#include "../Enemy.h"

class EneGolem : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_IDLE = 1;		//待機アニメーション
	static constexpr int ANIM_WALK = 2;		//歩きアニメーション
	
	static constexpr int ANIM_PUNCH = 5;		//殴りアニメーション(スキル１)
	static constexpr int ANIM_MOWDOWN = 6;		//薙ぎ払いアニメーション(スキル２)
	static constexpr int ANIM_SHOUT = 7;		//叫びアニメーション(スキル３)
	static constexpr int ANIM_JUMP_ATK = 8;		//ジャンプ攻撃アニメーション(スキル４)
	
	static constexpr int ANIM_JUMP = 3;			//ジャンプアニメーション(固有１)
	static constexpr int ANIM_LANDING = 4;		//着地アニメーション(固有２)

	//TODO:のちに作る
	//static constexpr int ANIM_ENTRY = 74;		//出現アニメーション
	//static constexpr int ANIM_DAMAGE = 39;	//ダメージアニメーション
	//static constexpr int ANIM_DEATH = 24;		//やられアニメーション

	//アニメーション速度
	static constexpr float SPEED_ANIM_IDLE = 60.0f;		//待機アニメーション速度
	static constexpr float SPEED_ANIM_WALK = 60.0f;		//歩きアニメーション速度
	static constexpr float SPEED_ANIM_PUNCH = 60.0f;	//パンチアニメーション速度
	static constexpr float SPEED_ANIM_MOWDOWN = 60.0f;	//薙ぎ払いアニメーション速度
	static constexpr float SPEED_ANIM_SHOUT = 60.0f;	//叫びアニメーション速度
	static constexpr float SPEED_ANIM_JUMP = 60.0f;		//ジャンプアニメーション速度

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標
	static constexpr float SCALE = 0.01f;		//モデルの大きさ

	//フレーム(ボーン)番号
	static constexpr int FRAME_L_HAND = 24;		//左の手のひら
	static constexpr int FRAME_L_FOOT = 46;		//左の足
	
	static constexpr int FRAME_R_HAND = 38;		//右の手のひら
	static constexpr int FRAME_R_FOOT = 50;		//右の足
	
	static constexpr int FRAME_HEAD = 15;		//頭

	//攻撃関係
	static constexpr float ALERT_TIME = 1.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 1.0f;	//攻撃の休憩時間

	//敵自身の当たり判定半径
	static constexpr float MY_COL_RADIUS = 300.0f * CHARACTER_SCALE;

	//敵ステータス
	static constexpr int HP_MAX = 150;			//敵の最大体力
	static constexpr float ATK_POW = 120.0f;	//敵の攻撃
	static constexpr float DEF = 100.0f;		//敵の防御
	static constexpr int STUN_DEF_MAX = 100;	//敵の最大スタン防御値

	//速度関係
	static constexpr float WALK_SPEED = 2.0f;	//歩きの速度
	static constexpr float RUN_SPEED = 4.0f;	//走りの速度

	//範囲関係
	static constexpr float SEARCH_RANGE = 2000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 500.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの当たり判定半径
	static constexpr float SKILL_ONE_COL_RADIUS = 50.0f;	//スキル１の当たり判定半径
	//スキルの攻撃力
	static constexpr float SKILL_ONE_POW = 20.0f;			//スキル１の攻撃力
	//スキルの持続時間
	static constexpr float SKILL_ONE_DURATION = 1.0f;		//スキル１の持続時間
	//スキルの後隙
	static constexpr float SKILL_ONE_BACKLASH = 2.0f;		//スキル１の後隙
	//スキル関係
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//スキル１の要素
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//スキルの当たり判定半径
	static constexpr float SKILL_TWO_COL_RADIUS = 50.0f;	//スキル２の当たり判定半径
	//スキルの攻撃力
	static constexpr float SKILL_TWO_POW = 20.0f;			//スキル２の攻撃力
	//スキルの持続時間
	static constexpr float SKILL_TWO_DURATION = 1.0f;		//スキル２の持続時間
	//スキルの後隙
	static constexpr float SKILL_TWO_BACKLASH = 2.0f;		//スキル２の後隙
	//スキル関係
	static constexpr ATK SKILL_TWO = { AsoUtility::VECTOR_ZERO	//スキル２の要素
		,SKILL_TWO_COL_RADIUS
		,SKILL_TWO_POW
		,SKILL_TWO_DURATION
		,SKILL_TWO_BACKLASH
		,0.0f };

private:
	//****************************************************************
	//関数
	//****************************************************************

	//キャラ固有設定
	void SetParam(void)override;

	//アニメーション番号の初期化
	void InitAnim(void)override;

	//スキルの初期化
	void InitSkill(void)override;

	//警告時間中かどうかを返す
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//休憩時間中かどうかを返す
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//敵の攻撃処理
	void Attack(void)override;

	//スキル1
	void Skill_One(void)override;

	//スキル2
	void Skill_Two(void);

	//状態遷移(攻撃警告)
	void ChangeStateAlert(void)override;
};

