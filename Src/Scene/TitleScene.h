#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SpaceDome;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �^�C�g�����S
	int imgTitleLogo_;

	// �X�J�C�h�[���p�̃_�~�[
	Transform spaceDomeTran_;

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// ���S�`��
	void DrawLogo(void);

};
