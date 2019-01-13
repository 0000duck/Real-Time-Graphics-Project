#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float bumpFactor;

float ShadowCalculation(vec4 fragPosLightSpace)
{  
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fs_in.Normal);

    vec3 lightDir = normalize(lightPos - fs_in.TangentFragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0f;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal.r = (normal.r - 0.5 ) * bumpFactor + 0.5;
    normal.g = (normal.g - 0.5) * bumpFactor + 0.5;

    normal = normalize(normal * 2.0 - 1.0);

    // with sinus cosinus

    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    //FragColor = vec4(ambient + diffuse + specular, 1.0);

    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(0.1, 0.1, 1.0, 1.0);
}

// #version 330 core
// out vec4 FragColor;

// in VS_OUT {
//     vec3 FragPos;
//     vec2 TexCoords;
//     vec4 FragPosLightSpace;
//     vec3 TangentLightPos;
//     vec3 TangentViewPos;
//     vec3 TangentFragPos;
//     vec3 Normal;
// } fs_in;

// uniform sampler2D diffuseMap;
// uniform sampler2D normalMap;

// uniform vec3 lightPos;
// uniform vec3 viewPos;
// uniform float bumpFactor;

// void main()
// {           
//      // obtain normal from normal map in range [0,1]
//     //vec3 normal = (texture(normalMap, fs_in.TexCoords).rgb - fs_in.Normal) * bumpFactor + fs_in.Normal;
//     vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
//     normal.r = (normal.r - 0.5 ) * bumpFactor + 0.5;
//     normal.g = (normal.g - 0.5) * bumpFactor + 0.5;
//     //vec3 normal = normalize(fs_in.Normal);
//     // transform normal vector to range [-1,1]
//     normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
//     // get diffuse color
//     vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
//     // ambient
//     vec3 ambient = 0.1 * color;
//     // diffuse
//     vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
//     float diff = max(dot(lightDir, normal), 0.0);
//     vec3 diffuse = diff * color;
//     // specular
//     vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     vec3 halfwayDir = normalize(lightDir + viewDir);  
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

//     vec3 specular = vec3(0.2) * spec;
//     FragColor = vec4(ambient + diffuse + specular, 1.0);
// }