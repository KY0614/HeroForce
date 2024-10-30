#pragma once
#include "PlayerBase.h"
class PlayerCpu :
    public PlayerBase
{
public:
    static constexpr float SEARCH_RADIUS = 400.0f;
    void SetParam(void)override;
    void Update(void)override;

protected:
    void Search(void);
    PlayerBase* player_;

    //ŒŸ’mƒtƒ‰ƒO
    bool isNotice_;

};

