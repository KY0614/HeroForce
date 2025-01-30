#pragma once
#include <functional>
#include <vector>
#include "../Character/PlayableChara/PlayerBase.h"
#include "../../Common/Fader.h"

class LevelupNotice;
class LevelupSelect;
class Fader;

class LevelScreenManager
{
public:

	//��ԊǗ�
	enum class STATE
	{
		NONE,		//�Ȃ�
		NOTICE,		//�ʒm
		SELECT,		//�I��
		END			//�I��
	};	

	//�����v�f�̎��
	enum class TYPE
	{
		ATTACK,		//�U���͂̏㏸
		DEFENSE,	//�h��͂̏㏸
		SPEED,		//�ړ����x�̏㏸
		LIFE,		//�ő�̗͂̏㏸
	  /*TRAP_FLOOR,
		TRAP_CANNON,
		TRAP_POW_UP_F,
		TRAP_POW_UP_C,*/
		MAX
	};

	//�������x��
	static constexpr int DEFAULT_LEVEL = 1;
	
	//�Q�[�W�ő�l
	static constexpr float CONSTANT_GAGE = 1200.0f;

	//�Q�[�WUI�g�嗦
	static constexpr float GAGE_SCALE_RATE = Application::SCREEN_SIZE_X * 1.8f / Application::DEFA_SCREEN_SIZE_X;

	//�Q�[�W�摜�T�C�Y
	static constexpr int GAGE_IMG_SIZE = 128 * GAGE_SCALE_RATE;

	//�A���t�@�l�ő�
	static constexpr float ALPHA_MAX = 150.0f;

	//���߃X�s�[�h
	static constexpr float ALPHA_SPEED = 3.0f;

	//��ލő�
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//�G�t�F�N�g�T�C�Y
	static constexpr float EFFECT_SCALE = 30.0f;

	//�Q�[�W���W
	static constexpr int GAGE_POS_X = 20;
	static constexpr int GAGE_POS_Y = 20;

	LevelScreenManager(void);
	~LevelScreenManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//�ǂݍ���
	void Load(void);

	//����������
	void Reset();

	//�o���l�̑���
	void AddExp(const float value);

	//�Q�[�W�̐ݒ�
	void SetGage(const int level);

	//���ʔ��f
	void Reflection(PlayerBase &player,const int playerNum);

	//�X�e�[�g�̐ݒ�
	void ChangeState(const STATE state);
	
	//�o���l�̏�Ԋm�F
	void CheckExp();

	void EffectSyne(PlayerBase& player, const int playerNum);

	//�Q�b�^�[
	inline float GetExp(void)const { return exp_; };
	inline STATE GetState(void)const { return state_; };
	inline TYPE GetType(const int playerNum)const;
	TYPE GetPreType(const int playerNum)const;
	const bool IsLevelUp(void) { return !(state_ == STATE::NONE || state_ == STATE::END); }

private:

	//�摜
	int imgGage_;
	int imgGageExp_;
	int *imgNumbers_;

	//�v���C���[�l��
	int playerNum_;

	//���
	STATE state_;

	//���
	std::vector<TYPE> selectTypes_;

	//���݂̃��x��
	int nowLevel_;

	//�o���l
	float exp_;
	float restExp_;

	//�Q�[�W(���̃��x���܂ł̌o���l��)
	float gauge_;

	//�A���t�@�l
	float alpha_;

	//�t�F�[�h����
	bool isFader_;

	//�O���
	std::vector<TYPE> preTypeData_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// ��ԊǗ�
	std::function<void(void)> stateUpdate_;	//�X�V
	std::function<void(void)> stateDraw_;	//�`��

	//�C���X�^���X
	std::unique_ptr<LevelupNotice> notice_;
	std::unique_ptr<LevelupSelect> select_;
	std::unique_ptr<Fader> fader_;

	//��ԕύX
	void ChangeStateNone();
	void ChangeStateNotice();
	void ChangeStateSelect();
	void ChangeStateEnd();

	//�e��X�V����
	void UpdateNone(void);
	void UpdateNotice(void);		//�ʒm
	void UpdateSelect(void);		//�����I��
	void UpdateEnd(void);			//�I��

	//�e��`�揈��
	void DrawNone();
	void DrawNotice();
	void DrawSelect();
	void DrawEnd();
						
	//UI�`��
	void DrawLevelUI();

	//�Ó]
	void FaderDraw();

	//�t�F�[�h�̐؂�ւ�����
	void Fade();

	//�f�o�b�O�@�\
	void DebagUpdate();
	void DebagDraw();
};