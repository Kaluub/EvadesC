#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float gradient_max; // default 0.3
uniform float gradient_intensity; // default 0.3
uniform vec3 gradient_color; // default vec3(1, 1, 1)

out vec4 finalColor;

void main() {
    float grad = gradient_max - (fragTexCoord.x/2 + fragTexCoord.y/2);
    finalColor = (fragColor + vec4(gradient_intensity * grad * gradient_color, 0)) * texture(texture0, fragTexCoord);
}