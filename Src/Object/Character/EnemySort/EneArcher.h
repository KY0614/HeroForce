#pragma once
#include "Enemy.h"

class Arrow;

class EneArcher : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//弓矢関係
	static constexpr int ARROW_SIZE_MAX = 5;	//矢の最大保持数
	static constexpr float RELOAD_TIME = 5.0f;	//矢のリロード時間
	static constexpr float ARROW_SPEED = 8.0f;	//矢の速さ

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_SKILL_ONE = 95;	//スキル1アニメーション
	static constexpr int ANIM_AIMING = 5;		//構えアニメーション(固有アニメーション1)
	static constexpr int ANIM_RELOAD = 6;		//弾補充アニメーション(固有アニメーション2)

	//アニメーション速度(キャラ固有)
	static constexpr int SPEED_ANIM_RELOAD = ARROW_SIZE_MAX * 7.0f;		//弾補充アニメーション(固有アニメーション2)

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標

	//攻撃関係
	static constexpr float ALERT_TIME = 1.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 2.5f;	//攻撃の休憩時間

	//範囲関係
	static constexpr float SEARCH_RANGE = 2000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 1500.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの当たり判定半径
	static constexpr float SKILL_ONE_COL_RADIUS = 23.0f;	//スキル１の当たり判定半径

	//スキルの攻撃力
	static constexpr float SKILL_ONE_POW = 15.0f;			//スキル１の攻撃力

	//スキルの持続時間
	static constexpr float SKILL_ONE_DURATION = 3.0f;		//スキル１の持続時間

	//スキルの後隙
	static constexpr float SKILL_ONE_BACKLASH = 2.5f;		//スキル１の後隙

	//スキル関係
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//スキル１の要素
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//コンストラクタ
	EneArcher(const VECTOR& _spawnPos);

private:
	//****************************************************************
	//変数
	//****************************************************************

	int arrowMdlId_;							//矢のモデル
	std::vector<std::shared_ptr<Arrow>> arrow_;	//弓矢
	bool isShotArrow_;							//矢を放ったかの判定(true:放った)
	int arrowCnt_;								//矢の使用個数カウンタ
	float reloadCnt_;							//矢のリロード時間
	Arrow* lastArrow_;							//最後に生成された矢

	//****************************************************************
	//関数
	//****************************************************************

	//解放
	void Destroy(void)override;

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
	//リロード中かどうかを返す
	const bool IsReload(void)const { return arrowCnt_ >= ARROW_SIZE_MAX; }

	//スキル1の警告
	void AlertSkill_One(void)override;

	//スキル1
	void Skill_One(void)override;

	//矢の生成
	Arrow& CreateArrow(void);

	//矢のリロード
	void ReloadArrow(void);

	//アニメーション終了時の動き
	void FinishAnim(void)override;

	//攻撃判定の初期化
	void ResetAtkJudge(void)override;

	//状態遷移(休憩)
	void ChangeStateAtk(void)override;

	//更新(攻撃)
	void Update(void)override;
	//更新(休憩)
	void UpdateBreak(void)override;
	//描画
	void Draw(void)override;
};

