struct VS_INPUT
{
    float3 in_pos : POSITION;
    float3 in_color : COLOR;
};

struct VS_OUTPUT
{
    float4 out_position : SV_POSITION;
    float3 out_color : COLOR;

};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.out_position = float4(input.in_pos, 1.0f);
    output.out_color = input.in_color;
    return output;
}