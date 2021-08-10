#pragma once

#include "Lambda/Core/Core.hpp"

namespace Lambda {

    class LAMBDA_API GraphicsContext {
       public:
        virtual ~GraphicsContext() = default;

        virtual void SwapBuffers() = 0;
        virtual void ViewPort(int32_t x, int32_t y, int32_t width,
                              int32_t height) = 0;
    };

}  // namespace Lambda
