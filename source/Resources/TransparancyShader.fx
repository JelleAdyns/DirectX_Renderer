//GLOBAL VARS

float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WORLD;
Texture2D gDiffuseMapTransparancy : DiffuseMap;
bool gUseNormalMap : UseNormalMap;
SamplerState gSamplerState;
//{
//    Filter = MIN_MAG_MIP_POINT;
//    AddressU = Wrap; //Wrap, Mirror, Clamp, Border
//    AddressV = Wrap; //Wrap, Mirror, Clamp, Border
//};

RasterizerState gRasterizerState
{
    CullMode = none;
    FrontCounterClockwise = false;
};

BlendState gBlendState
{
    BlendEnable[0] = true;
    SrcBlend = src_alpha;
    DestBlend = inv_src_alpha;
    BlendOp = add;
    SrcBlendAlpha = zero;
    DestBlendAlpha = zero;
    BlendOpAlpha = add;
    RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState gDepthStencilState
{
    DepthEnable = true;
    DepthWriteMask = zero;
    DepthFunc = less;
    StencilEnable = false;
    
    StencilReadMask = 0x0F;
    StencilWriteMask = 0x0F;
    
    FrontFaceStencilFunc = always;
    BackFaceStencilFunc = always;

    FrontFaceStencilDepthFail = keep;
    BackFaceStencilDepthFail = keep;

    FrontFaceStencilPass = keep;
    BackFaceStencilPass = keep;

    FrontFaceStencilFail = keep;
    BackFaceStencilFail = keep;

};

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
    //float4 WorldPosition : TEXTCOORD0;
    float2 UV : TEXTCOORD1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};


// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    //output.WorldPosition = float4(mul(normalize(input.Position), (float3x3) gWorldMatrix), 1.f);
    output.UV = input.UV;
    output.normal = mul(normalize(input.normal), (float3x3) gWorldMatrix);
    output.tangent = mul(normalize(input.tangent), (float3x3) gWorldMatrix);
    return output;
}


// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    return gDiffuseMapTransparancy.Sample(gSamplerState, input.UV);
}


//Technique
technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState,0);
        SetBlendState(gBlendState, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}