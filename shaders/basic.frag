#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;
in vec4 fPosEye;
in vec4 fragPosLightSpace;

out vec4 fColor;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;

//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;

// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

//components
vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;
vec3 colorPoint = vec3(0.0f);

//fog
uniform float fogDensity;
uniform vec4 fogColor;

//auxiliar data
vec3 ambient1 = vec3(0.0f);
vec3 diffuse1 = vec3(0.0f);
vec3 specular1 = vec3(0.0f);


// uniform spotlight
uniform vec3 lampColor;
uniform vec3 position;

//spotlight
in vec4 lightPosEye ;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;
float attenuation;

float computeFog()
{

    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
 	float fragmentDistance = length(fPosEye);
 	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

 	return clamp(fogFactor, 0.0f, 1.0f);
}
// spotDir =vec3(-0.042354f, 1.343869f, -1.765486f)
void calcPointLight(){

	vec3 normalEye = normalize(fNormal);	

    vec4 lightPosEye1 = view * model * vec4(position, 1.0f);
    vec3 lightPosEye = lightPosEye1.xyz;

    //compute light direction
    vec3 lightDirN = normalize(lightPosEye - fPosEye.xyz);

    //compute view direction 
    vec3 viewDirN = normalize(-fPosEye.xyz);
    vec3 reflection = normalize(reflect(lightDirN, normalEye));
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    
    float distance = length(lightPosEye - fPosEye.xyz); 
    float attenuation = 1.0 / (constant + linear * distance + 
                 quadratic * (distance * distance));
                 
    vec3 ambientp = attenuation * ambientStrength *lampColor;
    vec3 diffusep = attenuation * max(dot(normalEye, lightDirN), 0.0f) * lampColor;
    vec3 specularp = attenuation * specularStrength * specCoeff * lampColor;
    
    ambientp *= texture(diffuseTexture, fTexCoords);
    diffusep *= texture(diffuseTexture, fTexCoords);
    specularp *= texture(specularTexture, fTexCoords);

    vec3 colorPointNew = min((ambient + diffuse) + specular, 1.0f);
    colorPoint = colorPoint + colorPointNew;
    ambient1 += ambientp;
    diffuse1 += diffusep;
    specular1 += specularp;
}


void computeDirLight()
{
    //compute eye space coordinates
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin)
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = ambientStrength * lightColor;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;
}

void main() 
{
    computeDirLight();
    ambient *= texture(diffuseTexture, fTexCoords).rgb;
    diffuse *= texture(diffuseTexture, fTexCoords).rgb;
    specular *= texture(specularTexture, fTexCoords).rgb;
    
    //pointLight
    calcPointLight();
    ambient += ambient1;
    diffuse += diffuse1;
    specular += specular1;

    vec3 color = min((ambient + diffuse) + specular , 1.0f);

    //fog
    float fogFactor = computeFog();

    fColor = mix(fogColor, vec4(color, 1.0f), fogFactor);
}
