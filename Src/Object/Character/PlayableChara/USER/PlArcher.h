#pragma once
#include"../../../JobManagerBase.h"

class PlArcher :public JobManagerBase
{
public:
	//�萔
	static constexpr float SKILL_ONE_START_NOCHARGE = 0.5f;

	//�G�t�F�N�g���Đ����邽�߂Ƀf���^�^�C����萔�ɂ���
	static constexpr float DELTA_TIME = 1.0f / 60.0f;

	PlArcher(const SceneManager::CNTL _cntl);
	PlArcher(const InputManager::JOYPAD_NO _padNum);
	~PlArcher(void) = default;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	//�U������
	void AtkInput(void)override;

	//�X�L��1����
	void SkillOneInput(void)override;

	//�X�L��2����
	void SkillTwoInput(void)override;

	//�ʏ�U��������
	void AtkInit(void);

	//�X�L��1������
	void SkillOneInit(void);

	//�X�L��2������
	void SkillTwoInit(void);
};
