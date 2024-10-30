#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SkyDome();
	//SkyDome(const Transform& syncTransform);

	// �f�X�g���N�^
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const Transform& GetTransform() const;

private:

	// ���f������̊�{���
	Transform transform_;

	// ���@�̏��
	//const Transform& syncTransform_;

	// ���
	STATE state_;
	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);
	void ChangeStateFollow(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateStay(void);
	void UpdateFollow(void);

};

