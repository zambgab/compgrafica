#version 300 es

precision mediump float;

in vec3 fragN;
in vec3 fragL;
in vec3 fragV;
in vec2 fragTexCoord;

// Light properties
uniform vec4 Ia, Id, Is;

// Material properties
uniform vec4 Ka, Kd, Ks;
uniform float shininess;

// Diffuse texture sampler
uniform sampler2D diffuseTex;

uniform bool hasExtraTex;
uniform sampler2D diffuseTexNight;
// uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D cloudsTex;

out vec4 outColor;

// Blinn-Phong reflection model
vec4 BlinnPhong(vec3 N, vec3 L, vec3 V, vec2 texCoord) {
  N = normalize(N);
  L = normalize(L);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, shininess);
  }

  if (hasExtraTex){
    specular = specular * texture(specularTex,texCoord).x;
  }

  
  vec4 map_Kd = texture(diffuseTex, texCoord);

  if (hasExtraTex){
    float norm_lamb = (1.0 + dot(N, L)) / 2.0;
    norm_lamb = smoothstep(0.3,0.7,norm_lamb);
    map_Kd = mix(texture(diffuseTexNight, texCoord),map_Kd,norm_lamb);
  }

  vec4 map_Ka = map_Kd;

  vec4 diffuseColor = map_Kd * Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = map_Ka * Ka * Ia;

  return ambientColor + diffuseColor + specularColor;
}

void main() {
  vec4 color;

  color = BlinnPhong(fragN, fragL, fragV, fragTexCoord);
  if (hasExtraTex){
    vec4 cloud_color = texture(cloudsTex, fragTexCoord);
    color = mix(color,cloud_color,cloud_color.x/2.0);
  }


  if (gl_FrontFacing) {

    outColor = vec4(color.xyz,1.0f);
  } else {
    float i = (color.r + color.g + color.b) / 3.0;
    outColor = vec4(i, i, i, 1.0);
  }
}
