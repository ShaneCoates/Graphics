#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 Normal; 

out vec4 vPosition;
out vec3 vTexCoords;
out vec3 vNormal;

uniform mat4 ProjectionView;
uniform sampler3D perlinTexture;
uniform float time;
uniform float amplitude;
uniform float size;
void main() {
	vec2 tex = TexCoords;

	vTexCoords = vec3(tex, 0);
	
	vec3 pos3d = vec3(Position);
	pos3d.x /= (50 * size);
	pos3d.y /= (50 * size);
	pos3d.z /= (50 * size);
	
	float height = texture(perlinTexture, pos3d).r;

	vec4 pos = Position;
	pos.x *= size; 
	pos.z *= size;
	pos.y *= size;
	//pos += vec4(vNormal, 1) * (1 - height ) * 0.01f;
	vPosition = pos;
	gl_Position = ProjectionView * vPosition;
	vNormal = Normal;
}