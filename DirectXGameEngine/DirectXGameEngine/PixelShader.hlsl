struct PS_INPUT
{
    float4 in_position : SV_POSITION;
    float3 in_color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.in_color, 1.0f);
}