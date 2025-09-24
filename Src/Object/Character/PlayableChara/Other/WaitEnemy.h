#pragma once
#include "../Object/UnitBase.h"

class WaitEnemy : public UnitBase
{
public:
	static constexpr float ANIM_SPEED = 20.0f;	//�A�j���[�V�������x

	static constexpr int IDLE_ANIM = 41;		//IDLE�A�j���[�V�����ԍ�
	static constexpr int SPAWN_ANIM = 74;		//SPAWN�A�j���[�V�����ԍ�

	//�R���X�g���N�^
	WaitEnemy(void);

	//�f�X�g���N�^
	~WaitEnemy(void) = default;

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
	/// �z��̗v�f���w�肵�A���W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="index">�w�肷��v�f�ԍ�</param>
	void SetPosAtIndex(VECTOR pos, int index) { transArray_[index].pos = pos; };

	/// <summary>
	/// �p�x(����)��ݒ肷��
	/// </summary>
	/// <param name="quo">�p�x</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <param name="index">�z��̗v�f�̎w��</param>
	/// <returns>�w�肵���z��v�f�̍��W</returns>
	VECTOR GetPosAtIndex(int index) const{ return transArray_[index].pos; };

private:

	//�A�j���[�V�����ύX����
	float animChangeTime_[SceneManager::PLAYER_NUM];

	//�X�|�[�������ǂ��� true:�X�|�[���� false:�܂��X�|�[�����Ă��Ȃ�
	bool isSpawn_[SceneManager::PLAYER_NUM];

	/// <summary>
	/// 3D���f���̏���������
	/// </summary>
	void Init3DModel(void);
};

