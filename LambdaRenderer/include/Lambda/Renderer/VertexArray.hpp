#pragma once

#include "Lambda/Core/Core.hpp"

#include "Buffer.hpp"

#include <memory>

namespace Lambda {

    class LAMBDA_API VertexArray {
       public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(ref<VertexBuffer> const& buffer,
                                     BufferLayout const& layout) = 0;
        virtual void SetIndexBuffer(ref<IndexBuffer> const& buffer) = 0;

       public:
        static ref<VertexArray> Create();
    };

}  // namespace Lambda
