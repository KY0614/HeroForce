#pragma once
#include<vector>
#include <functional>
#include"../../../Manager/Decoration/EffectManager.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Utility/AsoUtility.h"
#include"../../System/GameUi/EnemyHpBar.h"
#include "../../UnitBase.h"

class EnemyHpBar;

class Enemy : public UnitBase
{
public:

//#define DEBUG_ENEMY

	//移行テストしております

	//****************************************************************
	//定数(キャラ共通)
	//****************************************************************

	//アニメーション番号
	static constexpr int ANIM_IDLE = 14;	//待機アニメーション
	static constexpr int ANIM_WALK = 93;	//歩きアニメーション
	static constexpr int ANIM_RUN = 54;		//走りアニメーション
	static constexpr int ANIM_DAMAGE = 39;	//ダメージアニメーション
	static constexpr int ANIM_DEATH = 24;	//やられアニメーション
	static constexpr int ANIM_ENTRY = 74;	//出現アニメーション

	//アニメーション速度
	static constexpr float SPEED_ANIM_WALK = 60.0f;	//歩きアニメーション速度

	//移動速度
	static constexpr float RUN_SPEED_MULTI = 1.2f;	//走る速度の倍率

	//補完用
	static constexpr float KEEP_COL_STAGE_POS = 500.0f;	//ステージ接触時座標補完用
	static constexpr float COL_STAGE_CNT = 0.3f;		//ステージ接触時補完用カウント
	
	static constexpr float START_CNT = 3.0f;			//スタート時補完用カウント

	static constexpr float COL_RADIUS = 50.0f;			//敵生成時の距離用

	//****************************************************************
	//列挙型
	//****************************************************************

	//現在状態
	enum class STATE
	{
		NORMAL			//通常
		,ALERT			//攻撃前(警告)
		,ATTACK			//攻撃
		,BREAK			//休憩
		,MAX
	};

	//敵の探索状態
	enum class SEARCH_STATE
	{
		CHICKEN_SEARCH		//鶏探し中
		,CHICKEN_FOUND		//追跡中(鶏)
		,PLAYER_FOUND		//追跡中(プレイヤー)
		,MAX
	};

	//敵のスキル行動
	enum class ATK_ACT
	{
		SKILL_ONE		//スキル1
		,SKILL_TWO		//スキル2
		,SKILL_THREE	//スキル3
		,MAX
	};

	//自分の種類のラベル分け　※作るかも！！

	//****************************************************************
	//メンバ関数
	//****************************************************************

	//コンストラクタ
	Enemy(const VECTOR& _spawnPos);
	//デストラクタ
	~Enemy() = default;

	//解放
	virtual void Destroy(void)override;

	//初期化
	void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	//警告時間中かどうかを返す(純粋仮想関数)
	virtual const bool IsAlertTime(void)const = 0;
	//休憩時間中かどうかを返す(純粋仮想関数)
	virtual const bool IsBreak(void)const = 0;

	//スタン中かどうかを返す
	//const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//現在のスキルの全配列を返す
	const std::vector<ATK> GetAtks(void)const { return nowSkill_; }

	//索敵範囲を返す
	const float GetSearchRange(void)const { return searchRange_; }

	//攻撃開始範囲を返す
	const float GetAtkStartRange(void)const { return atkStartRange_; }

	//経験値を返す
	const float GetExp(void)const { return exp_; }

	//現在状態を返す
	const STATE GetState(void) { return state_; }

	//攻撃情報を設定
	void SetAtk(const ATK& _atk) { atk_ = _atk; }

	void SetAtksIsHit(int _arrayNum, const bool _isHit) { nowSkill_[_arrayNum].isHit_ = _isHit; }

	//探索状態を返す
	SEARCH_STATE GetSearchState(void) { return searchState_; }

	/// <summary>
	/// アニメーションが終わったかを返す
	/// </summary>
	/// <param name="_anim">アニメーション番号(指定しなかったらアニメーションが終わってるかのみで判断)</param>
	/// <returns>(指定された)アニメーションが終わっているか</returns>
	bool IsFinishAnim(const ANIM _anim = ANIM::NONE);

	/// <summary>
	/// 探索状態を変更
	/// </summary>
	/// <param name="_searchState">変更する探索状態</param>
	void ChangeSearchState(const SEARCH_STATE _searchState);

	/// <summary>
	/// 標的予定対象の座標を変更
	/// </summary>
	/// <param name="_preTargetPos">標的予定対象の座標</param>
	void SetPreTargetPos(const VECTOR _preTargetPos) { if(!isColStage_ && startCnt_ >= START_CNT)preTargetPos_ = _preTargetPos; }

	/// <summary>
	/// 標的の座標を変更
	/// </summary>
	/// <param name="_targetPos">標的の座標</param>
	void SetTargetPos(const VECTOR _targetPos) { if(!isColStage_ && startCnt_ >= START_CNT)targetPos_ = _targetPos; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	/// <param name="_stunPow">スタン攻撃力</param>
	//void Damage(const int _damage, const int _stunPow);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeState(const STATE _state);

	//ステージに当たったら距離をとる
	void KeepCollStageDistance(void);

protected:

	//****************************************************************
	//メンバ変数
	//****************************************************************

	STATE state_;	//現在の状態

	std::function<void(void)> stateUpdate_;						//状態ごとの更新管理
	std::map<STATE,std::function<void(void)>> stateChanges_;	//状態遷移の管理

	std::map<ANIM, float>changeSpeedAnim_;	//アニメーション速度変更用

	float alertCnt_;		//攻撃の警告時間カウンタ
	float breakCnt_;		//攻撃の休憩時間カウンタ

	float walkSpeed_;		//敵ごとの歩く速度
	float runSpeed_;		//敵ごとの走る速度

	VERTEX3D alertVertex_[6];	//警告用の頂点情報

	std::map<ATK_ACT, ATK> skills_;								//スキルの種類
	std::vector<ATK> nowSkill_;									//現在のスキル
	std::function<void(void)> alertNowSkill_;					//現在のスキルの警告処理
	std::function<ATK&(void)> createSkill_;						//スキルの生成
	std::function<void(void)> processSkill_;					//スキルの処理
	std::map<ATK_ACT, std::function<void(void)>> alertSkills_;	//スキルごとの警告
	std::map<ATK_ACT, std::function<void(void)>> changeSkill_;	//スキルの変更用
	ATK_ACT atkAct_;											//スキルの種別用
	ATK* lastAtk_;												//最後に生成されたスキル
	bool isEndAlert_;											//警告が終わったか(true:終了した)
	bool isEndAllAtkSign_;										//全攻撃予兆が終了したか(true:終了した)
	bool isEndAllAtk_;											//全攻撃が終了したか(true:終了した)

	std::vector<ANIM> skillPreAnims_;	//スキルに対応した予備アニメーション
	std::vector<ANIM> skillAnims_;		//スキルに対応したアニメーション
	ANIM nowSkillPreAnim_;				//現在のスキルの予備アニメーション
	ANIM nowSkillAnim_;					//現在のスキルアニメーション

	VECTOR localCenterPos_;	//敵中央の相対座標
	VECTOR colPos_;			//敵自身の当たり判定用の相対座標

	SEARCH_STATE searchState_;											//探索判定
	std::map<SEARCH_STATE, std::function<void(void)>> SearchStateInfo_;	//探索状態による情報更新

	VECTOR preTargetPos_;	//標的予定対象の座標
	VECTOR targetPos_;		//標的の座標

	std::unique_ptr<EnemyHpBar> ui_;	//UI関係

	float searchRange_;		//索敵範囲
	float atkStartRange_;	//攻撃開始範囲

	//int stunDefMax_;	//気絶防御値の最大値
	//int stunDef_;		//気絶防御値

	bool isColStage_;	//ステージに当たったか(true:当たった)
	float colStageCnt_;	//ステージ接触補完用カウンタ

	float startCnt_;	//スタート補完用カウンタ

	float exp_;			//経験値

	//****************************************************************
	//メンバ関数
	//****************************************************************

	//キャラ固有設定
	virtual void SetParam(void) = 0;

	//外部関係
	void ParamLoad(CharacterParamData::UNIT_TYPE type)override;

	//アニメーション関係の初期化
	virtual void InitAnim(void);

	//エフェクトの初期化
	virtual void InitEffect(void);

	//スキルの初期化
	virtual void InitSkill(void) = 0;

	//スキルの前処理
	virtual void Alert(void);

	//スキル1の警告
	virtual void AlertSkill_One(void) = 0;

	//敵の攻撃処理
	virtual void Attack(void);

	//スキル1
	virtual void Skill_One(void) = 0;

	//スキルのランダム生成
	virtual void RandSkill(void);

	/// <summary>
	/// スキル生成準備
	/// </summary>
	/// <param name="_atkAct">生成するスキル</param>
	void SetUpSkill(ATK_ACT _atkAct);

	/// <summary>
	/// 警告範囲を描画する
	/// </summary>
	/// <param name="_pos">中心座標</param>
	/// <param name="_widthX">Xの描画の大きさ</param>
	/// <param name="_widthZ">Zの描画の大きさ</param>
	void CreateAlert(const VECTOR& _pos, const float _widthX, const float _widthZ);

	/// <summary>
	/// スキルの生成
	/// </summary>
	/// <param name="_atkAct">生成するスキル</param>
	/// <returns>生成したスキル</returns>
	ATK& CreateSkill(ATK_ACT _atkAct);

	//アニメーション終了時の動き
	virtual void FinishAnim(void)override;

	//攻撃判定の初期化
	virtual void ResetAlertJudge(void);

	//頂点情報の初期化
	void ResetAlertVertex(void);

	//攻撃判定の初期化
	virtual void ResetAtkJudge(void);

	//状態遷移(通常)
	void ChangeStateNormal(void);
	//状態遷移(攻撃警告)
	void ChangeStateAlert(void);
	//状態遷移(攻撃)
	virtual void ChangeStateAtk(void);
	//状態遷移(休憩)
	void ChangeStateBreak(void);

	//更新(通常)
	void UpdateNml(void);
	//更新(攻撃警告)
	void UpdateAlert(void);
	//更新(攻撃)
	void UpdateAtk(void);
	//更新(休憩)
	virtual void UpdateBreak(void);

	//描画(※デバッグ)
	virtual void DrawDebug(void);

	//探索状態ごとの情報更新(探索中)
	void SearchMoveInfo(void);

	//探索状態ごとの情報更新(追跡)
	void FoundMoveInfo(void);

	/// <summary>
	/// とある点からとある点までの移動ベクトルを返す
	/// </summary>
	/// <param name="_start">狙う側</param>
	/// <param name="_goal">向かう先</param>
	/// <param name="_speed">設定速度(未設定だと、方向ベクトルのみを返す)</param>
	/// <returns>向かう先までの移動ベクトル</returns>
	const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	//移動
	void Move(void);
};