#version 330

#define COEFF_SIZE 20
#define NUM_POINTS 13
#define NUM_POLS (NUM_POINTS - 1)/3
#define epsilon 0.05

#define IN_RANGE(x0,x1,x2) (x0 > x1 && x0 < x2)
in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4[COEFF_SIZE] coeffs;
uniform vec3[NUM_POINTS] bez_points;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;

out vec4 Color;

vec3 bezier(vec3 A, vec3 B, vec3 C, vec3 D, float t) {
  vec3 E = mix(A, B, t);
  vec3 F = mix(B, C, t);
  vec3 G = mix(C, D, t);

  vec3 H = mix(E, F, t);
  vec3 I = mix(F, G, t);

  vec3 P = mix(H, I, t);

  return P;
}

float f(float x, int idx) {
    return dot(coeffs[idx] , vec4(x*x*x, x*x, x, 1));
}

float smoothstep(x, edge0, edge1) {
  genType t;  /* Or genDType t; */
  t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
  return t * t * (3.0 - 2.0 * t);
}

int try_plot() {
  float x = position0.x
  for(int i = 0 ; i < NUM_POLS ; i++){
    float start_x = bez_points[i*3].x, end_x = bez_points[(i+1)*3].x;

    if(!IN_RANGE(x, start_x, end_x))
      continue;
    
    float t = (x - start_x)/(end_x - start_x);
    vec3 bez_t = bezier(bez_points[i], bez_points[i + 1], bez_points[i + 2], bez_points[i + 3], t);
    float y_val = bez_t.y;
    if(smoothstep(position0.y, y_val - epsilon, y_val) - smoothstep(position0.y, y_val, y_val + epsilon))
      return vec4()
  }
  return -1;
}

void main()
{
    if(IN_RANGE(f(position0.x) , position0.y))
	    Color = vec4(1,0,0,1) // texture2D(sampler1, texCoord0)* vec4(color0,1.0); //you must have gl_FragColor
    else 
        Color = vec4(0,0,0,1)
}
