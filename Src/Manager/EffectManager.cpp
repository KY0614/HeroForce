#include<EffekseerForDXLib.h>
#include<cassert>
#include "EffectManager.h"

/// <summary>
/// �G�t�F�N�g�̒ǉ�
/// </summary>
/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
/// <param name="_data">�G�t�F�N�g�̃f�[�^</param>
void EffectManager::Add(const EFFECT& _efc, int _data)
{
	//�A�z�z����ɂ��łɗv�f�������Ă��邩������
	//�����Ă����珈���I��
	if (effectRes_.find(_efc) != effectRes_.end())return;

	//�V�����v�f��ǉ�����
	effectRes_.emplace(_efc, std::move(_data));
}


/// <summary>
/// �G�t�F�N�g�̍Đ�
/// </summary>
/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
/// <param name="_pos">�Đ��ʒu</param>
/// <param name="_qua">�p�x</param>
/// <param name="_size">�傫��</param>
void EffectManager::Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size)
{
	//�v����
	//(�V���O���g��������Ă���G�t�F�N�g�}�l�[�W���ɂ�����PlayId�͗Վ��ϐ��̈�����ł����̂��H)
	//(PlayId���A�z�z��ɂ��ׂ��Ȃ̂��H��������PlayId���K�v�Ȃ̂��H)
	
	//�z����ɗv�f�������Ă��Ȃ���������
	if (effectPlay_.find(_efc) == effectPlay_.end()) {
		//�����Ă��Ȃ��Ƃ��v�f��ǉ�����
		effectPlay_.emplace(_efc, PlayEffekseer3DEffect(effectRes_[_efc]));
	}else {
		//�����Ă����猳�����ɏ㏑������
		effectPlay_[_efc] = PlayEffekseer3DEffect(effectRes_[_efc]);
	}

	//���̑��e��ݒ�
	//�傫��
		SetScalePlayingEffekseer3DEffect(effectPlay_[_efc], _size, _size, _size);
	//�p�x
		SetRotationPlayingEffekseer3DEffect(effectPlay_[_efc], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//�ʒu
		SetPosPlayingEffekseer3DEffect(effectPlay_[_efc], _pos.x, _pos.y, _pos.z);

}


/// <summary>
/// �G�t�F�N�g�̍Đ���~
/// </summary>
/// <param name="_efc">�G�t�F�N�g��ޖ�</param>
void EffectManager::Stop(const EFFECT& _efc)
{
	//�z����ɓ����Ă��Ȃ����̂��~���悤�Ƃ�����x��
	if (effectPlay_.find(_efc) == effectPlay_.end())assert("�ݒ肵�Ă��Ȃ��G�t�F�N�g���~���悤�Ƃ��Ă��܂��B");
	//�Đ���~
	StopEffekseer3DEffect(effectPlay_[_efc]);
}

//�������
void EffectManager::Release(void)
{
	//�z����̗v�f��S�ď���
	//���X�̃f�[�^�̓��\�[�X�}�l�[�W���������Ă���̂ł����B
	//�O�̂��߂��Ƃł���ɂ��Ă�����(_data��std::move�ɂ���)
	effectRes_.clear();
}
