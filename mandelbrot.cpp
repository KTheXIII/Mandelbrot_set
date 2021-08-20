#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <type_traits>
#include <stdexcept>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>

// requires https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

auto mandelbrot(std::complex<double> z, std::complex<double> c) {
    return z * z + c;
}

// TODO: Burning ship fractal
//       z_{n+1} = (|Re(z_n)| + i|Im(z_n)|)^2 + c, z_0 = 0
//       https://en.wikipedia.org/wiki/Burning_Ship_fractal
// TODO: Make tricorn implementation with degree of freedom calculation
//       f(z) = z^d + c, where d is can change
//       https://en.wikipedia.org/wiki/Tricorn_%28mathematics%29
// TODO: Better color generation

int32_t iterate(std::complex<double> z, std::complex<double> c, int32_t max,
                double radius = 2.0) {
    int32_t current = 0; // Current Iteration
    auto r2         = radius * radius;

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

uint8_t compute_color(float c, float scale, float freq = 12.f) {
    return uint8_t((std::sin(scale * freq * c - M_PI / 2.f) + 1.f) * 0.5f *
                   255.f);
}

struct Config {
    double x          = 0.0;
    double y          = 0.0;
    double scale      = 1.75;
    int32_t width     = 1024;
    int32_t height    = 1024;
    int32_t iteration = 512;
};

struct Flags {
    std::string filename = "mandelbrot.png";
    int32_t recursion    = 3;
    bool multithread     = true;
    bool write           = true;
};

std::ostream& operator<<(std::ostream& os, Config const& config) {
    os << "Config:\n";
    os << "  size:       " << config.width << "x" << config.height << '\n';
    os << "  scale:      " << config.scale << '\n';
    os << "  iterations: " << config.iteration << '\n';
    os << "  position:   " << config.x << "," << config.y << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, Flags const& flags) {
    os << "Flags:\n";
    os << "  recursion:     " << flags.recursion << '\n';
    os << "  multithreaded: " << (flags.multithread ? "true" : "false") << '\n';
    os << "  write:         " << (flags.write ? "true" : "false") << '\n';
    return os;
}

template <typename T> class Image {
  public:
    Image(int32_t const width, int32_t const height, int32_t const channels = 3)
        : m_Width(width), m_Height(height), m_Channels(channels) {
        m_Buffer = new T[m_Width * m_Height * m_Channels];
    }
    ~Image() { delete[] m_Buffer; }

    T* GetBuffer() const { return m_Buffer; }

    int32_t GetWidth() const { return m_Width; }
    int32_t GetHeight() const { return m_Height; }
    int32_t GetChannels() const { return m_Channels; }

    uint32_t GetPixelRGB(int32_t const& x, int32_t const& y) const {
        const auto i   = y * m_Width * m_Channels + x * m_Channels;
        uint32_t color = 0x00000000;
        color |= (m_Buffer[i + 0] << 16);
        color |= (m_Buffer[i + 1] << 8);
        color |= (m_Buffer[i + 2] << 0);
        return color;
    }
    void SetPixelRGB(int32_t const& x, int32_t const& y, T const& red,
                     T const& green, T const& blue) {
        const auto i    = y * m_Width * m_Channels + x * m_Channels;
        m_Buffer[i + 0] = red;
        m_Buffer[i + 1] = green;
        m_Buffer[i + 2] = blue;
    }
    void SetPixelRGBA(int32_t const& x, int32_t const& y, T const& red,
                      T const& green, T const& blue, T const& alpha) {
        if (m_Channels != 4) return; // Return because there's only 3 channels
        const auto i    = y * m_Width * m_Channels + x * m_Channels;
        m_Buffer[i + 0] = red;
        m_Buffer[i + 1] = green;
        m_Buffer[i + 2] = blue;
        m_Buffer[i + 3] = alpha;
    }

  private:
    int32_t m_Width, m_Height;
    int32_t m_Channels;
    T* m_Buffer;
};

constexpr auto USAGE = R"(
Usage: mandelbrot [options] [filename]

Options:
    [filename]          specify the output filename

    -h, --help          show list of command-line options
    -d, --dimension     set image size, width and height
                        example: -s 512,     will result in 512x512
                                 -s 480 360, will result in 480x360
    -i, --iteration     set maximum iterations
    -p, --position      set position
                        example: -p 0.1 0.5
    -s, --scale         set zoom scale

    --multithreaded     set flag for multithreading
    --singlethreaded    set flag for singlethreaded
    -r, --recursion     set maximum multithreaded recursion partition
    --no-output         don't output the image
)";

void parse_args(Flags& flags, Config& config, std::vector<std::string>& args) {
    for (int32_t i = 0; i < args.size(); i++) {
        auto curr = args.at(i);
        if (curr == "-h" || curr == "--help") {
            std::cout << USAGE << '\n';
            std::exit(0);
        }

        if (curr == "-d" || curr == "--dimension") {
            auto width = i < args.size() - 1 ? args.at(++i) : "";
            if (width.empty()) {
                std::cerr
                    << "flag -d is not in correct format, example: -d 512\n";
                std::exit(1);
            }
            auto height = i < args.size() - 1 ? args.at(i + 1) : width;
            if (height.at(0) == '-') height = width;
            else
                i++;

            try {
                config.width  = std::stoi(width);
                config.height = std::stoi(height);
                continue;
            } catch (std::invalid_argument const& e) {
                std::cerr << "error in flag -d: " << e.what() << '\n';
                std::exit(1);
            }
        }

        if (curr == "-i" || curr == "--iteration") {
            auto iteration = i < args.size() - 1 ? args.at(++i) : "";
            if (iteration.empty()) {
                std::cerr
                    << "flag -i is not in correct format, example: -i 256\n";
                std::exit(1);
            }

            try {
                config.iteration = std::stoi(iteration);
                continue;
            } catch (std::invalid_argument const& e) {
                std::cerr << "error in flag -i: " << e.what() << '\n';
                std::exit(1);
            }
        }

        if (curr == "-p" || curr == "--position") {
            auto x = i < args.size() - 1 ? args.at(++i) : "";
            auto y = i < args.size() - 1 ? args.at(++i) : "";

            if (x.empty() || y.empty()) {
                std::cerr << "flag -p is not in correct format, example: -p "
                             "0.1 0.5\n";
                std::exit(1);
            }

            try {
                config.x = std::stod(x);
                config.y = std::stod(y);
            } catch (std::invalid_argument const& e) {
                std::cerr << "error in flag -p: " << e.what() << '\n';
                std::exit(1);
            }
        }

        if (curr == "-s" || curr == "--scale") {
            auto scale = i < args.size() - 1 ? args.at(++i) : "";
            if (scale.empty()) {
                std::cerr
                    << "flag -s is not in correct format, example: -s 2.0\n";
                std::exit(1);
            }

            try {
                config.scale = std::stod(scale);
            } catch (std::invalid_argument const& e) {
                std::cerr << "error in flag -s: " << e.what() << '\n';
                std::exit(1);
            }
        }

        if (curr == "-r" || curr == "--recursion") {
            auto recursion = i < args.size() - 1 ? args.at(++i) : "";
            if (recursion.empty()) {
                std::cerr
                    << "flag -r is not in correct format, example: -r 3\n";
                std::exit(1);
            }

            try {
                flags.recursion = std::stoi(recursion);
                if (flags.recursion < 0)
                    throw std::runtime_error(
                        "negative recursion depth is not allowed");
            } catch (std::exception const& e) {
                std::cerr << "error in flag -r: " << e.what() << '\n';
                std::exit(1);
            }
        }

        if (curr == "--multithreaded") flags.multithread = true;
        if (curr == "--singlethreaded") flags.multithread = false;
        if (curr == "--no-output") flags.write = false;

        if (curr.at(0) != '-') {
            flags.filename = curr;
        }
    }

    if (args.size() == 1) {
        std::cout << USAGE << '\n';
        std::exit(0);
    }
}

int32_t main(int32_t argc, char const* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    Flags flags;
    Config config;
    parse_args(flags, config, args);

    std::cout << config << '\n';
    std::cout << flags << '\n';

    auto fractals = new int32_t[config.width * config.height];

    auto compute = [&](int32_t startr, int32_t endr, int32_t startc,
                       int32_t endc) {
        auto scale = config.scale;
        auto ratio = double(config.width) / double(config.height);
        for (int32_t i = startr; i < endr; i++) {
            double y = map<double>(i + 1, 1.0, config.height, 1.0, -1.0);
            for (int32_t j = startc; j < endc; j++) {
                double x = map<double>(j + 1, 1.0, config.width, -1.0, 1.0);
                std::complex<double> z{0., 0.};
                std::complex<double> c{(x * scale + config.x) * ratio,
                                       (y * scale + config.y)};
                fractals[i * config.width + j] =
                    iterate(z, c, config.iteration);
            }
        }
    };

    std::cout << "Computing fractal....\n";
    auto start = std::chrono::high_resolution_clock::now();

    if (flags.multithread) {
        const auto n_threads = std::thread::hardware_concurrency();
        std::cout << "Thread count: " << n_threads << '\n';
        std::vector<std::thread> threads;
        start = std::chrono::high_resolution_clock::now();

        const auto offset = config.height / int32_t(n_threads);
        for (int32_t i = 0; i < int32_t(n_threads); i++) {
            threads.emplace_back(compute, offset * i, offset * (i + 1), 0,
                                 config.width);
        }
        for (auto& thread : threads)
            thread.join();
    } else {
        compute(0, config.height, 0, config.width);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time took: " << elapsed.count() << " s\n\n";

    if (flags.write) {
        std::cout << "Generating image...\n";
        start = std::chrono::high_resolution_clock::now();
        Image<uint8_t> image(config.width, config.height);

        int32_t index = 0;
        for (int32_t i = 0; i < config.height; i++) {
            for (int32_t j = 0; j < config.width; j++) {
                const auto n = fractals[i * config.width + j];
                if (n < config.iteration) {
                    auto color = std::sqrt(float(n) / float(config.iteration));
                    image.SetPixelRGB(j, i, compute_color(color, .3f),
                                      compute_color(color, .1f),
                                      compute_color(color, .5f));
                } else {
                    image.SetPixelRGB(j, i, 0, 0, 0);
                }
            }
        }

        stbi_write_png(flags.filename.c_str(), image.GetWidth(),
                       image.GetHeight(), image.GetChannels(),
                       image.GetBuffer(),
                       image.GetWidth() * image.GetChannels());

        end     = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Time took: " << elapsed.count() << " s\n\n";
    }

    delete[] fractals;

    return 0;
}
