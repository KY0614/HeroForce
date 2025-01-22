cbuffer TimeBuffer : register(b0)
{
    float time; // ���ԕϐ�
}

// �e�N�X�`��
Texture2D Texture0 : register(t0); 

// �T���v���[
SamplerState Sampler0 : register(s0); 

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
    float4 color = Texture0.Sample(Sampler0, uv); // �e�N�X�`���̐F���擾
    float intensity = abs(sin(time)); // ���Ԃɉ������_�Ō���
    float3 yellow = float3(1.0, 1.0, 0.0); // ���F
    color.rgb = lerp(color.rgb, yellow, intensity); // ���̐F�Ɖ��F����
    return color;
}