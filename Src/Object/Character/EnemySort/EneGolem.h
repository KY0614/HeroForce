#pragma once

#include "Enemy.h"

class EneGolem : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_IDLE = 1;		//待機アニメーション
	static constexpr int ANIM_WALK = 2;		//歩きアニメーション
	static constexpr int ANIM_KNOCK = 12;	//倒れアニメーション
	
	static constexpr int ANIM_PUNCH = 6;		//殴りアニメーション(スキル１)
	static constexpr int ANIM_MOWDOWN = 8;		//薙ぎ払いアニメーション(スキル２)
	static constexpr int ANIM_SHOUT = 10;		//叫びアニメーション(スキル３)
	static constexpr int ANIM_JUMP_ATK = 11;	//ジャンプ攻撃アニメーション(スキル４)
	
	static constexpr int ANIM_JUMP = 3;			//ジャンプアニメーション(固有１)
	static constexpr int ANIM_LANDING = 4;		//着地アニメーション(固有２)
	static constexpr int ANIM_PRE_PUNCH = 5;	//殴り準備アニメーション(固有３)
	static constexpr int ANIM_PRE_MOWDOWN = 7;	//薙ぎ払い準備アニメーション(固有４)
	static constexpr int ANIM_PRE_SHOUT = 9;	//叫び準備アニメーション(固有５)

	//アニメーション速度
	static constexpr float SPEED_ANIM_IDLE = 60.0f;			//待機アニメーション速度
	static constexpr float SPEED_ANIM_WALK = 60.0f;			//歩きアニメーション速度
	static constexpr float SPEED_ANIM_KNOCK = 60.0f;		//倒れアニメーション速度
	static constexpr float SPEED_ANIM_PUNCH = 60.0f;		//パンチアニメーション速度
	static constexpr float SPEED_ANIM_PRE_PUNCH = 60.0f;	//パンチ前アニメーション速度
	static constexpr float SPEED_ANIM_MOWDOWN = 60.0f;		//薙ぎ払いアニメーション速度
	static constexpr float SPEED_ANIM_PRE_MOWDOWN = 30.0f;	//薙ぎ払いまえアニメーション速度
	static constexpr float SPEED_ANIM_PRE_SHOUT = 60.0f;	//叫び前アニメーション速度
	static constexpr float SPEED_ANIM_SHOUT = 40.0f;		//叫びアニメーション速度
	static constexpr float SPEED_ANIM_JUMP = 60.0f;			//ジャンプアニメーション速度

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,200.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標
	static constexpr float SCALE = 0.01f;		//モデルの大きさ

	//フレーム(ボーン)番号
	static constexpr int FRAME_L_HAND = 24;		//左の手のひら
	static constexpr int FRAME_L_FOOT = 46;		//左の足
	
	static constexpr int FRAME_R_HAND = 38;		//右の手のひら
	static constexpr int FRAME_R_FOOT = 50;		//右の足
	
	static constexpr int FRAME_HEAD = 15;		//頭

	//攻撃関係
	static constexpr float ALERT_TIME = 0.8f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 0.5f;	//攻撃の休憩時間

	//範囲関係
	static constexpr float SEARCH_RANGE = 2000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 700.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

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

	//スキルの警告判定用相対座標
	static constexpr VECTOR SKILL_ONE_ALERT = { 0.0f,0.0f,210.0f };	//スキル１の警告判定用相対座標
	//スキルエフェクト
	static constexpr float SKILL_ONE_EFF_SIZE = 10.0f;				//スキル１エフェクトの大きさ


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

	//スキルの警告判定用相対座標
	static constexpr VECTOR SKILL_TWO_ALERT = { 0.0f,0.0f,200.0f };		//スキル２の警告判定用相対座標
	//スキルの警告判定用範囲
	static constexpr float SKILL_TWO_ALERT_RANGE_X = 200.0f;			//スキル２の警告判定用範囲

	//スキルの当たり判定半径
	static constexpr float SKILL_THREE_COL_RADIUS = 100.0f;	//スキル３の当たり判定半径
	//スキルの攻撃力
	static constexpr float SKILL_THREE_POW = 20.0f;			//スキル３の攻撃力
	//スキルの持続時間
	static constexpr float SKILL_THREE_DURATION = 1.0f;		//スキル３の持続時間
	//スキルの後隙
	static constexpr float SKILL_THREE_BACKLASH = 1.0f;		//スキル３の後隙
	//スキル関係
	static constexpr ATK SKILL_THREE = { AsoUtility::VECTOR_ZERO	//スキル３の要素
		,SKILL_THREE_COL_RADIUS
		,SKILL_THREE_POW
		,SKILL_THREE_DURATION
		,SKILL_THREE_BACKLASH
		,0.0f };

	//スキル３の効果範囲
	static constexpr float SKILL_THREE_FALL_RADIUS = 500.0f;	//スキル３の隕石の落ちる範囲の半径
	//攻撃生成間隔
	static constexpr float SKILL_THREE_DELAY = 0.5f;			//スキル３の攻撃生成間隔
	//攻撃回数
	static constexpr int SKILL_THREE_MAX_CNT = 5;				//スキル３の攻撃回数
	//スキルエフェクト
	static constexpr float SKILL_THREE_SHOUT_EFF_SIZE = 10.0f;	//叫びエフェクトの大きさ
	static constexpr float SKILL_THREE_ATK_EFF_SIZE = 25.0f;	//叫びの攻撃エフェクトの大きさ


	//コンストラクタ
	EneGolem(const VECTOR& _spawnPos);

	//解放
	void Destroy(void)override;

private:	
	//****************************************************************
	//変数
	//****************************************************************

	int skillThreeCnt_;							//スキル３の生成カウンタ
	float skillThreeDelayCnt_;					//スキル３の発生間隔用カウンタ
	bool isPreSkillThreeAtk_;					//スキル３の発生予兆生成判定(true:生成しきった)
	ATK skillThreePreAtk_[SKILL_THREE_MAX_CNT];	//スキル３の発生予兆用

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

	//警告処理
	void Alert(void)override;

	//スキル1の警告
	void AlertSkill_One(void)override;
	
	//スキル2の警告
	void AlertSkill_Two(void);
	
	//スキル3の警告
	void AlertSkill_Three(void);

	//スキル1(殴り)
	void Skill_One(void)override;

	//スキル2(薙ぎ払い)
	void Skill_Two(void);

	//スキル3(叫び攻撃)
	void Skill_Three(void);

	//攻撃判定の初期化
	void ResetAlertJudge(void)override;
	
	//攻撃判定の初期化
	void ResetAtkJudge(void)override;

	//状態遷移(攻撃)
	void ChangeStateAtk(void)override;

	/// <summary>
	/// 円範囲のランダム一点を取る
	/// </summary>
	/// <param name="_myPos">攻撃者の座標</param>
	/// <param name="_r">攻撃生成範囲</param>
	/// <param name="_tPos">攻撃自体の座標</param>
	void GetRandomPointInCircle(const VECTOR _myPos, const int _r, VECTOR& _tPos);

	/// <summary>
	/// 攻撃達が重なっていないか
	/// </summary>
	/// <param name="_thisAtk">比較したい攻撃</param>
	/// <param name="_minDist">攻撃範囲の直径</param>
	/// <returns>攻撃が重なったか(true:重なった)</returns>
	bool IsOverlap(ATK& _thisAtk, float _minDist);
};
