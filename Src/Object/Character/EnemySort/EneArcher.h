#pragma once
#include "../Enemy.h"

class Arrow;

class EneArcher : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_SKILL_ONE = 7;	//スキル1アニメーション
	static constexpr int ANIM_SKILL_TWO = 8;	//スキル2アニメーション
	static constexpr int ANIM_RELOAD = 6;		//弾補充アニメーション(固有アニメーション)

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標

	//攻撃関係
	static constexpr float ALERT_TIME = 2.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 3.0f;	//攻撃の休憩時間

	//敵自身の当たり判定半径
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//敵ステータス
	static constexpr int HP_MAX = 5;			//敵の最大体力
	static constexpr int STUN_DEF_MAX = 100;	//敵の最大スタン防御値

	//速度関係
	static constexpr float WALK_SPEED = 2.0f;	//歩きの速度
	static constexpr float RUN_SPEED = 4.0f;	//走りの速度

	//範囲関係
	static constexpr float SEARCH_RANGE = 1000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの当たり判定半径
	static constexpr float SKILL_ONE_COL_RADIUS = 10.0f;	//スキル１
	static constexpr float SKILL_TWO_COL_RADIUS = 24.0f;	//スキル２

	//スキル関係
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO,SKILL_ONE_COL_RADIUS,1.0f,1.0f,2.0f,0.0f };	//スキル１
	static constexpr ATK SKILL_TWO = { AsoUtility::VECTOR_ZERO,SKILL_TWO_COL_RADIUS,5.0f,2.0f,3.0f,0.0f };	//スキル２

	//弓矢関係
	static constexpr int ARROW_SIZE_MAX = 10;	//矢の最大保持数
	static constexpr int SHOT_ARROW_NUM = 4;	//一度発射する矢の個数

private:
	//****************************************************************
	//関数
	//****************************************************************

	//キャラ固有設定
	void SetParam(void)override;

	//アニメーション番号の初期化
	void InitAnimNum(void)override;

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
	void Skill_Two(void)override;

	//矢の生成
	void CreateArrow(void);

	//状態遷移における初期化処理
	void InitChangeState(void)override;

	//****************************************************************
	//変数
	//****************************************************************

	int arrowMdlId_;				//矢のモデル
	Arrow* arrow_[ARROW_SIZE_MAX];	//弓矢
};

