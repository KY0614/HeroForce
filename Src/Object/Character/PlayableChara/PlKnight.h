#pragma once
#include"../Manager/SceneManager.h"
#include "../PlayerBase.h"
class PlKnight :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ

#ifdef DEBUG_ON
//クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#else // DEBUG_ON
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_ON

    //攻撃発生の時間
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 1.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

    //各攻撃の持続時間
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f /*- SKILL_TWO_START*/;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;

    //後隙
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;

    //各攻撃の座標
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;



    //固有アニメーション
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    PlKnight(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum);
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

private:
    float guardCnt_;        //ガード時間
};