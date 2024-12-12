#pragma once
#include<DxLib.h>
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
	void Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size);

	/// <summary>
	/// �G�t�F�N�g�̍Đ���~
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
	void Stop(const EFFECT& _efc);

	//�������
	void Release(void);

private:
	std::unordered_map<EFFECT,int> effectRes_;
};

