#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// ========================
// Directional Light Shader
// mesh : RectMesh
// g_int_0 : Light Index
// g_mat_0 : (LightView * LightProj) Matrix
// 
// g_tex_0 : Position Target
// g_tex_1 : Normal Target
// g_tex_3 : Shadow DepthMap
// ========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
};

VS_OUT VS_DirLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 화면 전체의 픽셀을 호출하기 위해 RectMesh의 
    // 로컬 정점을 2배로 확장시켜 MDC 좌표계 전체로 확장
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}


struct PS_OUT
{
    float4 vDiffuse  : SV_Target0;
    float4 vSpecular : SV_Target1;
};

PS_OUT PS_DirLightShader(VS_OUT _in)
{ 
    PS_OUT output = (PS_OUT) 0.f;
    
    // Position 정보
    float3 vViewPos = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    
    // 받아온 위치 정보가 없다면(0.f, 0.f, 0.f) 호출하지 않음.
    if (!any(vViewPos.xyz))
        discard;
    
    // 여기까지 왔다면 위치 정보가 있는 것이므로 같은 위치의 노말값을 가져와서 광원계산을 함.
    float3 vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    
    tLightColor lightcolor = (tLightColor) 0.f;
    CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);
    
    // 그림자 판정
    // ViewPos -> WorldPos 
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), g_matViewInv).xyz;
    
    // WorldPos -> Light 투영
    float4 vLightProj = mul(float4(vWorldPos, 1.f), g_mat_0);

    // w 로 나눠서 실제 xy 투영좌표를 구함
    vLightProj.xyz /= vLightProj.w;

    // 샘플링을 위해 투영좌표계를 UV 좌표계로 변환
    float fShadowPow = 0.f;
    float2 vDepthMapUV = float2((vLightProj.x / 2.f) + 0.5f, -(vLightProj.y / 2.f) + 0.5f);
    float fDepth = g_tex_3.Sample(g_sam_0, vDepthMapUV).r;

    // 광원에 기록된 깊이보다 물체의 깊이가 더 멀 때, 그림자 판정
    if (0.f != fDepth
        && 0.f <= vDepthMapUV.x && vDepthMapUV.x <= 1.f
        && 0.f <= vDepthMapUV.y && vDepthMapUV.y <= 1.f
        && vLightProj.z >= fDepth + 0.0001f)
    {
        fShadowPow = 0.9f;
    }
    
    output.vDiffuse = lightcolor.vDiffuse * saturate(1.f - fShadowPow) + lightcolor.vAmbient;
    output.vDiffuse = ceil(output.vDiffuse * 3) / 3.f;
    output.vSpecular = lightcolor.vSpecular * saturate(1.f - fShadowPow);
      
    // ImGui에서 확인할 수 있도록 알파값을 1로 넣어줌.
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}


//BLOOM======================
//VS_OUT VS_Bloom(VS_IN _in)
//{
//    VS_OUT output = (VS_OUT) 0.f;

//    float3 dirX = g_matWV[0];
//    float3 dirY = g_matWV[1];
    
//    float radius = 30.f;
    
//    float3 pos = _in.vPos + radius * (output.vPosition.x * dirX + output.vPosition.y * dirY);
//    output.vPosition = mul(g_matProj, float4(pos, 1));
//    output.vUV = 0.5 * output.vPosition.xy + 0.5;
//    return output;
//}


//PS_OUT PS_Bloom(VS_OUT _in)
//{
//    PS_OUT output = (PS_OUT) 0.f;
//    float4 corona = g_tex_2.Sample(g_sam_0, _in.vUV);
//    float strenth = 2.64f;
    
//     // Position 정보
//    float3 vViewPos = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    
//    // 받아온 위치 정보가 없다면(0.f, 0.f, 0.f) 호출하지 않음.
//    if (!any(vViewPos.xyz))
//        discard;
    
//    // 여기까지 왔다면 위치 정보가 있는 것이므로 같은 위치의 노말값을 가져와서 광원계산을 함.
//    float3 vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    
//    tLightColor lightcolor = (tLightColor) 0.f;
//    CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);
    
//    output.vDiffuse = strenth * pow(0.8, 10.4) * corona * lightcolor.vDiffuse;
//    return output;
//}



// ========================
// Point Light Shader
// mesh : SphereMesh
// g_int_0 : Light Index
// g_tex_0 : Position Target
// g_tex_1 : Normal Target
// ========================
VS_OUT VS_PointLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // PointLight는 화면 일부를 차지하는 Light이기 때문에
    // 화면의 어느 부분에 존재하는 지 알 수 있는 픽셀 좌표로 변환하여 넘겨줌.
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
    return output;
}

PS_OUT PS_PointLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // 픽셀 좌표를 해상도로 나눠 화면 내에서 상대위치를 알게 함.
    // 해당 Obj의 위치가 아닌 Obj의 위치에 포함된 것처럼 보이는 픽셀의 위치를 구하기 위한 계산
    float2 vUV = _in.vPosition.xy / g_Resolution;
    
    float3 vViewPos = g_tex_0.Sample(g_sam_0, vUV).xyz;    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv);
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv);
    
    // 만약 평면에서 Sphere 범위 안에 포함된 픽셀을
    // Sphere Mesh의 로컬 좌표계로 옮겼을 때 Sphere 범위 안에 포함된다면
    // 범위 안에 있는 것이므로 광원을 계산하고 아니라면 discard
    if (length(vLocalPos) <= 0.5f)
    {
        tLightColor lightcolor = (tLightColor) 0.f;
        float3 vViewNormal = g_tex_1.Sample(g_sam_0, vUV).xyz;
        CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);
        
        output.vDiffuse  = lightcolor.vDiffuse + lightcolor.vAmbient;
        output.vSpecular = lightcolor.vSpecular;
        
        output.vDiffuse.a  = 1.f;
        output.vSpecular.a = 1.f;
    }
    // 받아온 위치 정보가 없다면(0.f, 0.f, 0.f) 호출하지 않음.
    else
    {
        discard;
    }
    
    return output;
}

// ========================
// Spot Light Shader
// mesh : Cone
// g_int_0 : Light Index
// g_tex_0 : Position Target
// g_tex_1 : Normal Target
// ========================

VS_OUT VS_SpotLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // PointLight는 화면 일부를 차지하는 Light이기 때문에
    // 화면의 어느 부분에 존재하는 지 알 수 있는 픽셀 좌표로 변환하여 넘겨줌.
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;
}
    
PS_OUT PS_SpotLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // 픽셀 좌표를 해상도로 나눠 화면 내에서 상대위치를 알게 함.
    // 해당 Obj의 위치가 아닌 Obj의 위치에 포함된 것처럼 보이는 픽셀의 위치를 구하기 위한 계산
    float2 vUV = _in.vPosition.xy / g_Resolution;
    
    float3 vViewPos =  g_tex_0.Sample(g_sam_0, vUV).xyz;
    float3 vWorldPos = mul(float4(vViewPos,  1.f), g_matViewInv);
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv);
    
    // 만약 평면에서 Sphere 범위 안에 포함된 픽셀을
    // Sphere Mesh의 로컬 좌표계로 옮겼을 때 Sphere 범위 안에 포함된다면
    // 범위 안에 있는 것이므로 광원을 계산하고 아니라면 discard
    if (length(vLocalPos) <= 0.5f)
    {
        tLightColor lightcolor = (tLightColor) 0.f;
        float3 vViewNormal = g_tex_1.Sample(g_sam_0, vUV).xyz;
        CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);
        
        output.vDiffuse = lightcolor.vDiffuse + lightcolor.vAmbient;
        output.vSpecular = lightcolor.vSpecular;
        
        output.vDiffuse.a  = 1.f;
        output.vSpecular.a = 1.f;
    }
    else
    {
        discard;
    }
    
    return output;
}


// ========================
// Light Merge Shader
// mesh : RectMesh
// g_tex_0 : ColorTarget
// g_tex_1 : DiffuseTarget
// g_tex_2 : SpecularTarget
// g_tex_3 : EmissiveTarget
// ========================
VS_OUT VS_LightMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_LightMerge(VS_OUT _in) : SV_Target0
{
    float4 vOutColor = (float4) 0.f;
    
    // MRT에 기록된 정보 중 출력에 필요한 Color, Light 정보를 합쳐 출력함.
    float3 Color    = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    float3 Diffuse  = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    float3 Specular = g_tex_2.Sample(g_sam_0, _in.vUV).xyz;
    float3 Emissive = g_tex_3.Sample(g_sam_0, _in.vUV).xyz;
    
    vOutColor.xyz = (Color * Diffuse) + Emissive + Specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}


// ===============
// DepthMap Shader
// MRT : ShadowMap MRT
// RS : CULL_BACK
// BS : Default
// DS : Less
// ===============
struct VS_DEPTH_IN
{
    float3 vPos : POSITION;
   
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_DEPTH_OUT
{
    float4 vPosition : SV_Position;
    float4 vProjPos : POSITION;
};

VS_DEPTH_OUT VS_DepthMap(VS_DEPTH_IN _in)
{
    VS_DEPTH_OUT output = (VS_DEPTH_OUT) 0.f;
        
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vProjPos  = output.vPosition;

    return output;
}

float PS_DepthMap(VS_DEPTH_OUT _in) : SV_Target
{
    float fOut = 0.f;
    fOut = _in.vProjPos.z / _in.vProjPos.w;
    return fOut;
}

#endif