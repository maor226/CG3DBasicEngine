#version 330

#define IN_CERCEL(x,y)  (x*x+y*y < coeffs.z * coeffs.z)
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 coeffs;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;

uniform int POINTS_NUM;
uniform vec4[7] bez_points; 

out vec4 Color;

vec3 bezier(vec3 A, vec3 B, vec3 C, vec3 D, float t) {
  vec3 E = mix(A, B, t);
  vec3 F = mix(B, C, t);
  vec3 G = mix(C, D, t);

  vec3 M = mix(E, F, t);
  vec3 I = mix(F, G, t);

  vec3 P = mix(M, I, t);

  return P;
}

float f(float x, int idx) {
    return dot(coeffs , vec4(x*x*x, x*x, x, 1));
}
float distanceFromPoinSqr(vec3 position, vec4 point)
{
  return (position.s - point.x)*(position.s-point.x) + (position.t-point.y)*(position.t-point.y);
}


void main()
{
  vec3 pos = position0;
  int x = 0;
    // if(IN_CERCEL(pos.x, pos.y))
    for(int i = 0; i< POINTS_NUM; i++){
        if(distanceFromPoinSqr(position0, bez_points[i]*2.4) < 0.04){
         x = 1;
        }
    }
    if(x == 1)
      Color = vec4(0.9373, 0.0, 0.4392, 1.0); // texture2D(sampler1, texCoord0)* vec4(color0,1.0); //you must have gl_FragColor
    else 
      discard;
}