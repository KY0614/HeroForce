#include "PixelShader.h"

PixelShader* PixelShader::instance_ = nullptr;

void PixelShader::CreateInstance(void)
{
	//インスタンスがnullptrなら初期化されたのち生成処理を行う
	if (instance_ == nullptr)
	{
		instance_ = new PixelShader();
	}
	instance_->Init();
}

PixelShader& PixelShader::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		PixelShader::CreateInstance();
	}
	return *instance_;
}

void PixelShader::Init()
{
	//シェーダーの登録
	InitPS();

	//定数バッファ
	psConstBuf_ = CreateShaderConstantBuffer(sizeof(float) * 8);
}

void PixelShader::Destroy()
{
	//シェーダーの開放
	for (auto& ps : psMap_)
	{
		DeleteShader(ps.second);
	}

	psMap_.clear();

	//シェーダー定数バッファの開放
	DeleteShaderConstantBuffer(psConstBuf_);
}

void PixelShader::DrawGraph(const Vector2& pos, const int& handle)
{
	//シェーダーの設定
	int ps = SearchPS(PS_TYPE::TEXTURE);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, { x,y });

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PixelShader::DrawGraphToShader(const Vector2& pos, const int& handle, const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, { x,y });

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PixelShader::DrawExtendGraphToShader(const Vector2& pos, const Vector2& size, const int& handle)
{
	//シェーダーの設定
	int ps = SearchPS(PS_TYPE::TEXTURE);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//描画座標
	MakeSquereVertex(pos, size);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void PixelShader::DrawExtendGraphToShader(const Vector2& pos, const Vector2& size, const int& handle, const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	if (handle != -1)
	{
		SetUseTextureToShader(0, handle);
	}

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//描画座標
	MakeSquereVertex(pos, size);

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void PixelShader::DrawTimeGraphToShader(const Vector2& pos, const int& handle, const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	// ピクセルシェーダー用の定数バッファのアドレスを取得
	struct ShaderBuffer {
		float time;       // 点滅用の時間
		float padding[3]; // 16バイトアライメント
	};

	ShaderBuffer* cbBuf = (ShaderBuffer*)GetBufferShaderConstantBuffer(cBuf);
	cbBuf->time = buf.r; // `step_`の値を渡す

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, { x,y });

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PixelShader::MakeSquereVertex(Vector2 pos, Vector2 size)
{
	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	// ４頂点の初期化
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertex_[i].rhw = 1.0f;
		vertex_[i].dif = GetColorU8(255, 255, 255, 255);
		vertex_[i].spc = GetColorU8(255, 255, 255, 255);
		vertex_[i].su = 0.0f;
		vertex_[i].sv = 0.0f;
	}

	// 左上
	vertex_[cnt].pos = VGet(sX, sY, 0.0f);
	vertex_[cnt].u = 0.0f;
	vertex_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertex_[cnt].pos = VGet(eX, sY, 0.0f);
	vertex_[cnt].u = 1.0f;
	vertex_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertex_[cnt].pos = VGet(eX, eY, 0.0f);
	vertex_[cnt].u = 1.0f;
	vertex_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertex_[cnt].pos = VGet(sX, eY, 0.0f);
	vertex_[cnt].u = 0.0f;
	vertex_[cnt].v = 1.0f;

	// 頂点インデックス
	cnt = 0;
	index_[cnt++] = 0;
	index_[cnt++] = 1;
	index_[cnt++] = 3;

	index_[cnt++] = 1;
	index_[cnt++] = 2;
	index_[cnt++] = 3;
}

void PixelShader::InitPS()
{
	std::string path = "Data/Shader/";
	std::string fileName;

	auto loadPS = [&](PS_TYPE type, std::string fName) 
	{
		fileName = path + fName;
		psMap_.emplace(std::make_pair(type, LoadPixelShader(fileName.c_str())));
	};

	loadPS(PS_TYPE::TEXTURE, "Texture.cso");
	loadPS(PS_TYPE::FADE, "Fade.cso");
	loadPS(PS_TYPE::FADE_TEXTURE, "FadeTextrure.cso");
	loadPS(PS_TYPE::COL_TX, "ColorTex.cso");
	loadPS(PS_TYPE::YELLOW_BLINK, "YellowBlink.cso");
	loadPS(PS_TYPE::COLOR_BLINK, "ColorBlinking.cso");
}

const int& PixelShader::SearchPS(const PS_TYPE& ps) const
{
	//登録が行われているか判断
	const auto& psPair = psMap_.find(ps);
	if (psPair == psMap_.end())
	{
		//登録されていない
		//TRACE("ピクセルシェーダーが登録されていません");
		return -1;
	}

	return psMap_.at(ps);
}

PixelShader::PixelShader()
{
	Init();
}
