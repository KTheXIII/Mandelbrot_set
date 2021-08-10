#pragma once

#include "Lambda/Core/Core.hpp"
#include "Lambda/Core/Application.hpp"

#include "RendererAPI.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"

#include <memory>

namespace Lambda {

    class LAMBDA_API RenderCommand {
       public:
        inline static void SetClearColor(f64 const& red, f64 const& green,
                                         f64 const& blue,
                                         f64 const& alpha = 1.0) {
            s_RendererAPI->SetClearColor(red, green, blue, alpha);
        }
        inline static void Clear() { s_RendererAPI->Clear(); }
        inline static void Draw(ref<VertexBuffer> const& vb,
                                ref<IndexBuffer> const& ib,
                                ref<VertexArray> const& va) {
            s_RendererAPI->DrawIndexed(vb, ib, va);
        }

        static void SetClearColor(uint32_t const& color);
        static void SetRendererAPI(RendererAPI* api);

       private:
        friend class Lambda;
        static void CleanUp();

       private:
        static RendererAPI* s_RendererAPI;
    };

}  // namespace Lambda
