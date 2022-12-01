#version 450 core

uniform vec3 directionalLightColor;
uniform vec3 directionalLightDirection;
uniform vec3 directionalLightIntensity;
uniform vec3 ambientLight;
uniform vec3 viewPosition;

in vec3 fragnormal;
in vec3 modelcoords;

out vec4 FragColor;

// Blinn-Phong lighting model
vec3 illumination() {
    vec3 normal = normalize(fragnormal);
    vec3 lightDirection = normalize(directionalLightDirection);
    vec3 viewDirection = normalize(viewPosition - modelcoords);
    vec3 halfVector = normalize(lightDirection + viewDirection);
    float diffuse = max(dot(normal, lightDirection), 0.0);
    float specular = pow(max(dot(normal, halfVector), 0.0), 32.0);

    return ambientLight + directionalLightColor * directionalLightIntensity * (diffuse + specular);
}

void main() {
    vec3 color = vec3(0.5, 0.5, 0.5);
    vec3 light = illumination();

    FragColor = vec4(color * light, 1.0);
}
