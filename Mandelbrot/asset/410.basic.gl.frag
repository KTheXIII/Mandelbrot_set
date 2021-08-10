#version 410 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 color;

in vec4 io_color;
in vec2 io_uv;

uniform vec4 u_color;
uniform vec2 u_resolution;
uniform sampler2D u_texture;

void main() {
    color = io_color;
    // color = texture(u_texture, io_uv);
}