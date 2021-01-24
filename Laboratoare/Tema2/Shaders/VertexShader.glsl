#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 object_color;
uniform bool distortPlayer;
uniform bool UI;
uniform bool powerUpOn;
uniform float waveLength;

// Noise for 3d coordinates
float rand3D(in vec3 co){
    return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

float simple_interpolate(in float a, in float b, in float x)
{
   return a + smoothstep(0.0,1.0,x) * (b-a);
}

float interpolatedNoise3D(in float x, in float y, in float z)
{
    // Separate integer and fractional part
    float integer_x = x - fract(x);
    float fractional_x = x - integer_x;

    float integer_y = y - fract(y);
    float fractional_y = y - integer_y;

    float integer_z = z - fract(z);
    float fractional_z = z - integer_z;

    // Random noise between integer parts -> assign random number to grid point ([x:x+1], [y:y+1], [z:z+1])
    float v1 = rand3D(vec3(integer_x, integer_y, integer_z));
    float v2 = rand3D(vec3(integer_x+1.0, integer_y, integer_z));
    float v3 = rand3D(vec3(integer_x, integer_y+1.0, integer_z));
    float v4 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z));

    float v5 = rand3D(vec3(integer_x, integer_y, integer_z+1.0));
    float v6 = rand3D(vec3(integer_x+1.0, integer_y, integer_z+1.0));
    float v7 = rand3D(vec3(integer_x, integer_y+1.0, integer_z+1.0));
    float v8 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z+1.0));

    // Smooth interpolation function between grid points with fractional(z) as step
    float i1 = simple_interpolate(v1,v5, fractional_z);
    float i2 = simple_interpolate(v2,v6, fractional_z);
    float i3 = simple_interpolate(v3,v7, fractional_z);
    float i4 = simple_interpolate(v4,v8, fractional_z);

    // Smooth interpolation function with fractional(x) as step
    float ii1 = simple_interpolate(i1,i2,fractional_x);
    float ii2 = simple_interpolate(i3,i4,fractional_x);

    // Smooth interpolation function with fractional(y) as step
    return simple_interpolate(ii1 , ii2 , fractional_y);
}

float Noise3D(in vec3 coord, in float wavelength)
{
   return interpolatedNoise3D(coord.x/wavelength, coord.y/wavelength, coord.z/wavelength);
}

// Output value to fragment shader
out vec3 color;



void main()
{


	// TODO: send color light output to fragment shader
	color = object_color;

    // Add noise to v_position if powerUpOn
    if(distortPlayer && powerUpOn){
        vec3 new_position = Noise3D(v_position, waveLength) * v_position;
        gl_Position = Projection * View * Model * vec4(new_position, 1.0);    
    }
    else{
        if(UI){
            gl_Position =  Model * vec4(v_position, 1.0);
        }
        else{
            gl_Position = Projection * View * Model * vec4(v_position, 1.0);
        }
    }
}

// Source: http://www.science-and-fiction.org/rendering/noise.html#:~:text=A%20noise%20function%20for%203d,contains%20a%20good%20mixture%20of
