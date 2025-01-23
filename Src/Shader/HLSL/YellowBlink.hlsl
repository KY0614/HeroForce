//ピクセルシェーダーの入力
struct PS_INPUT
{
    //座標（プロジェクション空間）
    float4 Position : SV_POSITION;
    //ディフーズカラー
    float4 Diffuse : COLORO;
    float4 Spe : COLOR1;
    //float3 color : COLOR1;    // カスタムデータ
    //テクスチャ座標
    float2 TexCoords0 : TEXCOORD0;
};

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);


//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
    float g_time;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //UV座標を受け取る
    float2 uv = PSInput.TexCoords0;

    //UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol =
        g_SrcTexture.Sample(g_SrcSampler, uv);

    return float4(uv.y, uv.y, uv.y, 1.0f);
    return float4(uv.x, uv.x, uv.x, 1.0f);
    return srcCol;
    return float4(1.0f, 0.0f, 0.0f, 1.0f);

    float intensity = abs(sin(g_time)); // 時間に応じた点滅効果
    float3 yellow = float3(1.0, 1.0, 0.0); // 黄色
    srcCol.rgb = lerp(srcCol.rgb, yellow, intensity); // 元の色と黄色を補間
    return srcCol;
}