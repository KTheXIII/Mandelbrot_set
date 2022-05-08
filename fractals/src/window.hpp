/**
 * @file   window.hpp
 * @author Pratchaya Khansomboon (pratchaya.k.git@gmail.com)
 * @brief  Window abstraction layer for GLFW.
 * @date   2022-05-03
 *
 * @copyright Copyright (c) 2022
 */
#ifndef FRACTALS_WINDOW_HPP
#define FRACTALS_WINDOW_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

#include "mono.hpp"
#include "input.hpp"
#include "event.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace mono {
struct window_props {
    std::string  title  = "no title";
    std::int32_t width  = 738;
    std::int32_t height = 480;
    std::int32_t xpos{INT32_MIN};
    std::int32_t ypos{INT32_MIN};
};

class window {
  public:
    using event_fn = std::function<void(event const&)>;

  public:
    explicit window(window_props const& props = {});
    ~window();

    [[nodiscard]] auto shouldclose() const -> bool { return glfwWindowShouldClose(m_window); }
    [[nodiscard]] auto width()  const -> std::int32_t { return m_data.width;  }
    [[nodiscard]] auto height() const -> std::int32_t { return m_data.height; }
    [[nodiscard]] auto buffer_width()  const -> std::int32_t { return m_data.buffer_width; }
    [[nodiscard]] auto buffer_height() const -> std::int32_t { return m_data.buffer_height; }
    [[nodiscard]] auto xpos() const -> std::int32_t { return m_data.xpos; }
    [[nodiscard]] auto ypos() const -> std::int32_t { return m_data.ypos; }
    auto set_position(std::int32_t const& x, std::int32_t const& y) -> void {
        glfwSetWindowPos(m_window, x, y);
    }
    auto swap() -> void;
    auto poll() -> void;
    auto time() const -> double { return glfwGetTime(); }

    template <typename Callable>
    auto add_event_listener(event_type const& type, Callable const& func) -> std::size_t {
        auto id = (std::size_t)&func;
        if (m_data.events.find(type) == m_data.events.end()) {
            std::unordered_map<std::size_t, event_fn> fns{};
            fns.insert({id, event_fn{func}});
            m_data.events.insert({type, fns});
        } else {
            m_data.events[type].insert({id, event_fn{func}});
        }
        return id;
    }
    template <typename Callable>
    auto remove_event_listener(event_type const& type, [[maybe_unused]]Callable const& func) -> void {
        auto id  = (std::size_t)&func;
        auto fns = m_data.events.find(type);
        if (fns != std::end(m_data.events))
            fns->second.erase(id);
    }

    auto get_key(std::int32_t key) -> std::int32_t {
        return glfwGetKey(m_window, key);
    }
    auto make_key(std::int32_t const& key) -> ref<state::key> {
        m_keys.push_back(state::key::make(key));
        return m_keys.back();
    }

  public:
    [[nodiscard]] auto str() const -> std::string {
        std::string str{"mono::window { "};
        str += "title: \"" + m_data.title + "\", ";
        str += "width: "  + std::to_string(m_data.width)  + ", ";
        str += "height: " + std::to_string(m_data.height) + ", ";
        str += "buffer_width: "  + std::to_string(m_data.buffer_width) + ", ";
        str += "buffer_height: " + std::to_string(m_data.buffer_height) + " }";
        return str;
    }

  private:
    GLFWwindow* m_window{nullptr};
    std::vector<ref<state::key>> m_keys{};

    struct data {
        std::string title;
        std::int32_t width;
        std::int32_t height;
        std::int32_t buffer_width;
        std::int32_t buffer_height;
        std::int32_t xpos;
        std::int32_t ypos;

        std::unordered_map<event_type, std::unordered_map<std::size_t, event_fn>> events;
    };
    data m_data{};

  private:
    static auto user_ptr(GLFWwindow* window) -> window::data* {
        return static_cast<window::data*>(glfwGetWindowUserPointer(window));
    }
};
}

#endif //FRACTALS_WINDOW_HPP
