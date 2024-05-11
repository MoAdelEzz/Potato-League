
#version 330 core

uniform sampler2D tex;
uniform float time; // Time parameter to control wave movement

in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Apply a sine wave distortion to the texture coordinates



    vec2 distorted_coord = tex_coord;
    distorted_coord.x += sin(tex_coord.y * 0.001 + time) * 0.01; // Horizontal wave distortion
    distorted_coord.y += sin(tex_coord.x * 0.001 + time) * 0.01; // Vertical wave distortion

    frag_color = texture(tex, distorted_coord);

    vec2 new_tex_coord = tex_coord * 2 - 1;
    frag_color = frag_color / (1+(new_tex_coord[0]*new_tex_coord[0] + new_tex_coord[1]*new_tex_coord[1]));
}