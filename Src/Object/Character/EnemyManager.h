#pragma once

#include<vector>
#include<memory>

class Enemy;

class EnemyManager
{
public:
	//���ނ̓G�̍ő吔
	static constexpr int ONETYPE_MAX = 5;
	//�G�̏o���ő吔
	static constexpr int ENEMY_MAX = 8;

	//�t�F�[�Y���Ƃ̏���������
	
	//�t�F�[�Y1�̓G�̏���������
	static constexpr int PHASE_ONE_INIT_CREATE_ENEMY = 4;
	//�t�F�[�Y2�̓G�̏���������
	static constexpr int PHASE_TWO_INIT_CREATE_ENEMY = 6;

	//�G�̏o�����a
	static constexpr float GENELATE_RADIUS = 100.0f;

	//�G�Ԃ̋���
	static constexpr float ENEMY_DISTANCE = 30.0f;

	//�G�̐����Ԋu
	static constexpr float CREATE_INTERVAL = 5.0f;

	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	//�R���X�g���N�^
	EnemyManager(std::vector<VECTOR> _pos) : createPos_(_pos) {};

	//�f�X�g���N�^
	~EnemyManager() = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//�Փ˔���
	void CollisionStage(const Transform& stageTrans);

	/// <summary>
	/// �w�肵���G�̎擾
	/// </summary>
	/// <param name="_num">�~�����G�̔z��ԍ�</param>
	/// <returns>�G�̃C���X�^���X</returns>
	Enemy* GetActiveEnemy(int _num){ return activeEnemys_[_num]; }

	//�G�̑����̎擾
	const int GetActiveNum(void) { return activeNum_; }

	//�|���ꂽ�G�̌㏈�� _num=�|���ꂽ�G�̔z��ԍ�
	void DeathEnemy(int _num);

	//�t�F�[�Y�ύX
	void ProcessChangePhase(const int _phase);

private:

	//�X�V���������铮���̂���G
	Enemy* activeEnemys_[ENEMY_MAX];

	//�A�N�e�B�u�ȓG�̑���
	int activeNum_;

	//�������W
	std::vector<VECTOR> createPos_;

	//�����Ԋu
	float createIntCnt_;
	
	//�����̓G���A�N�e�B�u��Ԃ�
	void CreateEnemy(void);

	//�G�����ׂč폜����
	void DeleteAllEnemy(void);

	//�{�X���쐬
	void CreateBoss(void);

	//�d�Ȃ��Ă��Ȃ����W�����
	VECTOR GetNotOverlappingPos(void);

	/// <summary>
	/// �~�͈͂̃����_����_�����
	/// </summary>
	/// <param name="_myPos">�o���\����W</param>
	/// <param name="_r">�o���͈�</param>
	/// <param name="_tPos">�o����������W</param>
	void GetRandomPointInCircle(VECTOR _myPos, const int _r, VECTOR& _tPos);

	/// <summary>
	/// �G�B���d�Ȃ��Ă��Ȃ���
	/// </summary>
	/// <param name="_thisEnemy">��r�������G</param>
	/// <param name="_minDist">�G�̒��a</param>
	/// <returns>�G���d�Ȃ�����(true:�d�Ȃ���)</returns>
	bool IsOverlap(VECTOR& _tPos, float _minDist);
};

