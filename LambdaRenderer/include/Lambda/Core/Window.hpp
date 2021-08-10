/**
 * @file   Window.hpp
 * @author Pratchaya Khansomboon (pratchaya.k.git@gmail.com)
 * @brief  Window and its config.
 * @date 2021-08-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "Core.hpp"
#include "Input.hpp"

#include "Lambda/Renderer/RendererAPI.hpp"

namespace Lambda {

    // clang-format off
    /**
     * @brief Lambda Renderer config initialization.
     */
    struct LAMBDA_API Config {
        std::string Title = "Lambda Renderer";
        int32_t Width     = 1280;
        int32_t Height    = 720;
        int32_t PosX      = INT32_MIN;
        int32_t PosY      = INT32_MIN;
        bool IsFullscreen = false;
        bool IsVSync      = true;
        API API           = API::OpenGL;
    };
    // clang-format on

    /**
     * @brief Window abstraction layer for the Lambda Renderer.
     */
    class LAMBDA_API Window {
       public:
        virtual ~Window() = default;

        virtual void* GetNative() = 0;

       public:
        virtual void OnUpdate() = 0;
        virtual bool ShouldClose() const = 0;
        virtual void Close() = 0;

        virtual std::string GetTitle() const = 0;
        virtual void SetTitle(std::string const& title) = 0;
        virtual void GetSize(int32_t& width, int32_t& height) const = 0;
        virtual int32_t GetWidth() const = 0;
        virtual int32_t GetHeight() const = 0;
        virtual void SetWidth(int32_t const& width) = 0;
        virtual void SetHeight(int32_t const& height) = 0;
        virtual int32_t GetPosX() const = 0;
        virtual int32_t GetPosY() const = 0;
        virtual double GetTime() const = 0;

        virtual KeyState GetKeyState(KeyCode const& keycode) const = 0;
        virtual KeyState GetMouseButton(MouseButton const& button) const = 0;
        virtual void GetMousePos(double& x, double& y) const = 0;

       public:
        static ref<Window> Create(Config const& config = Config());
    };

}  // namespace Lambda
