
float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;
SamplerState gSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //Wrap, Mirror, Clamp, Border
    AddressV = Wrap; //Wrap, Mirror, Clamp, Border
};
// Input/Output Structs
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
    float2 UV : TEXTCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
    float2 UV : TEXTCOORD;
};


// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    //output.Position = float4(input.Position, 1.f);
    output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.Color = input.Color;
    output.UV = input.UV;
    //output.Position[0] = mul(gWorldViewProj[0][0], output.Position[0]) + mul(gWorldViewProj[1][0], output.Position[0]) + mul(gWorldViewProj[2][0], output.Position[0]) + gWorldViewProj[3][0];
    //output.Position[1] = mul(gWorldViewProj[0][1], output.Position[0]) + mul(gWorldViewProj[1][1], output.Position[0]) + mul(gWorldViewProj[2][1], output.Position[0]) + gWorldViewProj[3][1];
    //output.Position[2] = mul(gWorldViewProj[0][2], output.Position[0]) + mul(gWorldViewProj[1][2], output.Position[0]) + mul(gWorldViewProj[2][2], output.Position[0]) + gWorldViewProj[3][2];
   // output.Position = mul(gWorldViewProj[0], output.Position[0]) + mul(gWorldViewProj[1], output.Position[1]) + mul(gWorldViewProj[2], output.Position[2]) + gWorldViewProj[3];
  
    return output;
}


// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    return gDiffuseMap.Sample(gSamplerState, input.UV);
}


//Technique
technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}