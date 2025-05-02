#pragma once
#include <assert.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../Common/Transform.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../../../../Manager/GameSystem/DataBank.h"
#include "../PlayerBase.h"
#include "../../../UnitBase.h"

class OverPlayers : public UnitBase
{
public:

	//�A�j���[�V�����X�s�[�h
	static constexpr float ANIM_SPEED = 20.0f;

	//�A�C�h���A�j���ԍ�
	static constexpr int IDLE_ANIM = 24;
	static constexpr int DEATH_ANIM = 23;

	//�X�P�[��
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	//�ʒu
	static constexpr VECTOR PLAYER_POS_1 = { -240.0f, -40.0f, -50.0f };
	static constexpr VECTOR PLAYER_POS_2 = { -50.0f, -50.0f, -10.0f };
	static constexpr VECTOR PLAYER_POS_3 = { 50.0f, -50.0f, -10.0f };
	static constexpr VECTOR PLAYER_POS_4 = { 240.0f, -50.0f, -50.0f };

	//����
	static constexpr float PLAYER_ROT_1 = -30.0f;
	static constexpr float PLAYER_ROT_2 = -15.0f;
	static constexpr float PLAYER_ROT_3 = 15.0f;
	static constexpr float PLAYER_ROT_4 = 30.0f;

	//�v���C���[��
	static constexpr int PLAYERS = SceneManager::PLAYER_NUM;

	//�R���X�g���N�^
	OverPlayers(void);

	//�f�X�g���N�^
	~OverPlayers(void) = default;

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//���W�̐ݒ�
	void SetPos(VECTOR pos);

	//��]�̐ݒ�
	void SetRot(Quaternion quo) { for (auto& tran : trans_) { tran.quaRotLocal = quo; } };

	//���W��n��
	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };

	//�z��̍��W��n��
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };

private:

	//�L�������Ƃ̃g�����X�t�H�[��
	Transform trans_[PLAYERS];

	//��E
	SceneManager::ROLE role_[PLAYERS];

	//3D���f��������
	void Init3DModel(void);

	//�A�j���[�V��������
	void Animation();

	//�A�j���[�V�����̏�����
	void ResetAnim(const ANIM _anim, const float _speed);

	void FinishAnim()override;
};

