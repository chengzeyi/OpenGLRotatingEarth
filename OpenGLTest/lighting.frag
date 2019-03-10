#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;

out vec4 fragColor;

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    float diffuseIntensity;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float cosCutoff;
};

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform int gNumSpotLights;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gSampler;

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0f);
    vec3 vertexToEye = normalize(gEyeWorldPos - worldPos0);
    vec3 lightReflect = normalize(reflect(lightDirection, normal));

    float diffuseFactor = dot(normal, normalize(-lightDirection));
    float specularFactor = dot(vertexToEye, normal) > 0.0f ? dot(vertexToEye, lightReflect) : 0.0f;

    vec4 diffuseColor;
    vec4 specularColor;

    if (diffuseFactor > 0.0f)
    {
        diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
    }
    else
    {
        diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    if (specularFactor > 0.0f && diffuseFactor > 0.0f)
    {
        specularColor = vec4(light.color * gMatSpecularIntensity * pow(specularFactor, gSpecularPower), 1.0f);
    }
    else
    {
        specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return ambientColor + diffuseColor + specularColor;
}

vec4 calcDirectionalLight(vec3 normal)
{
    return calcLightInternal(gDirectionalLight.base, gDirectionalLight.direction, normal);
}

vec4 calcPointLight(PointLight light, vec3 normal)
{
    vec3 lightDirection = worldPos0 - light.position;
    float dist = length(lightDirection);

    vec4 color = calcLightInternal(light.base, lightDirection, normal);
    float attenuation = light.atten.constant + light.atten.linear * dist + light.atten.quadratic * dist * dist;
    return color / attenuation;
}

vec4 calcSpotLight(SpotLight light, vec3 normal)
{
    vec3 lightToPixel = normalize(worldPos0 - light.base.position);
    float spotFactor = dot(lightToPixel, normalize(light.direction));

    if (spotFactor > light.cosCutoff)
    {
        vec4 color = calcPointLight(light.base, normal);
        return color * (1.0f - (1.0f - spotFactor) * 1.0f / (1.0f - light.cosCutoff));
    }
    else
    {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

void main()
{
    vec3 normal = normalize(normal0);
    vec4 totalLight = calcDirectionalLight(normal);

    for (int i = 0; i < gNumPointLights; i++)
    {
        totalLight += calcPointLight(gPointLights[i], normal);
    }

    for (int i = 0; i < gNumSpotLights; i++)
    {
        totalLight += calcSpotLight(gSpotLights[i], normal);
    }
    
    fragColor = texture2D(gSampler, texCoord0.xy) * totalLight;
}
