#pragma once
class SoundManager
{
public:
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//�����Ɏg�p���鉹�y����ʉ��Ȃǂ𗅗�
	};

	/// <summary>
	/// �T�E���h�̒ǉ�
	/// </summary>
	/// <param name="_type">���</param>
	/// <param name="_data">�f�[�^</param>
	void Add(const SOUND _type, const int _data);

	
private:

};

