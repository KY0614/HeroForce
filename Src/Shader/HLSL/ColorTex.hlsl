
// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
	float4 Diffuse : COLOR0; // ディフューズカラー
	float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
};

// 定数バッファ：スロット3番目(b3と書く)
cbuffer cbParam : register(b3)
{
	float4 g_color;
	float g_num_x;
	float g_num_y;
	float g_animNum;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);//SetUseTextureToShaderで０に入れたテクスチャを使用

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);


float4 main(PS_INPUT PSInput) : SV_TARGET
{
	// 対象の画素を取得
	float2 uv = PSInput.TexCoords0;

	// テクスチャ範囲を調整
	float2 adjustedUV = float2((1.0f / 6) * g_animNum, 0.0f) + uv * float2((1.0f / 6), 1.0f);

	float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, adjustedUV);

	if (srcCol.a < 0.01f)
	{
		// 描画しない
		discard;
	}

	//float4 srcCol = g_SrcTexture.Sample(TextureSampler, PSInput.TexCoords0);

	//色調反転
	//srcCol = float4(1.0f - srcCol.r, 1.0f - srcCol.g, 1.0f - srcCol.b, 1.0f);
	
	//グレースケール
	//float mono = (srcCol.r + srcCol.g + srcCol.b) / 3;
	//srcCol = float4(mono, mono, mono, 1.0f);


	return srcCol * g_color;
}