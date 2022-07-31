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
uniform int isFog;
//for blur 
uniform float resolution;
uniform float radius;
uniform int is_blur;



out vec4 Color;

float liniarDepht(float depht){
	return (2.0 *near *far) / (far+near - (depht *2.0 -1.0))*(far - near);
}
float logisticDepht(float depht ,float steepness = 0.5f, float offset = 5.0f){
	return (1/1+exp(-steepness *(liniarDepht(depht)-offset))); //for cousing fog effect
}

const vec2 dir= vec2(1.0,0.0f); 
void main()
{
	Color = texture2D(sampler1, texCoord0); //you must have gl_FragColor
	// float c = logisticDepht(gl_FragCoord.z);
	// if ( c<0.1) discard;
	// Color.xyz = vec3(c);
	
	// if (Color.a<0.5)
	// 	discard;
	if(is_blur!=0){
	vec4 sum = vec4(0.0);
    
    //our original texcoord for this fragment
    vec2 tc = texCoord0;
    
    //the amount to blur, i.e. how far off center to sample from 
    //1.0 -> blur by one pixel
    //2.0 -> blur by two pixels, etc.
    float blur = radius/resolution; 
    
    //the direction of our blur
    //(1.0, 0.0) -> x-axis blur
    //(0.0, 1.0) -> y-axis blur
    float hstep = dir.x;
    float vstep = dir.y;
    
    //apply blurring, using a 9-tap filter with predefined gaussian weights
    
    sum += texture2D(sampler1, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
    sum += texture2D(sampler1, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(sampler1, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(sampler1, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;
    
    sum += Color * 0.2270270270;
    
    sum += texture2D(sampler1, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
    sum += texture2D(sampler1, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(sampler1, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(sampler1, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

    Color = sum;
	}
	//transferent 
	Color.a = Color.a*alpha;
	
	// fog : 
	if(isFog!=0)
		Color = mix(vec4(fog_color.xyz, Color.a*alpha),Color,fogFactor);

	
	
}
