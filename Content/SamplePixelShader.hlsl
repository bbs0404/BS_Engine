Texture2D tex : register(t0);
SamplerState sampleType : register(s0);

// �ȼ� ���̴��� ����� �ȼ��� �� �������Դϴ�.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 color : COLOR0;
};

// (������) �� �����Ϳ� ���� ��� �Լ��Դϴ�.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 texColor;
	texColor = tex.Sample(sampleType, input.texCoord);
	return texColor;
}
