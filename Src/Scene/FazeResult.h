#pragma once
#include "SceneBase.h"
class FazeResult :
    public SceneBase
{
public:

    //ランクの種類数
    static constexpr int RANK_MAX_NUM = 4;
    //ランクの各種倍率
    static constexpr float BNS_RANK_S = 2.0f;
    static constexpr float BNS_RANK_A = 1.5f;
    static constexpr float BNS_RANK_B = 1.3f;
    static constexpr float BNS_RANK_C = 1.0f;

    //表示間隔
    static constexpr float INTERVEL = 1.25f;

    //フォントサイズ
    static constexpr int FONT_SIZE = 56;
    static constexpr int FONT_HEAD_SIZE = 72;

    //各種描画位置
    static constexpr int HEAD_TEXT_POS_X = Application::SCREEN_SIZE_X / 2;
    static constexpr int HEAD_TEXT_POS_Y = 30;

    static constexpr int MES1_POS_X = 30;
    static constexpr int MES1_POS_Y = 150;

    static constexpr int MES2_POS_X = 30;
    static constexpr int MES2_POS_Y = 300;

    static constexpr int RANK_POS_X = Application::SCREEN_SIZE_X - ResourceManager::RANK_SIZE / 2;
    static constexpr int RANK_POS_Y = Application::SCREEN_SIZE_Y - ResourceManager::RANK_SIZE / 2;

    
    enum class RANK {
        C,
        B,
        A,
        S,
        MAX
    };

    FazeResult(void);
    ~FazeResult(void);

    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

    //終了しているか
    const bool IsEnd(void) { return isEnd_; };
    //リセット
    void Reset(void);

private:

    //仮置き
    char rankString_[RANK_MAX_NUM];

    //画像
    int imgBack_;
    int* imgRank_;

    //フォント
    int heasdFont_;
    int font_;

    //描画カウント
    float step_;

    float exp_; //基礎経験値
    float afterExp_;//計算後の経験値
    RANK rank_; //ランク
    bool isEnd_;//終了しているか
    
    //経験値の倍率を計算したものを取得
    float GetBonusExp(const RANK _rank)const;

    //ランクの変更(デバッグ用)
    void ChangeRank(void);
};

