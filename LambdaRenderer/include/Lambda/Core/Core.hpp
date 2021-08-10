#pragma once

#include <memory>

// Windows
#ifdef LAMBDA_PLATFORM_WINDOWS
#define LAMBDA_PLATFORM "Windows"
// #ifdef LAMBDA_BUILD_DLL
// #define LAMBDA_API __declspec(dllexport)
// #else
// #define LAMBDA_API __declspec(dllimport)
// #endif
#define LAMBDA_API
#else
#define LAMBDA_API
#endif

// macOS
#ifdef LAMBDA_PLATFORM_MACOS
#define LAMBDA_PLATFORM "macOS"
#endif

// Linux
#ifdef LAMBDA_PLATFORM_LINUX
#define LAMBDA_PLATFORM "Linux"
#endif

#define LAMBDA_LITERAL(x)   #x
#define LAMBDA_AS_STRING(x) LAMBDA_LITERAL(x)

namespace Lambda {

    typedef double f64;
    typedef float f32;

    template <typename T>
    using ref = std::shared_ptr<T>;

    template <typename T>
    using local = std::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr ref<T> make_ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    constexpr local<T> make_local(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // clang-format off
    enum class Platform : int32_t {
        Unknown = -1,
        Linux   =  1,
        macOS   =  2,
        Windows =  3,
    };
    // clang-format on

    Platform GetPlatform();

}  // namespace Lambda
