#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel


    vec2 tex_coord_green = tex_coord;
    vec4 frag_color_green = texture(tex, tex_coord_green);

    vec2 tex_coord_red = tex_coord;
    tex_coord_red[0]=tex_coord_red[0]-STRENGTH;
    vec4 frag_color_red = texture(tex, tex_coord_red);

    vec2 tex_coord_blue = tex_coord;
    tex_coord_blue[0]=tex_coord_blue[0]+STRENGTH;
    vec4 frag_color_blue = texture(tex, tex_coord_blue);


    frag_color = vec4(frag_color_red[0],frag_color_green[1],frag_color_blue[2],frag_color_green[3]);
}