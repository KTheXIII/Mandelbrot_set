#pragma once

#include <cstdint>
#include "Lambda/Core/Core.hpp"

namespace Lambda {

    class LAMBDA_API Texture {
       public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

       public:
        virtual void Bind(uint32_t const& slot = 0) const = 0;
        virtual void Unbind() const = 0;
        virtual void SetData(void* data, uint32_t const& size) = 0;

       public:
        static ref<Texture> Create(uint32_t const& width,
                                   uint32_t const& height);
    };

}  // namespace Lambda
