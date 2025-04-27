#ifndef _WATER
#define _WATER

#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// Water Shader
//
// g_tex_0 : Color Texture
// g_tex_1 : Normal Texture
// ========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
   
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VTX_IN_INST
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;

    // Per Instance Data
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint iRowIndex : ROWINDEX;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    // 픽셀 단위 라이팅을 위한 ViewPos 및 ViewNormal
    float3 vViewPos : POSITION;
        
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VS_OUT VS_Water(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    
    return output;
}

VS_OUT VS_Water_Inst(VTX_IN_INST _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, _in.iRowIndex);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));

    return output;
}


// Rasterizer로 정점 사이 픽셀 호출

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vData : SV_Target3;
    float4 vEmissive : SV_Target4;
};


PS_OUT PS_Water(VS_OUT _in) : SV_Target
{
    // obj 색상 설정(회색)
    float4 vObjectColor = float4(0.29f, 0.43f, 0.3f, 1.f);
    
    float3 vViewNormal = _in.vViewNormal;
     
    // 텍스쳐가 있으면, 해당 색상을 사용한다.
    if (g_btex_0)
    {
        float fChange = cos(((_in.vUV.x - g_AccTime * 0.05f) / 0.15f) * 2 * 3.1415926535f) * 0.005f;
        
       _in.vUV.y += fChange;
        
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
        // 색상이 없는 경우.
        if (!any(vObjectColor))
        {
            vObjectColor = float4(0.29f, 0.43f, 0.3f, 1.f) * 2.f;
        }
        
        else
        {
            vObjectColor *= float4(0.29f, 0.43f, 0.3f, 1.f) * 2.f;

        }
    }
       
    // NomalMap 이 있다면
    if (g_btex_1)
    {
        float3x3 matRot =
        {
            // 현재 사용 중인 리소스가 OpenGL에서 사용되는 리소스임
            // OpenGL은 DirextX와 바이노말의 방향이 반대이므로 바이노말을 뒤집어줌.
            _in.vViewTangent,
            -_in.vViewBinormal,
            _in.vViewNormal,
        };

        float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        // 로컬좌표 기준에 있는 정점 위치를 투영좌표계 기준으로 옮겨줌.
        vNormal = normalize((vNormal * 2.f) - 1.f);
        vViewNormal = mul(vNormal, matRot);
    }
    
    // 7번 텍스쳐 레지스터에 값이 있다면 CrackTexture임. 이걸 사용.
    if (g_btex_7)
    {
        float4 vCrackTextureColor = g_tex_7.Sample(g_sam_0, _in.vUV);
        
        // 값이 0이 아니라면.
        if (any(vCrackTextureColor))
        {
            vCrackTextureColor *= float4(1.f, 0.1f, 0.35f, 1.f);
            vCrackTextureColor *= (1.f - g_float_0);
            vObjectColor = vObjectColor + vCrackTextureColor;
        }
        
        // paperburn 시작 시간이 들어왔다면.
        if (g_float_1 > 0.f)
        {
            static float fFirstTime = g_float_1;
            
            // 6번 텍스쳐는 NoiseTexture.
            float4 vNoiseTextureColor = g_tex_6.Sample(g_sam_0, _in.vUV);
            float fGrey = (vNoiseTextureColor.r + vNoiseTextureColor.g + vNoiseTextureColor.b) / 3.f;
            
            //threshold on sin time
            float thresh = (1.f - (g_AccTime - fFirstTime) * 0.5f)
            + 0.5f;
            float thresh2 = ((1.f - (g_AccTime - fFirstTime) + 0.1f) * 0.5f)
            + 0.5f;
    
            if (fGrey > thresh2)
            {
                vObjectColor.r = 1.f;
                vObjectColor.g = 0.1f;
                vObjectColor.b = 0.35f;
            }
    
            if (fGrey > thresh)
            {
                vObjectColor.r = 0.f;
                vObjectColor.g = 0.f;
                vObjectColor.b = 0.f;
                vObjectColor.a = 0.f;
            }
        }
    }
    
    PS_OUT output = (PS_OUT) 0.f;
    
    output.vColor = float4(vObjectColor.xyz, 1.f);
    vViewNormal.x *= -1.f;
    vViewNormal.y *= -1.f;
    vViewNormal.z *= -1.f;
    output.vNormal = float4(vViewNormal.xyz, 1.f);
    output.vPosition = float4(_in.vViewPos.xyz, 1.f);
    output.vData = float4(0.f, 0.f, 0.f, 1.f);
    output.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
        
    return output;
}

#endif
