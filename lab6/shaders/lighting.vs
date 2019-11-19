#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
flat out int deltaCos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{

    vec3 Vector_Position = position;
    Vector_Position.y = Vector_Position.y * cos(Vector_Position.y + time);
    gl_Position = projection * view * model * vec4(Vector_Position, 1.0f);
    FragPos = vec3(model * vec4(Vector_Position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    if(normal.x == 0.0 && normal.z == 0.0){
        deltaCos = 0;
    }else{
        deltaCos = 1;
    }
} 