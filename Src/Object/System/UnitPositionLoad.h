#pragma once
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <DxLib.h>

class UnitPositionLoad
{
public:

	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};


	static constexpr float SCALE = 100.0f;
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);

	UnitPositionLoad();
	~UnitPositionLoad();

	void Init();

	//Json�t�@�C���̐ݒ�
	void SetJsonFile();

	//Json�ǂݍ���
	void JsonLoad();

	//�Q�b�^�[
	std::vector<VECTOR>GetPos(UNIT_TYPE type);
	std::vector<VECTOR>GetRot(UNIT_TYPE type);

protected:

	//JSON�t�@�C��
	std::string jsonFile_;

private:

	//���W
	std::map < UNIT_TYPE, std::vector<VECTOR>> pos_;

	//�p�x
	std::map < UNIT_TYPE, std::vector<VECTOR>> rot_;

};

