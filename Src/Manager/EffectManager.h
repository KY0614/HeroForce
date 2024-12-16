#pragma once
#include<DxLib.h>
#include"SoundManager.h"
#include"../Common/Quaternion.h"
#include<unordered_map>
#include<string>
class EffectManager
{
public:
	enum class EFFECT
	{
		//�g�p����G�t�F�N�g�𗅗�
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static EffectManager& GetInstance(void);

	/// <summary>
	/// �G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
	/// <param name="_data">�G�t�F�N�g�̃f�[�^</param>
	void Add(const EFFECT& _efc,int _data);

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
	/// <param name="_pos">�Đ��ʒu</param>
	/// <param name="_qua">�p�x</param>
	/// <param name="_size">�傫��</param>
	/// <param name="_sound">���ʉ�</param>
	void Play(const EFFECT& _efc,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size,
		const SoundManager::SOUND _sound = SoundManager::SOUND::NONE);

	/// <summary>
	/// �G�t�F�N�g�̍Đ���~
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
	void Stop(const EFFECT& _efc);

	//�������
	void Release(void);

private:
	//�C���X�^���X�p
	static EffectManager* instance_;

	//�G�t�F�N�g�f�[�^�i�[�p
	std::unordered_map<EFFECT,int> effectRes_;	//�����f�[�^
	std::unordered_map<EFFECT,int> effectPlay_;	//�Đ��f�[�^

	//�R���X�g���N�^���f�X�g���N�^
	EffectManager() = default;
	~EffectManager() = default;
};

