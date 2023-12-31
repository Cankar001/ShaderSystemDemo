#   
  >                 GLSL.std.450                      main                2   <   =        assets/shaders/FlatColorShader.glsl  �    �     // OpModuleProcessed entry-point main
// OpModuleProcessed client opengl100
// OpModuleProcessed target-env opengl
// OpModuleProcessed entry-point main
#line 1
#version 450 core
#pragma shader:vertex

layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec2 a_TexCoord;
layout(location = 2)in vec4 a_Color;

layout(set = 0, binding = 0)uniform global_uniform_object
{
    mat4 projection;
 mat4 view;
} global_ubo;

layout(binding = 1, set = 0)uniform push_constants
{


 mat4 model;
} u_push_constants;


layout(location = 1)out struct dto
{
 vec2 TexCoord;
 vec4 Color;
} out_dto;

void main()
{
 out_dto . TexCoord = a_TexCoord;
 out_dto . Color = a_Color;
 gl_Position = global_ubo . projection * global_ubo . view * u_push_constants . model * vec4(a_Position, 1.0);
}
    
 GL_GOOGLE_cpp_style_line_directive    GL_GOOGLE_include_directive      main      
   dto   
       TexCoord      
      Color        out_dto      a_TexCoord       a_Color      gl_PerVertex             gl_Position         gl_PointSize            gl_ClipDistance         gl_CullDistance            !   global_uniform_object     !       projection    !      view      #   global_ubo    *   push_constants    *       model     ,   u_push_constants      2   a_Position    <   gl_VertexID   =   gl_InstanceID   G           G           G           H                H              H              H              G        H  !          H  !       #       H  !             H  !         H  !      #   @   H  !            G  !      G  #   "       G  #   !       H  *          H  *       #       H  *             G  *      G  ,   "       G  ,   !      G  2          G  <         G  =              !                              	           
      	            
   ;                       +                        ;                       +                    	   ;                    	                +                           	                        ;                 	        !              "      !   ;  "   #         $            *          +      *   ;  +   ,        0            1      0   ;  1   2      +     4     �?   ;         ;  ;   <      ;  ;   =      6               �                 =           A              >                    =  	         A              >                     A  $   %   #      =      &   %   A  $   '   #      =      (   '   �      )   &   (   A  $   -   ,      =      .   -   �      /   )   .   =  0   3   2   Q     5   3       Q     6   3      Q     7   3      P  	   8   5   6   7   4   �  	   9   /   8   A     :         >  :   9   �  8  