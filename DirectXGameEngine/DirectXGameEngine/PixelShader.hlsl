struct PS_INPUT
{
    float4 in_position : SV_POSITION;
    float2 in_tex_coord : TEXCOORD;
};

Texture2D obj_texture : TEXTURE : register(t0);
SamplerState obj_sampler_state : SAMPLER : register(s0);

float4 main(const PS_INPUT input) : SV_TARGET
{
    float3 pixel_color = obj_texture.Sample(obj_sampler_state, input.in_tex_coord);
    return float4(pixel_color, 1.0f);
}