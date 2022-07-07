#include "Basic.hlsli"
VSOutput main(float4 pos: POSITION,float normal:NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}
//float4 main( float4 pos : POSITION) : SV_POSITION
//{
//	return pos;
//}