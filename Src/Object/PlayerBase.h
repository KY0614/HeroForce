#pragma once
#include"../Utility/AsoUtility.h"
#include<map>
#include "UnitBase.h"
class PlayerBase:
    public UnitBase
{
public:
    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float FRAME_ATK_DURATION = 1.0f;
    static constexpr float FRAME_ATK_BACKRASH = 0.3f;
    static constexpr float FRAME_DODGE_MAX = 0.5f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 0.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    //各アニメーション番号
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_NUM = 2;





    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
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
    //球体の色
    unsigned int color_;
    //操作系（コントローラー）
    void KeyBoardControl(void);

    //デバッグ用関数
    void DrawDebug(void);


   



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
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash_(); }


    //攻撃フレーム
    

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
     bool IsSkill(SKILL _skill) { return skillCnt_[_skill] < FRAME_SKILL1; }

     //スキルクールタイム中フラグ
     bool IsSkillCool(void);

     //スキル持続時間
     std::map<SKILL,float> skillCnt_;

     //スキルクールタイム
     std::map<SKILL,float> skillCdt_;

     std::string skillNum_;

     //ゲッター
     //--------------------------------------------
     //位置
     const VECTOR GetPos(void)const override { return trans_.pos; }
     //角度
     const VECTOR GetRot(void)const override { return trans_.rot; }
     //大きさ
     const VECTOR GetScl(void)const override { return trans_.scl; }
     //防御力
     const float GetDef(void)const override { return def_; };
private:
};

