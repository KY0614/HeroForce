#pragma once
#include "SceneBase.h"
class FazeResult :
    public SceneBase
{
public:

    //�����N�̎�ސ�
    static constexpr int RANK_MAX_NUM = 4;
    //�����N�̊e��{��
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

    //�I�����Ă��邩
    const bool IsEnd(void) { return isEnd_; };
    //���Z�b�g
    void Reset(void);

private:

    //���u��
    char rankString_[RANK_MAX_NUM];

    float exp_; //��b�o���l
    float afterExp_;//�v�Z��̌o���l
    RANK rank_; //�����N
    bool isEnd_;//�I�����Ă��邩
    
    //�o���l�̔{�����v�Z�������̂��擾
    float GetBonusExp(const RANK _rank)const;

    //�����N�̕ύX(�f�o�b�O�p)
    void ChangeRank(void);
};

