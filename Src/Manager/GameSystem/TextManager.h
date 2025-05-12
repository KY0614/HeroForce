#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class TextManager
{
public:

	//※フォント生成は各cppで行ってください
	//　その際DeleteFontToHandleを忘れずに！

	//フォント種類
	enum class FONT_TYPE
	{
		NORMAR,
		DOT,
		LOGO,
		BOKUTACHI,
		HANAZOME,
		BANANA,
		PENGS,
		KAKUMEI,
		MAX
	};

	//テキスト呼び出し用
	enum class TEXTS		//MES---メッセージの略
	{
		LEVEL_UP,			//強化関係のテキスト
		LV_UP_SELECT_MES,
		LV_ATK_MES,
		LV_DEF_MES,
		LV_SPEED_MES,
		LV_LIFE_MES,
		MAX
	};

	//テキスト中央揃え用(テキストの長さ ÷ 定数)
	static constexpr int CENTER_TEXT = 2;
	static constexpr int CENTER_TEXT_H = 4;	//英文などの半角のみのテキストはこれで割る

	//インスタンスの生成
	//シングルトン化
	static void CreateInstance();
	static TextManager& GetInstance();

	//初期化
	void Init();

	/// <summary>
	/// テキストを読みこむ
	/// </summary>
	/// <param name="_text"></param>テキスト種類
	/// <returns></returns>テキスト
	std::string TextLoad(const TEXTS _text);

	/// <summary>
	/// フォントの名前を返す
	/// </summary>
	/// <param name="_type"></param>フォント種類
	/// <returns></returns>名前
	inline std::string GetFontName(const FONT_TYPE _type) const { return fontName_[static_cast<int>(_type)]; }

	/// <summary>
	/// テキストサイズを返す
	/// </summary>
	/// <param name="_txt"></param>テキスト
	/// <param name="_num"></param>ナンバー
	/// <returns></returns>テキストサイズ
	int GetTextSize(const std::string _txt,const int _num)const;

	//解放処理
	void Destroy();

private:

	//フォント種類
	std::string fontPath_[static_cast<int>(FONT_TYPE::MAX)];	//パス
	std::string fontName_[static_cast<int>(FONT_TYPE::MAX)];	//名前

	//データ
	std::unordered_map<TextManager::TEXTS, std::string> textDatas_;

	//フォント初期化
	void InitFont();

	//テキストを保持するマップ
	std::unordered_map<TEXTS, std::string> LoadDialogues(const std::string& _filename);

	//テキストの読み込み
	std::string* _Load(const TEXTS _text);

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	TextManager();
	~TextManager() = default;

	//シングルトン化
	static TextManager* instance_;
};
