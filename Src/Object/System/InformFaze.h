<<<<<<< HEAD
#pragma once
#include<DxLib.h>
#include<string>
#include"../../Application.h"

class InformFaze
{
public:

	static constexpr int INFORM_HARF = Application::DEFAULT_FPS;
	static constexpr int INFORM_FINISH = Application::DEFAULT_FPS * 2;

	static constexpr int MISSION_SIZE_X = 1270;
	static constexpr int MISSION_SIZE_Y = 720;

	//表示の初期位置及び最終位置(最後の-は微調整用)
	static constexpr float STRING_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 60.0f;
	static constexpr float STRING_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f;

	InformFaze(void);
	~InformFaze(void);

	bool Update(void);
	void Draw(void);

	/// <summary>
	/// フェーズ数設定(実質リセット)※フェーズ表示を行う前に必ず呼び出すこと
	/// </summary>
	/// <param name="_cnt">フェーズ数</param>
	void SetFazeCnt(const int _cnt);

private:
	VECTOR informStrPos_;		//フェーズ数表示位置
	VECTOR informImgPos_;		//目標画像表示位置
	int fazeCnt_;			//フェーズ数カウント
	int informCnt_;			//経過時間カウント
	int missionImg_[2];		//ミッション画像
	int missionCnt_;		//上記配列の指数
	int font_;	//フォント
};

=======
#pragma once
#include<DxLib.h>
#include<string>
#include"../../Application.h"

class InformFaze
{
public:

	static constexpr int INFORM_HARF = Application::DEFAULT_FPS;
	static constexpr int INFORM_FINISH = Application::DEFAULT_FPS * 2;

	static constexpr int MISSION_SIZE_X = 1270;
	static constexpr int MISSION_SIZE_Y = 720;

	//表示の初期位置及び最終位置(最後の-は微調整用)
	static constexpr float STRING_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 60.0f;
	static constexpr float STRING_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f;

	InformFaze(void);
	~InformFaze(void);

	bool Update(void);
	void Draw(void);

	/// <summary>
	/// フェーズ数設定(実質リセット)※フェーズ表示を行う前に必ず呼び出すこと
	/// </summary>
	/// <param name="_cnt">フェーズ数</param>
	void SetFazeCnt(const int _cnt);

private:
	VECTOR informStrPos_;		//フェーズ数表示位置
	VECTOR informImgPos_;		//目標画像表示位置
	int fazeCnt_;			//フェーズ数カウント
	int informCnt_;			//経過時間カウント
	int missionImg_[2];		//ミッション画像
	int missionCnt_;		//上記配列の指数
	int font_;	//フォント
};

>>>>>>> Data2
