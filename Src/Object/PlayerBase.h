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
    static constexpr int SKILL_NUM = 2;

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
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_DODGE_MAX = 0.43f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 1.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    static constexpr int MAX_HP = 100;
    //攻撃座標
    static constexpr VECTOR COL_LOCAL_POS = { 0.0f,20.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = SCALE * 200.0f;




    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    
protected:

    enum class STATE
    {
        IDLE
        ,MOVE
        ,ATK
        ,DODGE
    };

    enum class SKILL
    {
        ONE
        ,TWO
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

    //デバッグ用関数
    void DrawDebug(void);

    //カウント関数
    void Count(float& _cnt)
    { 
        _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


   



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
    void Attack(void);
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
    SKILL skill_;
    //スキル1
    virtual void Skill_1(void);

     //スキル2
     virtual void Skill_2(void);
     /// <summary>
     /// スキル中フラグ
     /// </summary>
     /// <param name="_frameSkillNo">スキルフレーム最大値(今はスキル1か2)</param>
     /// <returns>スキル中/スキル中でない</returns>
     bool IsSkill(SKILL _skill) { return skillCnt_[_skill]>0.0f && skillCnt_[_skill] < FRAME_SKILL1; }

     //スキルクールタイム中フラグ
     bool IsSkillCool(void);

     //スキル持続時間
     std::map<SKILL,float> skillCnt_;

     //スキルクールタイム
     std::map<SKILL,float> skillCdt_;

     std::string skillNum_;

     //ダメージ関数
     void Damage(void);

     int leftStickX_;

     int leftStickY_;

     float stickDeg_;







     //デバッグ用
     void InitDebug(void);
     unsigned int color_Col_;
     unsigned int color_Atk_;
   
private:
};

