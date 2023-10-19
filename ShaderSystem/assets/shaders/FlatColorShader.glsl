#version 450 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

layout(set = 0, binding = 0) uniform global_uniform_object
{
    mat4 projection;
	mat4 view;
} global_ubo;

layout(binding = 1, set = 0) uniform push_constants
{
	
	// Only guaranteed a total of 128 bytes.
	mat4 model; // 64 bytes
} u_push_constants;

// Data Transfer Object
layout(location = 1) out struct dto
{
	vec2 TexCoord;
	vec4 Color;
} out_dto;

void main()
{
	out_dto.TexCoord = a_TexCoord;
	out_dto.Color = a_Color;
	gl_Position = global_ubo.projection * global_ubo.view * u_push_constants.model * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

// Data Transfer Object
layout(location = 1) in struct dto
{
	vec2 TexCoord;
	vec4 Color;
} in_dto;

void main()
{
	o_Color = in_dto.Color;
}
