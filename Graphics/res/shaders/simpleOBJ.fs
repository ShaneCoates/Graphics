#version 410
precision mediump float;

in vec3 normalInterp;
in vec3 vertPos;
in vec2 vTexCoord; 

uniform sampler2D diffuse; 
uniform vec3 LightDir;
uniform vec3 CameraPos;
out vec4 FragColor;

void main()
{
   vec3 E = normalize(-vertPos); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(LightDir,normalInterp));  
 
   //calculate Ambient Term:  
   vec4 Iamb = vec4(0.1,0.1,0.1,0.1);    

   //calculate Diffuse Term:  
   vec4 Idiff = vec4(1,1,1,1)* max(dot(normalInterp,LightDir), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = vec4(0.1,0.1,0.1,0.1)
                * pow(max(dot(R,E),0.0),0);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   // write Total Color:  
   FragColor = texture(diffuse, vTexCoord) * (Iamb + Idiff + Ispec);     

};