#pragma once

#include <vector>
#include <initializer_list>
#include <cstdint>
#include <iterator>

#include "Lambda/Core/Core.hpp"

namespace Lambda {

    enum class BufferType : uint32_t {
        Bool,
        Byte,
        UByte,
        Short,
        UShort,
        Int,
        UInt,
        Fixed,
        Half,
        Float,
        Double
    };

    struct BufferElement {
        BufferType Type;
        uint32_t Count;
        uint8_t Normalized;
    };

    class LAMBDA_API BufferLayout {
       public:
        BufferLayout();
        BufferLayout(std::initializer_list<BufferElement> const& elements);
        ~BufferLayout();

        void Append(std::initializer_list<BufferElement> const& elements);
        std::vector<BufferElement> const& GetElements() const {
            return m_Elements;
        }

       private:
        std::vector<BufferElement> m_Elements;
    };

    class LAMBDA_API VertexBuffer {
       public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

       public:
        static ref<VertexBuffer> Create(void const* vertices,
                                        uint32_t const& size);
    };

    class LAMBDA_API IndexBuffer {
       public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual int32_t GetCount() const = 0;

       public:
        static ref<IndexBuffer> Create(uint32_t const* indices,
                                       uint32_t const& count);
    };

}  // namespace Lambda
