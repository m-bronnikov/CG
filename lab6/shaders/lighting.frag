#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
flat in int deltaCos;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 strenght;

void main()
{
    // Ambient
    float ambientStrength = strenght.x;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    float difStrenght = strenght.y;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    if(deltaCos == 0){
        diff = max(diff, max(dot(-norm, lightDir), 0.0));
    }
    vec3 diffuse = difStrenght * diff * lightColor;
    
    // Specular
    float specularStrength = strenght.z;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    vec3 darksideDir = reflect(-lightDir, -norm);
    float maximum = dot(viewDir, reflectDir);
    if(deltaCos == 0){
        maximum = max(maximum, dot(viewDir, darksideDir));
    }
    float spec = pow(max(maximum, 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
} 