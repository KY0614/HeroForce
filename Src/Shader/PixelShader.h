#pragma once
#include <DxLib.h>
#include <string>
#include<unordered_map>
#include "../Common/Vector2.h"

class PixelShader
{
public:

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static PixelShader& GetInstance(void);

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 3;
	
	// 頂点数
	static constexpr int NUM_VERTEX = 4;
	
	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;
	
	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	enum class PS_TYPE
	{
		TEXTURE,
		COL_TX,
		FADE,
		FADE_TEXTURE,
		YELLOW_BLINK,
		COLOR_BLINK,
		MAX
	};

	//初期化処理
	void Init();

	//解放処理
	void Destroy();

	//画像通常描画
	void DrawGraph(
		const Vector2& pos, 
		const int& handle);

	//画像描画（定数付き）
	void DrawGraphToShader(
		const Vector2& pos,
		const int& handle, 
		const PS_TYPE& type,
		const COLOR_F& buf,
		const COLOR_F& subBuf = COLOR_F{});

	//サイズ指定描画（通常）
	void DrawExtendGraphToShader(
		const Vector2& pos,
		const Vector2& size, 
		const int& handle);

	//サイズ指定描画
	void DrawExtendGraphToShader(
		const Vector2& pos, 
		const Vector2& size,
		const int& handle, 
		const PS_TYPE& type,
		const COLOR_F& buf,
		const COLOR_F& subBuf = COLOR_F{});

	//画像描画（更新処理付き)
	void DrawTimeGraphToShader(
		const Vector2& pos,
		const int& handle,
		const PS_TYPE& type,
		const COLOR_F& buf,
		const COLOR_F& subBuf = COLOR_F{});

private:

	//頂点情報
	VERTEX2DSHADER vertex_[NUM_VERTEX];
	WORD index_[NUM_VERTEX_IDX];

	//シェーダー定数バッファ
	int psConstBuf_;

	//シェーダーのリソースマップ
	std::unordered_map< PS_TYPE, int>psMap_;

	//描画用の四角頂点の作成
	void MakeSquereVertex(Vector2 pos, Vector2 size);

	//シェーダーの読み込み
	void InitPS();

	//シェーダーの検索
	const int& SearchPS(const PS_TYPE& ps)const;

	//コンストラクタ
	PixelShader();

	//デストラクタ
	~PixelShader() = default;

	//インスタンス用
	static PixelShader* instance_;

};

