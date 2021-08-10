#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uv;

out vec4 io_color;
out vec2 io_uv;

void main() {
    io_color = a_color;
    io_uv    = a_uv;

    gl_Position = vec4(a_position, 1.0f);
}
