#include "EffectManager.h"

/// <summary>
/// �G�t�F�N�g�̒ǉ�
/// </summary>
/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
/// <param name="_data">�G�t�F�N�g�̃f�[�^</param>
void EffectManager::Add(const EFFECT& _efc, int _data)
{
	//�A�z�z����ɂ��łɗv�f�������Ă��Ȃ���������
	//�����Ă����珈���I��
	if (effectRes_.find(_efc) != effectRes_.end())return;

	//�V�����v�f��ǉ�����
	effectRes_.emplace(_efc, std::move(_data));
}
