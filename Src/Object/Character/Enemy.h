#pragma once
#include<vector>
#include"../../Utility/AsoUtility.h"
#include "../UnitBase.h"

class Enemy : public UnitBase
{
public:

#define DEBUG_ENEMY

	//�A�j���[�V�����ԍ�
	static constexpr int ANIM_IDLE = 14;	//�ҋ@�A�j���[�V����
	static constexpr int ANIM_WALK = 93;	//�����A�j���[�V����
	static constexpr int ANIM_RUN = 54;		//����A�j���[�V����
	static constexpr int ANIM_SKILL_1 = 9;	//�X�L��1�A�j���[�V��������
	static constexpr int ANIM_SKILL_2 = 11;	//�X�L��2�A�j���[�V��������
	static constexpr int ANIM_DAMAGE = 39;	//�_���[�W�A�j���[�V����
	static constexpr int ANIM_DEATH = 24;	//����A�j���[�V����
	static constexpr int ANIM_ENTRY = 74;	//�o���A�j���[�V����

	//�A�j���[�V�������x
	static constexpr float DEFAULT_SPEED_ANIM = 20.0f;	//�f�t�H���g�̃A�j���[�V�������x

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W
	
	//�G���g�̓����蔻�蔼�a
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//�U���֌W
	static constexpr float ALERT_TIME = 120.0f;	//�U���̌x������
	static constexpr float BREAK_TIME = 100.0f;	//�U���̋x�e����

	//���x�֌W
	static constexpr float WALK_SPEED = 2.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_1_COL_RADIUS = 10.0f;	//�X�L���P
	static constexpr float SKILL_2_COL_RADIUS = 24.0f;	//�X�L���Q

	//�X�L���֌W
	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,SKILL_1_COL_RADIUS,1.0f,60.0f,120.0f,0.0f };	//�X�L���P
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,SKILL_2_COL_RADIUS,5.0f,180.0f,300.0f,0.0f };	//�X�L���Q

	//���ݏ��
	enum class STATE
	{
		NORMAL			//�ʏ�
		,ALERT			//�U���O(�x��)
		,ATTACK			//�U��
		,BREAK			//�x�e
		,MAX
	};

	//�U���̎��
	//enum class ATK_PAT
	//{

	//};

	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy() = default;

	//���
	void Destroy(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�
	const bool IsAlertTime(void)const { return alertCnt_ < ALERT_TIME; }
	//�x�e���Ԓ����ǂ�����Ԃ�
	const bool IsBreak(void)const { return breakCnt_ < BREAK_TIME; }
	//�X�^�������ǂ�����Ԃ�
	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//�G���g�̓����蔻����W��Ԃ�
	//const VECTOR GetColPos(void)const { return colPos_; }

	//���݂̃X�L���̍��W��Ԃ�
	//const VECTOR GetNowSkillPos(void)const { return nowSkill_.pos_; }

	//���݂̃X�L���̑S�z���Ԃ�
	const std::vector<ATK> GetAtks(void)const { return nowSkill_; }

	//�擾�o���l��Ԃ�
	const float GetExp(void)const { return exp_; }

	//���G�͈͂�Ԃ�
	const float GetSearchRange(void) { return searchRange_; }

	//�U���J�n�͈͂�Ԃ�
	const float GetAtkStartRange(void) { return atkStartRange_; }

	/// <summary>
	/// �ړ���Ԃ�ύX
	/// </summary>
	/// <param name="_isMove">�ړ�����ǂ���(true:�ړ�����)</param>
	void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

	/// <summary>
	/// �_���[�W
	/// </summary>
	/// <param name="_damage">�_���[�W��</param>
	/// <param name="_stunPow">�X�^���U����</param>
	void Damage(const int _damage, const int _stunPow);

	//���ݏ�Ԃ�Ԃ�
	const STATE GetState(void) { return state_; }

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeState(const STATE _state);

protected:

	//�L�����ŗL�ݒ�
	virtual void SetParam(void);

	//�A�j���[�V�����ԍ��̏�����
	virtual void InitAnimNum(void);

	//�X�L���̏�����
	virtual void InitSkill(void);

	//�A�j���[�V�����I�����̓���
	void FinishAnim(void)override;

private:
	
	STATE state_;	//���݂̏��

	float alertCnt_;			//�U���̌x�����ԃJ�E���^
	float breakCnt_;			//�U���̋x�e���ԃJ�E���^

	std::vector<ATK> skills_;			//�X�L���̎��
	std::vector<ATK> nowSkill_;			//���݂̃X�L��
	
	std::vector<ANIM> skillAnims_;		//�X�L���ɑΉ������A�j���[�V����
	ANIM nowSkillAnim_;					//���݂̃X�L���A�j���[�V����
		
	VECTOR colPos_;			//�G���g�̓����蔻��p�̑��΍��W

	float searchRange_;		//���G�͈�
	float atkStartRange_;	//�U���J�n�͈�
	float moveSpeed_;		//�ړ���
	bool isMove_;			//�ړ����Ă��邩�ǂ���(true:�ړ���)

	int stunDefMax_;	//�C��h��l�̍ő�l
	int stunDef_;		//�C��h��l

	float exp_;		//�擾�o���l

	VECTOR targetPos_;	//���f�o�b�O�@�W�I�̍��W

	//�X�V(�ʏ�)
	void UpdateNml(void);
	//�X�V(�U���x��)
	void UpdateAlert(void);
	//�X�V(�U��)
	void UpdateAtk(void);
	//�X�V(�x�e)
	void UpdateBreak(void);

	/// <summary>
	/// �W�I�̃x�N�g����Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>�W�I�ւ̕����x�N�g��</returns>
	const VECTOR GetTargetVec(void)const;

	//�ړ�
	void Move(void);

	//�G�̍U������
	void Attack(void);
	//�X�L���̃����_������
	void RandSkill(void);
};
