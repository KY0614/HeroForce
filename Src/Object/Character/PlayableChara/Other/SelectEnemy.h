#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Object/UnitBase.h"

class SelectEnemy : public UnitBase
{
public:
	static constexpr float ANIM_SPEED = 20.0f;

	static constexpr int IDLE_ANIM = 41;
	static constexpr int SPAWN_ANIM = 74;

	//�R���X�g���N�^
	SelectEnemy(void);

	//�f�X�g���N�^
	~SelectEnemy(void) = default;

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
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(VECTOR pos) { for (auto& tran : transArray_) { tran.pos = pos; } };
	void SetPosArray(VECTOR pos, int i) { transArray_[i].pos = pos; };

	/// <summary>
	/// �p�x(����)��ݒ肷��
	/// </summary>
	/// <param name="quo">�p�x</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };

private:

	//�A�j���[�V�����ύX����
	float animChangeTime_[SceneManager::PLAYER_NUM];
	bool isSpawn_[SceneManager::PLAYER_NUM];

	void Init3DModel(void);
};

