#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class TextManager
{
public:

	//���t�H���g�����͊ecpp�ōs���Ă�������
	//�@���̍�DeleteFontToHandle��Y�ꂸ�ɁI

	//�t�H���g���
	enum class FONT_TYPE
	{
		NORMAR,
		DOT,
		LOGO,
		BOKUTACHI,
		MAX
	};

	//�e�L�X�g�Ăяo���p
	enum class TEXTS		//MES---���b�Z�[�W�̗�
	{
		LEVEL_UP,			//�����֌W�̃e�L�X�g
		LV_UP_SELECT_MES,
		LV_ATK_MES,
		LV_DEF_MES,
		LV_SPEED_MES,
		LV_LIFE_MES,
		LV_TRAP_MES,
		LV_CANNON_MES,
		LV_TRAP_UP_MES,
		LV_CANNON_UP_MES,
		MAX
	};

	//�e�L�X�g���������p(�e�L�X�g�̒��� �� �萔)
	static constexpr int CENTER_TEXT = 2;
	static constexpr int CENTER_TEXT_H = 4;	//�p���Ȃǂ̔��p�݂̂̃e�L�X�g�͂���Ŋ���

	//�C���X�^���X�̐���
	//�V���O���g����
	static void CreateInstance(void);
	static TextManager& GetInstance(void);

	void Init();
	void Release();

	//�e�L�X�g�ǂݍ���
	std::string TextLoad(TEXTS text);

	//�t�H���g���O�󂯎��
	inline std::string GetFontName(FONT_TYPE type) const { return fontName_[static_cast<int>(type)]; }

	//�������
	void Destroy();

private:

	//�t�H���g���
	std::string fontPath_[static_cast<int>(FONT_TYPE::MAX)];	//�p�X
	std::string fontName_[static_cast<int>(FONT_TYPE::MAX)];	//���O

	//�f�[�^
	std::unordered_map<TextManager::TEXTS, std::string> textDatas_;

	//�t�H���g������
	void InitFont();

	//�e�L�X�g��ێ�����}�b�v
	std::unordered_map<TEXTS, std::string> LoadDialogues(const std::string& filename);

	//�e�L�X�g�̓ǂݍ���
	std::string* _Load(TEXTS text);

#pragma region �V���O���g���֌W
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	TextManager(void);
	~TextManager(void) = default;

	//�V���O���g����
	static TextManager* instance_;
#pragma endregion

};