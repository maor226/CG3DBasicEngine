#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
in float fogFactor;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform float alpha;
//for depht usege
uniform float near;
uniform float far;
uniform vec4 fog_color;//=vec3(0.5f,0.5f,0.5f);


out vec4 Color;

float liniarDepht(float depht){
	return (2.0 *near *far) / (far+near - (depht *2.0 -1.0))*(far - near);
}
float logisticDepht(float depht ,float steepness = 0.5f, float offset = 5.0f){
	return (1/1+exp(-steepness *(liniarDepht(depht)-offset))); //for cousing fog effect
}

void main()
{
	Color = texture2D(sampler1, texCoord0); //you must have gl_FragColor
	// float c = logisticDepht(gl_FragCoord.z);
	// if ( c<0.1) discard;
	// Color.xyz = vec3(c);
	Color.a = Color.a*alpha;
	// if (Color.a<0.5)
	// 	discard;

	// fog : 
	Color = mix(vec4(fog_color.xyz, Color.a*alpha),Color,fogFactor);

	
}
