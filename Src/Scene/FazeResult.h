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
    
    enum class RANK {
        S,
        A,
        B,
        C,
        MAX
    };

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

    float exp_; //基礎経験値
    float afterExp_;//計算後の経験値
    RANK rank_; //ランク
    bool isEnd_;//終了しているか
    
    //経験値の倍率を計算したものを取得
    float GetBonusExp(const RANK _rank)const;

    //ランクの変更(デバッグ用)
    void ChangeRank(void);
};

