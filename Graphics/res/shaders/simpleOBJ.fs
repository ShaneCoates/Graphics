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
	float d = max(0, dot(normalize(normalInterp.xyz), LightDir));
	vec3 E = normalize(CameraPos - vertPos.xyz);
	vec3 R = reflect(-LightDir, normalInterp.xyz);
	float s = max(0, dot(E, R));
	s = pow(s, 4);

	FragColor = texture(diffuse, vTexCoord) * vec4(vec3(1, 1, 1) * d + vec3(1, 1, 1) * s,  1);
	FragColor.a = 1;
};