#pragma once
#include "SceneBase.h"
class FazeResult :
    public SceneBase
{
public:
    static constexpr float BNS_RANK_S = 2.0f;
    static constexpr float BNS_RANK_A = 1.5f;
    static constexpr float BNS_RANK_B = 1.3f;
    static constexpr float BNS_RANK_C = 1.0f;
    
    enum class RANK {
        NONE,
        S,
        A,
        B,
        C
    };

    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

private:

    float exp_; //Šî‘bŒoŒ±’l
    
    //ŒoŒ±’l‚Ì”{—¦‚ðŒvŽZ‚µ‚½‚à‚Ì‚ðŽæ“¾
    float GetBonusExp(const RANK _rank)const;
};

