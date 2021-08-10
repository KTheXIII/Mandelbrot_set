#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <thread>
#include <type_traits>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>

// requires https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

auto mandelbrot(std::complex<double> z, std::complex<double> c) {
    return z * z + c;
}

int32_t iterate(std::complex<double> z, std::complex<double> c,
                int32_t max, double radius = 2.0) {
    int32_t current = 0;  // Current Iteration
    auto r2 = radius * radius;

    for (int32_t i = 0; i < max; i++) {
        z = mandelbrot(z, c);
        if (z.real() * z.real() + z.imag() * z.imag() > r2) break;
        current++;
    }

    return current;
}

template <typename T, typename = typename std::enable_if<
                          std::is_arithmetic<T>::value, T>::type>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T>
struct vec2 {
    T x;
    T y;
};

struct Settings {
    vec2<double> center{0., 0.};
    double scale = 1.75;
    int32_t width = 512;
    int32_t height = 512;
    int32_t iterations = 512;
};

uint8_t compute_color(float c, float scale, float freq = 12.f) {
    return uint8_t((std::sin(scale * freq * c - M_PI / 2.f) + 1.f) * 0.5f *
                   255.f);
}

constexpr auto USAGE = R"(
    Usage:  mandelbrot [width] [height]

    Options:
            [width]     Width of the mandelbrot image
            [height]    Height of the mandelbrot image
)";

int32_t main(int32_t argc, char const* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    constexpr auto channels = 3;
    Settings settings;

    if (argc == 2) {
        std::cout << USAGE;
        std::exit(0);
    }

    if (argc >= 3) {
        try {
            settings.width = std::stoi(args[1]);
            settings.height = std::stoi(args[2]);
        } catch (std::exception const& e) {
            std::cerr << e.what() << '\n';
        }
    }

    if (argc >= 4) {
        try {
            settings.iterations = std::stoi(args[3]);
        } catch (std::exception const& e) {
            std::cerr << e.what() << '\n';
        }
    }

    if (argc >= 7) {
        try {
            settings.center.x = std::stod(args[4]);
            settings.center.y = std::stod(args[5]);
            settings.scale = std::stod(args[6]);
        } catch (std::exception const& e) {
            std::cerr << e.what() << '\n';
        }
    }

    auto size = settings.width * settings.height;

    std::cout << "Allocating memory...\n";
    std::cout << "Fractals:  " << (size * sizeof(int32_t)) << " bytes\n";
    std::cout << "Image:     " << (size * sizeof(int8_t) * channels)
              << " bytes\n\n";

    std::chrono::duration<double> elapsed;
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    auto fractals = new int32_t[size];
    // std::vector<vec2<double>> positions;
    auto image = new uint8_t[size * channels];

    // for (int32_t i = 0; i < settings.height; i++) {
    //    double y = map<double>(i + 1, 1.0, settings.height, 1.0, -1.0);
    //    for (int32_t j = 0; j < settings.width; j++) {
    //        double x = map<double>(j + 1, 1.0, settings.width, -1.0, 1.0);
    //        positions.push_back({x, y});
    //    }
    //}

    const auto threadCount = int32_t(std::thread::hardware_concurrency());
    std::cout << "Thread count: " << threadCount << '\n';
    std::vector<std::thread> threads;

    auto compute = [&](int32_t startr, int32_t endr, int32_t startc,
                       int32_t endc) {
        auto scale  = settings.scale;
        auto center = settings.center;
        for (int32_t i = startr; i < endr; i++) {
            double y = map<double>(i + 1, 1.0, settings.height, 1.0, -1.0);
            for (int32_t j = startc; j < endc; j++) {
                double x = map<double>(j + 1, 1.0, settings.width, -1.0, 1.0);
                std::complex<double> z{0., 0.};
                std::complex<double> c{x * scale + center.x, y * scale + center.y};
                fractals[i * settings.width + j] = iterate(z, c, settings.iterations);
            }
        }
    };

    std::cout << "Computing fractal....\n";
    start = std::chrono::high_resolution_clock::now();

    const auto offset = settings.width / threadCount;
    for (int32_t i = 0; i < threadCount; i++) {
        threads.emplace_back(compute, 0, settings.height, offset * i,
                             offset * (i + 1));
    }

    for (auto& thread : threads) thread.join();

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time took: " << elapsed.count() << " s\n";

    std::cout << "Generating image...\n";
    start = std::chrono::high_resolution_clock::now();
    int32_t index = 0;
    for (int32_t i = 0; i < settings.height; i++) {
        for (int32_t j = 0; j < settings.width; j++) {
            auto n = fractals[i * settings.width + j];
            if (n < settings.iterations) {
                auto color = std::sqrt(float(n) / float(settings.iterations));
                image[index++] = compute_color(color, .3f);
                image[index++] = compute_color(color, .1f);
                image[index++] = compute_color(color, .5f);
            } else {
                image[index++] = 0;
                image[index++] = 0;
                image[index++] = 0;
            }
        }
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time took: " << elapsed.count() << " s\n";

    stbi_write_png("mandelbrot.png", settings.width, settings.height, channels,
                   image, settings.width * channels);

    std::cout << "Cleaning up...\n";
    delete[] image;
    delete[] fractals;
    std::cout << "Done...\n";

    return 0;
}

