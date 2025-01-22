cbuffer TimeBuffer : register(b0)
{
    float time; // 時間変数
}

// テクスチャ
Texture2D Texture0 : register(t0); 

// サンプラー
SamplerState Sampler0 : register(s0); 

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
    float4 color = Texture0.Sample(Sampler0, uv); // テクスチャの色を取得
    float intensity = abs(sin(time)); // 時間に応じた点滅効果
    float3 yellow = float3(1.0, 1.0, 0.0); // 黄色
    color.rgb = lerp(color.rgb, yellow, intensity); // 元の色と黄色を補間
    return color;
}