#pragma once
#include"../Manager/SceneManager.h"
#include "../PlayerBase.h"
class PlKnight :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ
    //スキル使用開始時にクールタイムを増やす時間
    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //ガード可能になる残りクールタイム(ガード時間1秒確保したいから)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;
#ifdef DEBUG_COOL
//クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

#else // DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_COOL

    //攻撃発生の時間
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 1.0f;
    static constexpr float SKILL_TWO_START = 0.3f;

    //各攻撃の持続時間
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f /*- SKILL_TWO_START*/;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;

    //後隙
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.0f;

    //各攻撃の座標
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;

    //攻撃威力
    static constexpr float ATK_POW = 7.0f;
    static constexpr float SKILL_ONE_POW = 30.0f;
    static constexpr float SKILL_TWO_POW = 0.0f;

    //VECTOR pos_;		//位置
    //float radius_;		//当たり判定の半径
    //float pow_;			//技威力
    //float duration_;	//持続時間（攻撃がどれくらい続くかを記述)
    //float backlash_;	//後隙（後隙がどれくらい続くかを記述)
    //float cnt_;			//カウンター
    //bool isHit_;

    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,COL_ATK
        ,ATK_POW
        ,FRAME_ATK_DURATION
        ,FRAME_ATK_BACKRASH
        ,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS
        ,COL_SKILL1 
        ,SKILL_ONE_POW
        ,FRAME_SKILL1_DURATION 
        ,FRAME_SKILL1_BACKRASH 
        ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS 
        ,COL_SKILL2 
        ,SKILL_TWO_POW
        ,FRAME_SKILL2_DURATION 
        ,FRAME_SKILL2_BACKRASH 
        ,0.0f,false };

    //固有アニメーション
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    PlKnight(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl);
    PlKnight(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum);
    ~PlKnight(void) = default;
    void SetParam(void)override;

protected:
    //攻撃アクションのパラメータの初期化
    void InitAct(void)override;
    void AtkFunc(void)override;

    void ResetGuardCnt(void)override;

    //各スキルの処理
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //固有アニメーション番号の初期化
    void InitCharaAnim(void)override;

    //長押しスキル
    void ChargeAct(void)override;

private:
    float guardCnt_;        //ガード時間
};