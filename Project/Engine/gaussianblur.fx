#ifndef _GAUSSIANBLUR
#define _GAUSSIANBLUR

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// ========================
// GaussianBlurShader
// mesh : RectMesh
// g_tex_0 : ColorTarget
// g_tex_1 : EmissiveTarget
// ========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_GaussianBlur(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

static float GaussianFilter2[7][7] =
{
    0.0044f, 0.0083f, 0.0160f, 0.0237f, 0.0160f, 0.0083f, 0.0044f,
    0.0083f, 0.0162f, 0.0335f, 0.0492f, 0.0335f, 0.0162f, 0.0083f,
    0.0160f, 0.0335f, 0.0686f, 0.1013f, 0.0686f, 0.0335f, 0.0160f,
    0.0237f, 0.0492f, 0.1013f, 0.1496f, 0.1013f, 0.0492f, 0.0237f,
    0.0160f, 0.0335f, 0.0686f, 0.1013f, 0.0686f, 0.0335f, 0.0160f,
    0.0083f, 0.0162f, 0.0335f, 0.0492f, 0.0335f, 0.0162f, 0.0083f,
    0.0044f, 0.0083f, 0.0160f, 0.0237f, 0.0160f, 0.0083f, 0.0044f
};

float4 PS_GaussianBlur(VS_OUT _in) : SV_Target1
{
    float4 vOutColor = (float4) 0.f;
    
    // 영향을 줄 범위.
    float2 texelSize = 5.f / float2(g_Resolution.x, g_Resolution.y);
    
    // 가우시안 필터를 적용할 픽셀 중심 좌표
    float2 centerUV = _in.vUV;
    
    // Emissive 값 추출.
    vOutColor = g_tex_1.Sample(g_sam_0, centerUV);
    
    // 가우시안 필터를 적용할 픽셀 주위 Emissive 값을 추출하여 누적
    // 7x7 가우시안 필터 사용
    for (int i = -3; i <= 3; ++i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            float2 offset = float2(i, j) * texelSize;
            vOutColor += (g_tex_1.Sample(g_sam_0, centerUV + offset) * GaussianFilter2[i + 3][j + 3]);
        }
    }
    
    // 5x5 가우시안 필터 사용
    //for (int i = -2; i <= 2; ++i)
    //{
    //    for (int j = -2; j <= 2; ++j)
    //    {
    //        float2 offset = float2(i, j) * texelSize;
    //        vOutColor += g_tex_1.Sample(g_sam_0, centerUV + offset) * GaussianFilter[i + 2][j + 2];
    //    }
    //}
    
    return vOutColor;
}

#endif