#pragma once
#include<unordered_map>
#include<string>
class EffectManager
{
public:
	enum class EFFECT
	{
		//�g�p����G�t�F�N�g�𗅗�
	};

	void Load(EFFECT& _efc,std::string _path);
	void Play(void);
	void Stop(void);

private:
	EFFECT type_;
	std::unordered_map<EFFECT,int> effectRes_;
	int playEffect_;
};

