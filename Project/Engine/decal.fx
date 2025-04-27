#ifndef _DECAL
#define _DECAL

#include "value.fx"
// ============================
// Decal Shader
// RS_TYPE  : CULL_FRONT
// DS_TYPE  : NO_TEST_NO_WRITE
// BS_TYPE  : ONE_ONE

// g_int_0  : Light
// g_float_0: Alpha
// g_tex_0  : Output Texture
// g_tex_1  : Position Target
// g_tex_2  : Normal Texture
// =============================

struct VS_DECAL_IN
{
    float3 vPos : POSITION;
};

struct VS_DECAL_OUT
{
    float4 vPosition   : SV_Position;
    float3 vViewPos    : POSITION;
    float3 vViewNormal : NORMAL;
};

VS_DECAL_OUT VS_Decal(VS_DECAL_IN _in)
{
    VS_DECAL_OUT output = (VS_DECAL_OUT) 0.f;    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);        
    return output;
}

struct PS_OUT
{
    float4 vColor    : SV_Target0;
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(VS_DECAL_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = _in.vPosition.xy / g_Resolution;
    
    // 계산한 UV값으로 PositionTargetTex에서 위치값 추출
    float3 vViewPos = g_tex_1.Sample(g_sam_0, vUV).xyz;    
    
    // 얻어온 위치값이 없다면 discardj
    if(!any(vViewPos))
        discard;
    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv);
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv);
        
    
    if (abs(vLocalPos.x) <= 0.5f
        && abs(vLocalPos.y) <= 0.5f
        && abs(vLocalPos.z) <= 0.5f)
    {
        if(g_btex_0)
        {
            // 겹친영역의 UV 판정
            // 바닥을 기준으로 한다면 x축과 z축이 UV의 x, y가 되므로 
            // -0.5 ~ 0.5인 x축을 0 ~ 1로 보간하고
            // 0.5 ~ -0.5인 z축을 0 ~ 1로 보간하여 UV를 구한다.
            float2 vSampleUV = float2(vLocalPos.x + 0.5f, 0.5f - vLocalPos.z);
            float4 vSampleColor = g_tex_0.Sample(g_sam_1, vSampleUV);
            
            // 색상의 알파가 0이라면 discard
            if (vSampleColor.a == 0.f)
                discard;
            
            // 광원 취급이라면 Emissive에 아니라면 Color에 출력
            if (g_int_0)
            {
                output.vEmissive = vSampleColor;
                output.vEmissive.a = g_float_0;
                
            }
            else
            {
                output.vColor = vSampleColor;
            }
            
            // 주변의 픽셀에 접근해서 기울기를 구해 Tangent, Binormal을 구할 수 있음.
            // 이 둘을 외적하여 Normal을 구해 전체 표면 방향정보를 알아냄
                                              
        }
        else
        {
            output.vColor = float4(1.f, 0.f, 0.f, 1.f);
        }
    }
    else
    {
        discard;        
    }
    
    return output;
     //=================
    
}
#endif