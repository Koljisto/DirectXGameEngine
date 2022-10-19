cbuffer my_cbuffer : register (b0)
{
	float4x4 mat;
}

struct VS_INPUT
{
    float3 in_pos : POSITION;
    float2 in_tex_coord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 out_position : SV_POSITION;
    float2 out_tex_coord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.out_position = mul(float4(input.in_pos, 1.0f), mat);
    output.out_tex_coord = input.in_tex_coord;
    return output;
}