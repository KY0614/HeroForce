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
	static void CreateInstance(void);
	static TextManager& GetInstance(void);

	void Init();
	void Release();

	//テキスト読み込み
	std::string TextLoad(TEXTS text);

	//フォント名前受け取り
	inline std::string GetFontName(FONT_TYPE type) const { return fontName_[static_cast<int>(type)]; }

	//テキストサイズの受け取る
	int GetTextSize(std::string txt, int num)const;

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
	std::unordered_map<TEXTS, std::string> LoadDialogues(const std::string& filename);

	//テキストの読み込み
	std::string* _Load(TEXTS text);

#pragma region シングルトン関係
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	TextManager(void);
	~TextManager(void) = default;

	//シングルトン化
	static TextManager* instance_;
#pragma endregion

};
