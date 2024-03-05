#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;
out vec4 fPosEye;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceTrMatrix;

//spotlight
out vec4 fragPosLightSpace;

void main() 
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
    fPosEye = view * model * vec4(vPosition, 1.0f);
    fPosition = vec3(model * vec4(vPosition, 1.0f));
    fNormal = mat3(transpose(inverse(model))) * vNormal;
    fTexCoords = vTexCoords;
    fragPosLightSpace = model * vec4(vPosition, 1.0f);
}
