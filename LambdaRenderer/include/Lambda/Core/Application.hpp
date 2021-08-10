#pragma once

#include <memory>

#include "Core.hpp"
#include "Window.hpp"
#include "Input.hpp"

#include "Lambda/Renderer/Buffer.hpp"
#include "Lambda/Renderer/Shader.hpp"

namespace Lambda {

    class LAMBDA_API Lambda {
       public:
        struct Version {
            int32_t MAJOR;
            int32_t MINOR;
            int32_t PATCH;
        };
        static Version GetVersion();
        static char const* GetVersionString();
        static char const* GetName();

       public:
        Lambda(Config const& prop = Config());
        ~Lambda();

        std::shared_ptr<Window> GetWindow() const { return m_Window; }
        inline bool ShouldClose() { return m_Window->ShouldClose(); }
        KeyState GetKeyState(KeyCode const& keycode) const;
        KeyState GetMouseButton(MouseButton const& button) const;
        void GetMousePos(double& x, double& y) const;

        void Update();

       private:
        ref<Window> m_Window;

       private:
        static Version s_Version;
    };

}  // namespace Lambda
