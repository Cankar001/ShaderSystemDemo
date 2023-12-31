#   
                   GLSL.std.450                     main    
                   assets/shaders/FlatColorShader.glsl  _    �     // OpModuleProcessed entry-point main
// OpModuleProcessed client opengl100
// OpModuleProcessed target-env opengl
// OpModuleProcessed entry-point main
#line 1
#version 450 core
#pragma shader:fragment

layout(location = 0)out vec4 o_Color;


layout(location = 1)in struct dto
{
 vec2 TexCoord;
 vec4 Color;
} in_dto;

void main()
{
 o_Color = in_dto . Color;
}
  
 GL_GOOGLE_cpp_style_line_directive    GL_GOOGLE_include_directive      main      
   o_Color      dto          TexCoord            Color        in_dto  G  
          G                !                               	         ;  	   
                                        ;                       +                       6               �                 A              =           >  
      �  8  