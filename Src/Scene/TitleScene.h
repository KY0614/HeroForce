#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SkyDome;
class StageManager;

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

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	// �^�C�g�����S
	int imgTitleLogo_;
	//�X�^�[�g
	int imgStart_;

	// �X�J�C�h�[���p�̃_�~�[
	Transform spaceDomeTran_;


	// ���S�`��
	void DrawLogo(void);

};
