#pragma once
#include<vector>
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

class Enemy : public UnitBase
{
public:

	//�A�j���[�V�����ԍ�
	static constexpr int ANIM_IDLE = 14;	//�ҋ@�A�j���[�V����
	static constexpr int ANIM_WALK = 93;	//�����A�j���[�V����
	static constexpr int ANIM_RUN = 54;		//����A�j���[�V����
	static constexpr int ANIM_DAMAGE = 39;	//�_���[�W�A�j���[�V����
	static constexpr int ANIM_DEATH = 71;	//����A�j���[�V����
	static constexpr int ANIM_ENTRY = 74;	//�o���A�j���[�V����

	static constexpr float ATK_CDT = 60.0f;		//�U���̃N�[���_�E���̎���
	static constexpr float ATK_BTC = 100.0f;	//�U���̋x�e����

	static constexpr float WALK_SPEED = 2.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,1.0f,60.0f,120.0f,0.0f };	//�X�L���P
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,5.0f,180.0f,300.0f,0.0f };	//�X�L���Q

	//���ݏ��
	enum class STATE
	{
		NORMAL			//�ʏ�
		,ALERT_ATTACK	//�U���O(�x��)
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

	//�ʒu
	const VECTOR GetPos(void)const override { return trans_.pos; }
	//�p�x
	const VECTOR GetRot(void)const override { return trans_.rot; }
	//�傫��
	const VECTOR GetScl(void)const override { return trans_.scl; }
	//�h���
	const float GetDef(void)const override { return def_; }

	//�U���N�[���_�E�������ǂ�����Ԃ�(true:�U���N�[���_�E����)
	const bool IsAtkCD(void)const { return atkCdt_ < ATK_CDT; }

	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//�擾�o���l��Ԃ�
	const float GetExp(void) { return exp_; }

	/// <summary>
	/// �_���[�W
	/// </summary>
	/// <param name="_damage">�_���[�W��</param>
	void Damage(const int _damage);

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeState(const STATE _state) { state_ = _state; }

protected:

	//�L�����ŗL�ݒ�
	virtual void SetParam(void);

	//�X�L���̏�����
	virtual void InitSkill(void);

private:
	
	STATE state_;	//���݂̏��

	float atkCdt_;				//�U���̃N�[���_�E���J�E���^
	float atkBtc_;				//�U���̋x�e���ԃJ�E���^

	std::vector<ATK> skill_;	//�X�L���̎��
	ATK nowSkill_;				//���݂̃X�L��

	float moveSpeed_;	//�ړ���

	int stunDefMax_;	//�C��h��l�̍ő�l
	int stunDef_;		//�C��h��l

	float exp_;		//�擾�o���l

	//�A�j���[�V�����ԍ��̏�����
	void InitAnimNum(void);

	//�X�V(�ʏ�)
	void UpdateNml(void);
	//�X�V(�U���x��)
	void UpdateAlert(void);
	//�X�V(�U��)
	void UpdateAtk(void);
	//�X�V(�x�e)
	void UpdateBreak(void);

	//���G
	void Search(void);

	//�ړ�
	void Move(void);
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="_moveSpeed">�ړ�����</param>
	/// <param name="_deg">�ړ�����(�f�O���[�p)</param>
	void ProcessMove(const float _moveSpeed, const float _deg);
	/// <summary>
	/// �����]��
	/// </summary>
	/// <param name="_deg">��]����(�f�O���[�p)</param>
	/// <param name="_axis">��]��</param>
	void Turn(const float _deg, const VECTOR _axis);

	//�G�̍U������
	void Attack(void);
	//�X�L���̃����_������
	void RandSkill(void);

};

