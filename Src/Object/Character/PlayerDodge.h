#pragma once
#include "../UnitBase.h"

class PlayerDodge
{
public:

	enum State
	{
		IDLE,			// 待機
		COOL_TIME,		// クールタイム
		DODGE_DURING,	// ドッジ中
	};

	static constexpr int ANIM_NUM = 28;
	static constexpr float SPEED_ANIM_DODGE = 30.0f;
	static constexpr float SPEED_DODGE = 15.0f;
	static constexpr float FRAME_DODGE_MAX = 1.0f * UnitBase::CHARACTER_SCALE;
	static constexpr float DODGE_CDT_MAX = 0.5f;

	void Init();
	void Update(Transform& transform);

	void Reset();

	//回避関連
  //---------------------------------------
	const bool IsDodge(void)const;

	//回避関連
	//---------------------------------------
	//回避可能か
	//回避中かどうか

	const bool IsDodgeable(void)const;
	
	//クールタイム中かどうか
	const bool IsCoolDodge(void)const;

	//// 回避
	//void Dodge(Transform* transform);

	//ドッジカウント初期化
	void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }

	const float GetDodgeCnt() { return dodgeCnt_; }
	void SetDodgeCnt(const float _dodgeCnt) { dodgeCnt_ = _dodgeCnt; }

private:
	// 回避系
	float dodgeCnt_;            //ドッジカウント
	float dodgeCdt_;            //ドッジの後隙

};

