#version 330

attribute vec3 position;
attribute vec3 normal;
attribute vec4 Ka;
attribute vec4 Kd;
attribute vec4 Ks;
attribute vec2 texcoord;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;
out float fogFactor;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;

const float density = 0.07;
const float gradient = 0.8f;

void main()
{
	
	texCoord0 = texcoord;
	color0 = vec3(Ka);
	normal0 = (Model  * vec4(normal, 0.0)).xyz;
	position0 = vec3(Proj *View *Model * vec4(position, 1.0));
	gl_Position = Proj *View * Model* vec4(position, 1.0); //you must have gl_Position


	//fog:
	vec4 pos_rel = View *Model *vec4(position,1.0);

	float dist = length(pos_rel.xyz);
	fogFactor = exp(-pow((dist*density),gradient));
	fogFactor = clamp(fogFactor,0.0f,1.0f);
}
