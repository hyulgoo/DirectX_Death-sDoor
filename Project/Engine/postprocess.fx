#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"

struct VS_IN
{
	float3 vLocalPos : POSITION;
	float2 vUV : TEXCOORD;	
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOORD;
};


// ============================
// GrayShader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_GrayShader(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_GrayShader(VS_OUT _in) : SV_Target
{
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);

	float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;

	vColor = float4(vAver, vAver, vAver, 1.f);

	return vColor;
}


// ============================
// Distortion Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Noise Texture
// ============================
VS_OUT VS_Distortion(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);	
	output.vUV = _in.vUV;

	return output;
}

//float4 PS_Distortion(VS_OUT _in) : SV_Target
//{
//	float2 vUV = _in.vPosition.xy / g_Resolution;
//		
//	
//	float fChange = cos(( (vUV.x - g_AccTime * 0.05f) / 0.15f) * 2 * 3.1415926535f) * 0.05f;
//
//	vUV.y += fChange;
//
//	float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
//	//vColor.r *= 2.f;
//
//	return vColor;
//}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
	float2 vUV = _in.vPosition.xy / g_Resolution;
	
	// Noise Texture 가 세팅이 되어 있다면
	if (g_btex_1)
	{
		float2 vNoiseUV = float2(_in.vUV.x - (g_AccTime * 0.2f), _in.vUV.y);
		float4 vNoise = g_tex_1.Sample(g_sam_0, vNoiseUV);

		vNoise = (vNoise - 0.5f) * 0.02f;		

		vUV += vNoise.r;
	}

	float4 vColor = g_tex_0.Sample(g_sam_0, vUV);

	return vColor;
}

float4 PS_ShockWaveShader(VS_OUT _in) : SV_Target
{
	// 특정 위치에서 파장처럼 퍼져나가는 효과를 줌.
    float CurrentTime = 10.f * g_float_0 / 7.f;

    float3 WaveParams = float3(10.f, 1.f, 0.1f);

    float ratio = 1.f;

	//마우스 위치를 기준으로 퍼지게 하려면 아래 줄을 사용
	//float2 WaveCentre = float2( iMouse.xy / iResolution.xy );

    float2 WaveCentre = float2(0.5f, 0.5f);
    WaveCentre.y *= ratio;

    float2 texCoord = _in.vPosition.xy / g_Resolution.xy;
    texCoord.y *= ratio;
    float Dist = distance(texCoord, WaveCentre);


    float4 Color = g_tex_0.Sample(g_sam_0, texCoord);

	//중심에서 변수 거리 내의 픽셀만 왜곡함.
    if ((Dist <= ((CurrentTime) + (WaveParams.z))) &&
	(Dist >= ((CurrentTime) - (WaveParams.z))))
    {
		// 픽셀 오프셋 거리
        float Diff = (Dist - CurrentTime);
        float ScaleDiff = (1.f - pow(abs(Diff * WaveParams.x), WaveParams.y));
        float DiffTime = (Diff * ScaleDiff);

		// 왜곡 방향
        float2 DiffTexCoord = normalize(texCoord - WaveCentre);

		// 시간이 지날수록 점차 약해지게 함.
        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.f));
        Color = g_tex_0.Sample(g_sam_0, texCoord);
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.f);
    }

    return Color;
}


// ============================
// WindShader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Noise Texture
// g_vec2_0 : Noise Texture Resolution
// g_vec4_0 : Spawn Data
// g_vec4_1 : Spawn Data
// ============================
VS_OUT VS_WindShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
    output.vUV = _in.vUV;

    return output;
}

// 바람이 불고 있는 효과를 계산하는 함수
float calcWind(float2 uv, float4 PerAmpThickDec, float rep, float time)
{
    float val = abs(PerAmpThickDec.z / (uv.y + PerAmpThickDec.y * sin(uv.x / PerAmpThickDec.x + PerAmpThickDec.w)));
    float mask = 0.f;
    uv.x += time;
    uv.x = fmod(uv.x, rep) - rep * 0.5f;
    val += -2.f / dot(uv, uv);
    uv.x += rep * 0.5f;
    val += -2.f / dot(uv, uv);
    uv.x -= rep * 0.25f;
    mask = rep * 0.5f / dot(uv, uv);
    val = step(val, 0.5f) + step(mask, 1.f);
    return step(val, 0.5f);
}

float4 PS_WindShader(VS_OUT _in) : SV_Target
{
    float4 bg = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 col = float4(0.f, 0.f, 0.f, 0.f);

    float4 params = float4(0.65f, 0.17f, 0.1f, 0.5f);
    
    float2 uv = 30.f * _in.vUV;
    uv.y += uv.x; // 대각선 방향으로 이동하도록 uv 값 변환
   
    col += float4(0.04f, 0.04f, 0.04f, 0.f) * calcWind(uv + float2(0.f, -10.f), params, 8.f, g_AccTime * 1.5f);
    col += float4(0.04f, 0.04f, 0.04f, 0.f) * calcWind(uv + float2(0.f, -20.f), params, 10.f, g_AccTime * 2.5f);
    col += float4(0.04f, 0.04f, 0.04f, 0.f) * calcWind(uv + float2(0.f, -30.f), params, 12.f, g_AccTime * 3.5f);
    col += float4(0.04f, 0.04f, 0.04f, 0.f) * calcWind(uv + float2(0.f, -40.f), params, 8.f, g_AccTime * 4.5f);
    col += float4(0.04f, 0.04f, 0.04f, 0.f) * calcWind(uv + float2(0.f, -50.f), params, 12.f, g_AccTime * 5.5f);
    
    bg.rgb += col;

    return bg;
}






#endif