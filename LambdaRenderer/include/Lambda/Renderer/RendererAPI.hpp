#pragma once

#include "Lambda/Core/Core.hpp"
#include "Lambda/Core/Core.hpp"

#include "Buffer.hpp"
#include "VertexArray.hpp"

#include <cstdint>
#include <string>
#include <memory>

namespace Lambda {

    enum class LAMBDA_API API : int32_t {
        None = -1,
        OpenGL = 0,
    };

    class LAMBDA_API RendererAPI {
       public:
        inline static API GetAPI() { return s_API; }
        static char const* GetAPIString();
        static void SetAPI(API const& api);

        static RendererAPI* Create();

       public:
        virtual ~RendererAPI() = default;

        virtual void SetClearColor(f64 const& red, f64 const& green,
                                   f64 const& blue, f64 const& alpha) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(ref<VertexBuffer> const& vb,
                                 ref<IndexBuffer> const& ib,
                                 ref<VertexArray> const& va) = 0;

       private:
        static API s_API;
    };

}  // namespace Lambda
