
Texture2D g_MeshTexture;            // Color texture for mesh
SamplerState g_SampleState;

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) {
    matrix World;
    matrix View;
    matrix Projection;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT {
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 UV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR, float2 Tex : TEXCOORD0) {
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = Color;
    output.UV = Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader - Color
//--------------------------------------------------------------------------------------
float4 PS_Color(VS_OUTPUT input) : SV_Target{
    return input.Color;
}


//--------------------------------------------------------------------------------------
// Pixel Shader - Texture
//--------------------------------------------------------------------------------------
float4 PS_Texture(VS_OUTPUT input) : SV_Target{
    return g_MeshTexture.Sample(g_SampleState, input.UV)/* * input.Color*/;
}