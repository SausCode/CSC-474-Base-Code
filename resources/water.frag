//
//#version 410 core
//out vec4 color;
//in vec3 vertex_normal;
//in vec3 vertex_pos;
//in vec2 vertex_tex;
//uniform vec3 campos;
//
//uniform sampler2D tex;
//uniform sampler2D tex2;
//
//void main()
//{
//vec3 n = normalize(vertex_normal);
//vec3 lp=vec3(10,-20,-100);
//vec3 ld = normalize(vertex_pos - lp);
//float diffuse = dot(n,ld);
//
//vec4 tcol = texture(tex, vertex_tex);
//color = tcol;
//color.a = (color.r + color.g + color.b)/3.;
//}

#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;
in vec3 lightPos;

out vec4 color;

void main() {
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 normal = normalize(fragNor);
    
    // Ambient calculation
    vec3 ambientColor = vec3(0);
    
    // Diffuse calculation
//  float diffuseFactor = max(dot(lightDir, normal), 0); // Clamp to prevent color from reaching back side
    float diffuseFactor = (dot(lightDir, normal) + 1) / 2; // Normalize so the color bleeds onto the back side
    vec3 diffuseColor = vec3(0.8, 0.2, 0.5) * diffuseFactor;
    
    // Specular calculation
    float specularStrength = 0.5f;
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 3.0f;
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0), shininess);
    vec3 specularColor = vec3(1) * specularFactor * specularStrength;

    color.rgba = vec4(ambientColor + diffuseColor + specularColor, 1);

	//color.rgb = vec3(0,0,1);
}

