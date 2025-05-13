#pragma once

#include<vector>
#include<memory>

class Enemy;

class EnemyManager
{
public:

	//****************************************************************
	//�萔
	//****************************************************************
	
	//�G�̐�
	static constexpr int ONETYPE_MAX = 3;	//���ނ̓G�̍ő吔
	static constexpr int ENEMY_MAX = 8;		//�G�̏o���ő吔

	//�t�F�[�Y
	static constexpr int PHASE_FIRST = 1;					//�ŏ��̃t�F�[�Y
	static constexpr int PHASE_ONE_INIT_CREATE_ENEMY = 4;	//�t�F�[�Y1�̓G�̏���������
	static constexpr int PHASE_TWO_INIT_CREATE_ENEMY = 6;	//�t�F�[�Y2�̓G�̏���������

	//�G�̏o���֌W
	static constexpr float GENELATE_RADIUS = 300.0f;			//�G�̏o�����a
	static constexpr float ENEMY_DISTANCE = 20.0f;				//�G�Ԃ̋���
	static constexpr float CREATE_INTERVAL = 5.0f;				//�G�̐����Ԋu

	//���[�v�̐���
	static constexpr int RETRY_LIMIT = 200;	//���[�v�񐔏��

	//****************************************************************
	//�񋓌^
	//****************************************************************

	//�G�̎��
	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	//****************************************************************
	//�����o�֐�
	//****************************************************************

	//�R���X�g���N�^
	EnemyManager(std::vector<VECTOR> _pos);

	//�f�X�g���N�^
	~EnemyManager() = default;

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);
	//���
	void Release(void);

	//�Փ˔���
	void CollisionStage(const Transform& stageTrans);

	/// <summary>
	/// �w�肵���G�̎擾
	/// </summary>
	/// <param name="_num">�~�����G�̔z��ԍ�</param>
	/// <returns>�G�̃C���X�^���X</returns>
	Enemy& GetActiveEnemy(int _num)const { return *activeEnemys_[_num]; }

	//���o���l�ʂ̎擾
	const float GetAllExp(void)const { return allExp_; }

	//���o���l���폜
	void ClearAllExp(void) { allExp_ = 0.0f; }

	//�G�̑����̎擾
	const int GetActiveNum(void)const { return activeNum_; }

	//�|���ꂽ�G�̌㏈�� _num=�|���ꂽ�G�̔z��ԍ�
	void DeathEnemy(int _num);

	//�G�̓|���ꂽ����Ԃ�
	const int GetDunkCnt(void)const { return dunkCnt_; }

	//�t�F�[�Y�ύX
	void ProcessChangePhase(const int _phase);

private:
	//****************************************************************
	//�����o�ϐ�
	//****************************************************************
	
	std::unique_ptr<Enemy> activeEnemys_[ENEMY_MAX];	//�����Ă���G
	TYPE activeEnemysType_[ENEMY_MAX];					//�����Ă���G�̎��

	int activeNum_;										//�A�N�e�B�u�ȓG�̑���
	int activeTypeNum_[static_cast<int>(TYPE::MAX)];	//��ޕʂ̓G�̗�

	float allExp_;	//���o���l��
	int dunkCnt_;	//�|�����G�̑���(�t�F�[�Y���ƂɃ��Z�b�g)

	std::vector<VECTOR> createPos_;	//�������W
	float createIntCnt_;			//�����Ԋu
	
	//****************************************************************
	//�����o�֐�
	//****************************************************************

	//�����̓G���A�N�e�B�u��Ԃ�
	void CreateEnemy(void);

	//�G�����ׂč폜����
	void DeleteAllEnemy(void);

	//�{�X���쐬
	void CreateBoss(void);
};

