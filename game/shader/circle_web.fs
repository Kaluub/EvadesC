#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform float gradient_max; // default 0.3
uniform float gradient_intensity; // default 0.3
uniform vec3 gradient_color; // default vec3(1.0, 1.0, 1.0)

void main() {
    float grad = gradient_max - (fragTexCoord.x * 0.5 + fragTexCoord.y * 0.5);
    gl_FragColor = (fragColor + vec4(gradient_intensity * grad * gradient_color, 0.0)) * texture2D(texture0, fragTexCoord);
}