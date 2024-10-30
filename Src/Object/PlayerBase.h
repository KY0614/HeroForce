#pragma once
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include<map>
#include "UnitBase.h"
class PlayerBase:
    public UnitBase
{
public:
   
    //各アニメーション番号
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 10;

    //アニメーションスピード
    static constexpr int SPEED_ANIM_IDLE = 20.0f;
    static constexpr int SPEED_ANIM_RUN = 80.0f;
    static constexpr int SPEED_ANIM_DODGE = 30.0f;
    static constexpr int SPEED_ANIM_ATK = 50.0f;

    static constexpr float SCALE = 0.2f;


    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float FRAME_ATK_DURATION = 0.5f;
    static constexpr float FRAME_SKILL1_DURATION = 0.5f;
    static constexpr float FRAME_SKILL2_DURATION = 1.5f;
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_DODGE_MAX = 1.0f*SCALE;
    //static constexpr float FRAME_DODGE_MAX = 0.2f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 1.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    static constexpr int MAX_HP = 100;
    //攻撃座標
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,20.0f,20.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,20.0f,20.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,20.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = SCALE * 100.0f;
    static constexpr float COL_SKILL1 = SCALE * 150.0f;
    static constexpr float COL_SKILL2 = SCALE * 200.0f;




    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;
    
protected:

    ATK skill1_;
    ATK skill2_;



    struct ACT
    {
        ATK atk;
        ATK skill;
    };

    enum class STATE
    {
        IDLE
        ,MOVE
        ,ATK
        ,DODGE
    };

    enum class SKILL_NUM
    {
        ONE
        ,TWO
        ,MAX
    };

   

    enum class CNTL
    {
        KEYBOARD
        ,PAD
    };

    //アニメNo初期化
    void InitAnimNum(void);

    //操作系（キーボード)
    void KeyBoardControl(void);

    //ゲームパッド
    void GamePad(void);

    CNTL cntl_;

    //コントローラー変更用関数
    void ChangeControll(CNTL _cntl);

    //プレイヤー(CPUとユーザー)共通処理
    void Common(void);

    //デバッグ用関数
    void DrawDebug(void);

    //カウント関数
    void Count(float& _cnt)
    { 
        _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


    //各アクションの共通処理
    void Action(ATK& _act);

    //攻撃座標の同期
    void SyncActPos(ATK& _act,VECTOR _colPos);


    //移動関連
    //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);
    //方向処理
    void Turn(float _deg, VECTOR _axis);

    //移動スピード
    float speedMove_;

    bool IsMove(void) { return speedMove_ > 0; }

    


    //攻撃
    //-------------------------------------

    //攻撃処理
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash(); }


    ATK atk_;
    
    //回避関連
    //---------------------------------------
    bool IsDodge(void) { return 0.0f<frameDodge_&&frameDodge_ < FRAME_DODGE_MAX; }
    //クールタイム中かどうか
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    //ドッジカウント初期化
    void ResetDodgeFrame(void){frameDodge_ = 0.0f; }

    float frameDodge_;
    float dodgeCdt_;

    

    //スキル仮想関数
    //-----------------------------------------
    SKILL_NUM skillNo_;
    //スキル1
    virtual void Skill_1(void);

     //スキル2
     virtual void Skill_2(void);

     //スキル
     //virtual void Skill(void);
     /// <summary>
     /// スキル中フラグ
     /// </summary>
     /// <param name="_frameSkillNo">スキルフレーム最大値(今はスキル1か2)</param>
     /// <returns>スキル中/スキル中でない</returns>
     bool IsSkill1Action(void) { return skills_[SKILL_NUM::ONE].IsAttack() || skills_[SKILL_NUM::ONE].IsBacklash(); }
     bool IsSkill2Action(void) { return skills_[SKILL_NUM::TWO].IsAttack() || skills_[SKILL_NUM::TWO].IsBacklash(); }

     bool IsSkillAction(SKILL_NUM _num) { return skills_[_num].IsAttack() || skills_[_num].IsBacklash(); }
     //すべてのスキルが使用中かどうか
     bool IsSkillAll(void) { return IsSkillAction(SKILL_NUM::ONE) || IsSkillAction(SKILL_NUM::TWO); }


     //スキルクールタイム中フラグ
     bool IsSkillCool(void);

     //スキルごとに再生するアニメーションを決める
     void SkillAnim(void);

     //スキル持続時間
     std::map<SKILL_NUM,float> skillCnt_;

     //スキルクールタイム
     std::map<SKILL_NUM,float> skillCdt_;

     std::string skillNum_;

     //ダメージ関数
     void Damage(void);

     int leftStickX_;

     int leftStickY_;

     float stickDeg_;

     std::map<SKILL_NUM, ATK> skills_;





     //デバッグ用
     void InitDebug(void);
     unsigned int color_Col_;
     unsigned int color_Atk_;
     unsigned int color_skl1_;
     unsigned int color_skl2_;
   
private:
};

