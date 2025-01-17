#pragma once
#include<vector>
#include <functional>
#include"../../Utility/AsoUtility.h"
#include "../UnitBase.h"

class Enemy : public UnitBase
{
public:

#define DEBUG_ENEMY

	//�����e�X�g

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

	//�G�̃X�L���s��
	enum class ATK_ACT
	{
		SKILL_ONE
		,SKILL_TWO
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

	std::function<void(void)> stateUpdate_;						//��Ԃ��Ƃ̍X�V�Ǘ�
	std::map<STATE,std::function<void(void)>> stateChanges_;	//��ԑJ�ڂ̊Ǘ�

	std::map<ANIM, float>changeSpeedAnim_;	//�A�j���[�V�������x�ύX�p

	float alertCnt_;			//�U���̌x�����ԃJ�E���^
	float breakCnt_;			//�U���̋x�e���ԃJ�E���^

	float walkSpeed_;		//�G���Ƃ̕������x
	float runSpeed_;		//�G���Ƃ̑��鑬�x

	std::map<ATK_ACT, ATK> skills_;								//�X�L���̎��
	std::vector<ATK> nowSkill_;									//���݂̃X�L��
	std::function<ATK&(void)> createSkill_;						//�X�L���̐���
	std::function<void(void)> processSkill_;					//�X�L���̏���
	std::map<ATK_ACT, std::function<void(void)>> changeSkill_;	//�X�L���̕ύX�p

	std::vector<ANIM> skillPreAnims_;	//�X�L���ɑΉ������\���A�j���[�V����
	std::vector<ANIM> skillAnims_;		//�X�L���ɑΉ������A�j���[�V����
	ANIM nowSkillPreAnim_;				//���݂̃X�L���̗\���A�j���[�V����
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

	//�A�j���[�V�����֌W�̏�����
	virtual void InitAnim(void);

	//�X�L���̏�����
	virtual void InitSkill(void) = 0;

	//�G�̍U������
	virtual void Attack(void);

	//�X�L��1
	virtual void Skill_One(void) = 0;

	//�X�L���̃����_������
	virtual void RandSkill(void);

	/// <summary>
	/// �X�L����������
	/// </summary>
	/// <param name="_atkAct">��������X�L��</param>
	void SetUpSkill(ATK_ACT _atkAct);

	/// <summary>
	/// �X�L���̐���
	/// </summary>
	/// <param name="_atkAct">��������X�L��</param>
	/// <returns>���������X�L��</returns>
	ATK& CreateSkill(ATK_ACT _atkAct);

	//�A�j���[�V�����I�����̓���
	virtual void FinishAnim(void)override;

	//��ԑJ��(�ʏ�)
	void ChangeStateNormal(void);
	//��ԑJ��(�U���x��)
	void ChangeStateAlert(void);
	//��ԑJ��(�U��)
	virtual void ChangeStateAtk(void);
	//��ԑJ��(�x�e)
	virtual void ChangeStateBreak(void);

	//�X�V(�ʏ�)
	void UpdateNml(void);
	//�X�V(�U���x��)
	void UpdateAlert(void);
	//�X�V(�U��)
	virtual void UpdateAtk(void);
	//�X�V(�x�e)
	virtual void UpdateBreak(void);

	//�`��(���f�o�b�O)
	virtual void DrawDebug(void);

	/// <summary>
	/// �W�I�܂ł̈ړ��x�N�g����Ԃ�
	/// </summary>
	/// <param name="_speed">�ݒ葬�x(���ݒ肾�ƁA�����x�N�g���݂̂�Ԃ�)</param>
	/// <returns>�W�I�ւ̈ړ�(����)�x�N�g��</returns>
	const VECTOR GetTargetVec(const float _speed = 1.0f)const;
	
	/// <summary>
	/// �W�I�܂ł̈ړ��x�N�g����Ԃ�
	/// </summary>
	/// <param name="_pos">�_�����̍��W</param>
	/// <param name="_speed">�ݒ葬�x(���ݒ肾�ƁA�����x�N�g���݂̂�Ԃ�)</param>
	/// <returns>�W�I�ւ̈ړ�(����)�x�N�g��</returns>
	const VECTOR GetTargetVec(const VECTOR _pos, const float _speed = 1.0f)const;

	//�ړ�
	void Move(void);
};

