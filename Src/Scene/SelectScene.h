#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"


class SelectScene :public SceneBase
{
public:

	//�I�����Ă�����
	enum class SELECT 
	{
		NUMBER,		//�l��
		ROLE		//��E
	};

	enum class CONTROL
	{
		KEY,
		PAD
	};

	// �L�[�R���t�B�O
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE
	};

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	//�l���I�𒆂̏���
	void NumberUpdate(void);

	//��E�I�𒆂̏���
	void RoleUpdate(void);

	//�f�o�b�O�`��
	void DrawDebug(void);

	//�I��������̂̎�ނ�ς���
	void ChangeSelect(SELECT select);

	void ProcessSelect(void);

	void KeyKonfigSetting(void);

	void Move(VECTOR pos);

private:

	SELECT select_;

	CONTROL ctrl_;

	KEY_CONFIG key;

	VECTOR pos_;
};

