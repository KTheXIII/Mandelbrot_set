#pragma once

#include <array>
#include <string>

#include "Lambda/Core/Core.hpp"

namespace Lambda {

    class LAMBDA_API Shader {
       public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

       public:
        virtual void SetFloat(const std::string& name,
                              float const& value) const = 0;
        virtual void SetFloat2(const std::string& name,
                               std::array<float, 2> const& value) const = 0;
        virtual void SetFloat3(const std::string& name,
                               std::array<float, 3> const& value) const = 0;
        virtual void SetFloat4(const std::string& name,
                               std::array<float, 4> const& value) const = 0;

        virtual std::string const& GetName() const = 0;

       public:
        static ref<Shader> Create(std::string const& name,
                                  std::string const& vspath,
                                  std::string const& fspath);
    };

}  // namespace Lambda
