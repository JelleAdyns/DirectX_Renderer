
//GLOBAL VARS
float gPI = { 3.14159265358979323846f };
float gShininess = { 25.f };
float gLightIntensity = { 7.f };
float3 gLightDirection = { 0.577f, -0.577f, 0.577f };
float3 gAmbient = { 0.03f, 0.03f, 0.03f };
float3 gCameraPosition : CAMERA;
float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WORLD;
Texture2D gDiffuseMapOpaque : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossinessMap : GlossinessMap;
bool gUseNormalMap : UseNormalMap;

SamplerState gSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //Wrap, Mirror, Clamp, Border
    AddressV = Wrap; //Wrap, Mirror, Clamp, Border
};


RasterizerState gRasterizerState
{
    CullMode = back;
    FrontCounterClockwise = false;
};

BlendState gBlendState;
DepthStencilState gDepthStencilState;

// Input/Output Structs
struct VS_INPUT
{
	float3 Position : POSITION;
    float2 UV : TEXTCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION0;
    float4 WorldPosition : TEXTCOORD0;
    float2 UV : TEXTCOORD1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};


// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.WorldPosition = float4(mul(normalize(input.Position), (float3x3) gWorldMatrix), 1.f);
    output.UV = input.UV;
    output.normal = mul(normalize(input.normal), (float3x3)gWorldMatrix);
    output.tangent = mul(normalize(input.tangent), (float3x3)gWorldMatrix);
    return output;
}


// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 invViewDirection = normalize(gCameraPosition - input.WorldPosition.xyz );

    float observedArea;
    float3 reflection;
    if(gUseNormalMap)
    {
        float3 biNormal = cross(input.normal, input.tangent);

        float3x3 tangentSpaceAxis = float3x3(float3(input.tangent),
                                        float3(biNormal),
                                        float3(input.normal));
    
        float3 normalSample = gNormalMap.Sample(gSamplerState, input.UV).rgb * 2 - float3(1.f, 1.f, 1.f);

        normalSample = mul(normalize(normalSample), tangentSpaceAxis);

        observedArea = dot(normalSample, -gLightDirection);
        if (observedArea <= 0) return float4(0.0f, 0.0f, 0.0f, 0.0f);
        reflection = reflect(gLightDirection, normalSample);
	
    }
    else
    {
        observedArea = dot(input.normal, -gLightDirection);
        if (observedArea <= 0) return float4(0.0f, 0.0f, 0.0f, 0.0f);
        reflection = reflect(-gLightDirection, input.normal);
    }
   
   
    const float3 diffuse = (gLightIntensity * gDiffuseMapOpaque.Sample(gSamplerState, input.UV).rgb / gPI) * observedArea;
    const float cosAlpha = max(0.f, dot(reflection, invViewDirection));

    const float3 specular = pow(cosAlpha, gGlossinessMap.Sample(gSamplerState, input.UV).r * gShininess) * gSpecularMap.Sample(gSamplerState, input.UV).rgb;

    return saturate(float4(diffuse + specular + gAmbient, 0.f));
}


//Technique
technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}