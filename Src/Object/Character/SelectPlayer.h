#pragma once
#include"../../Utility/AsoUtility.h"
#include"../Common/Transform.h"
#include"../Manager/SceneManager.h"
#include "../UnitBase.h"

class SelectPlayer : public UnitBase
{
public:
	//�R���X�g���N�^
	SelectPlayer(void);

	//�f�X�g���N�^
	~SelectPlayer(void) = default;

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

	void SetRole(int role);

private:

	Transform trans_[SceneManager::PLAYER_NUM];
	int role;

	void Init3DModel(void);
	void UpdateTrans(void);
};

