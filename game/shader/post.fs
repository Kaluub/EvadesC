#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float render_width;
uniform float render_height;

out vec4 finalColor;

float scale = 5.0;

void main()
{
    float frequency = render_height / scale;

    float globalPos = fragTexCoord.y * frequency;
    float wavePos = cos((fract(globalPos) - 0.5) * 3.14159);

    vec4 texelColor = texture(texture0, fragTexCoord);

    finalColor = mix(vec4(0.0, 0.3, 0.0, 0.0), texelColor, wavePos);
}