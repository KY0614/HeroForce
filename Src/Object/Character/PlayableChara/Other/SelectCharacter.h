#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class SelectCharacter : public UnitBase
{
public:

	//�L�����N�^�[�֘A
	static constexpr float ROT_SPEED = 0.5f;			//�L�����N�^�[�̉�]���x

	//�L�����N�^�[�A�j���[�V�����֘A
	static constexpr float ANIM_SPEED = 20.0f;			//�L�����N�^�[�̃A�j���[�V�������x

	static constexpr float CHICKEN_SPEED = 40.0f;		//�`�L���̃A�j���[�V�������x

	static constexpr int IDLE_ANIM = 36;				//IDLE�A�j���[�V�����ԍ�

	static constexpr int KNIGHT_ANIM = 1;				//�R�m�p�A�j���[�V�����ԍ�

	static constexpr int AXE_ANIM = 2;					//���g���p�A�j���[�V�����ԍ�

	static constexpr int MAGE_ANIM = 61;				//���@�g���A�j���[�V�����ԍ�

	static constexpr int ARCHER_ANIM = 6;				//�|�g���A�j���[�V�����ԍ�

	static constexpr int CHICKEN_SWINGHAND_ANIM = 6;	//�`�L���p�A�j���[�V�����ԍ�

	//�R���X�g���N�^
	SelectCharacter(void);

	//�f�X�g���N�^
	~SelectCharacter(void) = default;

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

	/// <summary>
	/// Transform�z��̐擪�̍��W���擾(���̍��W�������Ȃ̂�)
	/// </summary>
	/// <returns>Transform�z��̐擪�̍��W</returns>
	VECTOR GetFrontPos(void)const { return transArray_[0].pos; }

	/// <summary>
	/// �`�L���̍��W���擾
	/// </summary>
	/// <returns>�`�L���̍��W</returns>
	VECTOR GetChickenPos(void)const { return trans_.pos; };

	//�Z�b�^�[(�ݒ�p�֐�)---------------------------------------------------

	/// <summary>
	/// ��E��ݒ肷��
	/// </summary>
	/// <param name="role">��E</param>
	void SetRole(int role){ role_ = role - 1; };

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(VECTOR pos);

	/// <summary>
	/// ���W��ݒ肷��(�`�L���p)
	/// </summary>
	/// <param name="pos">���W</param>
	void SetChickenPos(VECTOR pos){trans_.pos = pos;};

	/// <summary>
	/// �p�x(����)��ݒ肷��
	/// </summary>
	/// <param name="quo">�p�x</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	/// <summary>
	/// �p�x(����)��ݒ肷��(�`�L���p)
	/// </summary>
	/// <param name="quo">�p�x</param>
	void SetRotChicken(Quaternion quo) { trans_.quaRotLocal = quo;  };

	/// <summary>
	/// ��莞�Ԃ��ƂɃA�j���[�V������ς���
	/// </summary>
	void UpdateAnimCycle(void);

private:

	//��E
	int role_;

	//�A�j���[�V�����ύX����(transArray�ɍ��킹�Ĕz��)
	float animChangeTime_[SceneManager::PLAYER_NUM];

	/// <summary>
	/// 3D���f���̏���������
	/// </summary>
	void Init3DModel(void);
};