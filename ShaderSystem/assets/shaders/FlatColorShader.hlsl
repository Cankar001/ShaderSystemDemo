#version 450 core
#pragma shader:vertex

struct vs_in
{
    float3 pos : POS;
    float2 uv0 : TEXCOORD0;
    float4 color;
};

struct vs_out
{
    float4 pos      : SV_POSITION,
	float2 uv0      : TEXCOORD0,
	float4 color    : COLOR0
};

cbuffer global_ubo : register(b0)
{
    matrix Projection;
    matrix View;
} global_ubo;

cbuffer u_push_constants : register(b1)
{
    matrix Model;
} u_push_constants;

vs_out main(vs_in input)
{
    vs_out out;
    out.pos = global_ubo.projection * global_ubo.view * u_push_constants.model * float4(input.pos, 1.0);
    out.uv0 = input.uv0;
    out.color = input.color;
    return out;
}

#version 450 core
#pragma shader:fragment

struct vs_out
{
    float4 pos      : SV_POSITION,
	float2 uv0      : TEXCOORD0,
	float4 color
};

float4 main(vs_out input)
{
    return input.color;
}
