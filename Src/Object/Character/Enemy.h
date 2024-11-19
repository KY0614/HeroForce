#pragma once
#include<vector>
#include"../../Utility/AsoUtility.h"
#include "../UnitBase.h"

class Enemy : public UnitBase
{
public:

#define DEBUG_ENEMY

	//****************************************************************
	//�萔(�L��������)
	//****************************************************************

	//�A�j���[�V�����ԍ�
	static constexpr int ANIM_IDLE = 14;	//�ҋ@�A�j���[�V����
	static constexpr int ANIM_WALK = 93;	//�����A�j���[�V����
	static constexpr int ANIM_RUN = 54;		//����A�j���[�V����
	static constexpr int ANIM_DAMAGE = 39;	//�_���[�W�A�j���[�V����
	static constexpr int ANIM_DEATH = 24;	//����A�j���[�V����
	static constexpr int ANIM_ENTRY = 74;	//�o���A�j���[�V����

	//****************************************************************
	//�񋓌^
	//****************************************************************

	//���ݏ��
	enum class STATE
	{
		NORMAL			//�ʏ�
		,ALERT			//�U���O(�x��)
		,ATTACK			//�U��
		,BREAK			//�x�e
		,MAX
	};

	//�����̎�ނ̃��x�������@����邩���I�I

	//****************************************************************
	//�����o�֐�
	//****************************************************************

	//�R���X�g���N�^
	Enemy() = default;
	//�f�X�g���N�^
	~Enemy() = default;

	//���
	void Destroy(void)override;

	//������
	void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�(�������z�֐�)
	virtual const bool IsAlertTime(void)const = 0;
	//�x�e���Ԓ����ǂ�����Ԃ�(�������z�֐�)
	virtual const bool IsBreak(void)const = 0;

	//�X�^�������ǂ�����Ԃ�
	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//���݂̃X�L���̑S�z���Ԃ�
	const std::vector<ATK> GetAtks(void)const { return nowSkill_; }

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
	/// �W�I�̍��W��ύX
	/// </summary>
	/// <param name="_targetPos">�W�I�̍��W</param>
	void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

	//�W�I�̕����Ɍ���
	void LookTargetVec(void);

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

	//****************************************************************
	//�����o�ϐ�
	//****************************************************************

	STATE state_;	//���݂̏��

	float alertCnt_;			//�U���̌x�����ԃJ�E���^
	float breakCnt_;			//�U���̋x�e���ԃJ�E���^

	float walkSpeed_;		//�G���Ƃ̕������x
	float runSpeed_;		//�G���Ƃ̑��鑬�x

	std::vector<ATK> skills_;			//�X�L���̎��
	std::vector<ATK> nowSkill_;			//���݂̃X�L��

	std::vector<ANIM> skillAnims_;		//�X�L���ɑΉ������A�j���[�V����
	ANIM nowSkillAnim_;					//���݂̃X�L���A�j���[�V����

	VECTOR localCenterPos_;	//�G�����̑��΍��W
	VECTOR colPos_;			//�G���g�̓����蔻��p�̑��΍��W

	float moveSpeed_;		//�ړ���
	bool isMove_;			//�ړ����Ă��邩�ǂ���(true:�ړ���)

	VECTOR targetPos_;		//�W�I�̍��W

	float searchRange_;		//���G�͈�
	float atkStartRange_;	//�U���J�n�͈�

	int stunDefMax_;	//�C��h��l�̍ő�l
	int stunDef_;		//�C��h��l

	//****************************************************************
	//�����o�֐�
	//****************************************************************

	//�L�����ŗL�ݒ�
	virtual void SetParam(void) = 0;

	//�A�j���[�V�����ԍ��̏�����
	virtual void InitAnimNum(void);

	//�X�L���̏�����
	virtual void InitSkill(void) = 0;

	//�G�̍U������
	virtual void Attack(void) = 0;

	//�X�L��1
	virtual void Skill_One(void);

	//�X�L���̃����_������
	virtual void RandSkill(void);

	//�A�j���[�V�����I�����̓���
	virtual void FinishAnim(void)override;

	//��ԑJ�ڂɂ����鏉��������
	virtual void InitChangeState(void);

	//�X�V(�ʏ�)
	void UpdateNml(void);
	//�X�V(�U���x��)
	void UpdateAlert(void);
	//�X�V(�U��)
	void UpdateAtk(void);
	//�X�V(�x�e)
	virtual void UpdateBreak(void);

	/// <summary>
	/// �W�I�܂ł̃x�N�g�����x��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>�W�I�ւ̕����x�N�g��</returns>
	const VECTOR GetMovePow2Target(void)const;

	//�ړ�
	void Move(void);
};

