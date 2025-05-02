#pragma once
#include<DxLib.h>
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
class PlayerBase;
class PlayerInput
{
public:
    //*************************************************
//各アクション操作ボタン
//*************************************************
//移動
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;

    //攻撃
    static constexpr int ATK_KEY = KEY_INPUT_E;
    static constexpr InputManager::JOYPAD_BTN ATK_BTN = InputManager::JOYPAD_BTN::RIGHT;

    //スキル
    static constexpr int SKILL_KEY = KEY_INPUT_Q;
    static constexpr InputManager::JOYPAD_BTN SKILL_BTN = InputManager::JOYPAD_BTN::TOP;

    //スキル変更キー
    static constexpr int SKILL_CHANGE_KEY = KEY_INPUT_J;
    static constexpr InputManager::JOYPAD_BTN SKILL_CHANGE_BTN = InputManager::JOYPAD_BTN::R_BUTTON;

    //回避
    static constexpr int DODGE_KEY = KEY_INPUT_N;
    static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::LEFT;

    //アクションボタンの種類
    enum class ACT_CNTL
    {
        NONE    //何もしていないとき
        , MOVE   //移動
        , DODGE             //回避
        , NMLATK            //通常攻撃    
        , CHANGE_SKILL      //スキル切り替え
        , SKILL_DOWN        //短押しスキル
        , SKILL_KEEP        //長押しスキル(押しっぱなし)
        , SKILL_UP          //長押しスキル(離す)
    };

    //インスタンスの生成
    static void CreateInstance(void);
    static PlayerInput& GetInstance(void);

    
    void Update(PlayerBase* _player,InputManager::JOYPAD_NO _padNum,SceneManager::CNTL _cntl);




    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    //ゲッタ
    //----------------------------------------------------------------
    ACT_CNTL GetAct(void) { return actCntl_; }
    float GetStickDeg(void){ return stickDeg_; }
    float GetMoveDeg(void) { return moveDeg_; }

private:

    //シングルトン化するために
	PlayerInput(void);
	~PlayerInput(void)=default;

    //メンバ関数
    void InputKeyBoard(PlayerBase* _player);
    void InputPad(PlayerBase* _player, InputManager::JOYPAD_NO _padNum);

    //メンバ変数
    //-----------------------------------------------------------------------
    //操作管理用
    ACT_CNTL actCntl_;

    float leftStickX_;
    float leftStickY_;
    float stickDeg_;            //パッドのスティックの角度
    float moveDeg_;             //移動方向
};

