#include <iostream>
#include <cstdint>

#include "Lambda/Lambda.hpp"

// clang-format off
float vertices[] = {
//  positions: x,y,z        colors: r,g,b,a            texture coordinates
     1.0f,  1.0f,  0.0f,    1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
     1.0f, -1.0f,  0.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
};

uint32_t indices[] = {
    0, 1, 2,
    0, 2, 3,
};
// clang-format on

int32_t main(int32_t argc, char const* argv[]) {
    Lambda::Config config;
    config.Title = "Mandelbrot Set";
    config.API   = Lambda::API::OpenGL;
    config.PosY  = -800;
    Lambda::Lambda lambda(config);

    using KeyCode  = Lambda::KeyCode;
    using KeyState = Lambda::KeyState;
    auto isRunning = true;

    auto shader =
        Lambda::Shader::Create("basic", "Mandelbrot/asset/410.basic.gl.vert",
                               "Mandelbrot/asset/410.basic.gl.frag");
    Lambda::BufferLayout layout{
        {Lambda::BufferType::Float, 3, 0},
        {Lambda::BufferType::Float, 4, 0},
        {Lambda::BufferType::Float, 2, 0},
    };

    auto va = Lambda::VertexArray::Create();
    auto vb = Lambda::VertexBuffer::Create(vertices, sizeof(vertices));
    auto ib = Lambda::IndexBuffer::Create(indices, sizeof(indices));
    va->AddVertexBuffer(vb, layout);

    double mouseX, mouseY;
    Lambda::RenderCommand::SetClearColor(0x000000);
    while (isRunning) {
        isRunning = !lambda.ShouldClose() &&
                    lambda.GetKeyState(KeyCode::ESCAPE) != KeyState::PRESS;
        Lambda::RenderCommand::Clear();
        shader->Bind();
        Lambda::RenderCommand::Draw(vb, ib, va);
        lambda.Update(); // Update and poll the inputs
    }

    return 0;
}
