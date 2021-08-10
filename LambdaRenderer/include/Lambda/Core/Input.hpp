/**
 * @file   Input.hpp
 * @author Pratchaya Khansomboon (pratchaya.k.git@gmail.com)
 * @brief  Input codes for keyboard, mouse and other input devices.
 * @date 2021-08-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include "Core.hpp"

#include <cstdint>

// clang-format off

namespace Lambda {

    enum class LAMBDA_API KeyState : int32_t {
        UNKNOWN = -1,
        RELEASE =  0,
        PRESS   =  1,
        REPEAT  =  2,
    };

    // Mirrors the GLFW keys
    enum class LAMBDA_API KeyCode : int32_t {
        UNKNOWN         =  -1,  // Unkown Key

        // Printable keys
        SPACE           =  32,  // Space
        APOSTROPHE      =  39,  // '
        COMMAN          =  44,  // ,
        MINUS           =  45,  // -
        PERIOD          =  46,  // .
        SLASH           =  47,  // /
        N0              =  48,  // 0
        N1              =  49,  // 1
        N2              =  50,  // 2
        N3              =  51,  // 3
        N4              =  52,  // 4
        N5              =  53,  // 5
        N6              =  54,  // 6
        N7              =  55,  // 7
        N8              =  56,  // 8
        N9              =  57,  // 9
        SEMICOLON       =  59,  // ;
        EQUAL           =  61,  // =
        A               =  65,  // A
        B               =  66,  // B
        C               =  67,  // C
        D               =  68,  // D
        E               =  69,  // E
        F               =  70,  // F
        G               =  71,  // G
        H               =  72,  // H
        I               =  73,  // I
        J               =  74,  // J
        K               =  75,  // K
        L               =  76,  // L
        M               =  77,  // M
        N               =  78,  // N
        O               =  79,  // O
        P               =  80,  // P
        Q               =  81,  // Q
        R               =  82,  // R
        S               =  83,  // S
        T               =  84,  // T
        U               =  85,  // U
        V               =  86,  // V
        W               =  87,  // W
        X               =  88,  // X
        Y               =  89,  // Y
        Z               =  90,  // Z
        LEFT_BRACKET    =  91,  // [
        BACKSLASH       =  92,  // '\'
        RIGHT_BRACKET   =  93,  // ]
        GRAVE_ACCENT    =  96,  // `
        WORLD_1         = 161,  // non-US #1
        WORLD_2         = 162,  // non-US #2

        // Function keys
        ESCAPE          = 256,
        ENTER           = 257,
        TAB             = 258,
        BACKSPACE       = 259,
        INSERT          = 260,
        DELETE          = 261,
        RIGHT           = 262,  // ARROW KEY: Right
        LEFT            = 263,  // ARROW KEY: Left
        DOWN            = 264,  // ARROW KEY: Down
        UP              = 265,  // ARROW KEY: Up
        PAGE_UP         = 266,
        PAGE_DOWN       = 267,
        HOME            = 268,
        END             = 269,
        CAPS_LOCK       = 280,
        SCROLL_LOCK     = 281,
        NUM_LOCK        = 282,
        PRINT_SCREEN    = 283,
        PAUSE           = 284,
        F1              = 290,
        F2              = 291,
        F3              = 292,
        F4              = 293,
        F5              = 294,
        F6              = 295,
        F7              = 296,
        F8              = 297,
        F9              = 298,
        F10             = 299,
        F11             = 300,
        F12             = 301,
        F13             = 302,
        F14             = 303,
        F15             = 304,
        F16             = 305,
        F17             = 306,
        F18             = 307,
        F19             = 308,
        F20             = 309,
        F21             = 310,
        F22             = 311,
        F23             = 312,
        F24             = 313,
        F25             = 314,
        KP_0            = 320,
        KP_1            = 321,
        KP_2            = 322,
        KP_3            = 323,
        KP_4            = 324,
        KP_5            = 325,
        KP_6            = 326,
        KP_7            = 327,
        KP_8            = 328,
        KP_9            = 329,
        KP_DECIMAL      = 330,
        KP_DIVIDE       = 331,
        KP_MULTIPLY     = 332,
        KP_SUBTRACT     = 333,
        KP_ADD          = 334,
        KP_ENTER        = 335,
        KP_EQUAL        = 336,
        LEFT_SHIFT      = 340,
        LEFT_CONTROL    = 341,
        LEFT_ALT        = 342,
        LEFT_SUPER      = 343,
        RIGHT_SHIFT     = 344,
        RIGHT_CONTROL   = 345,
        RIGHT_ALT       = 346,
        RIGHT_SUPER     = 347,
        MENU            = 348,

        LAST            = MENU,
    };

    // Mirrors the GLFW MouseButtons
    enum class LAMBDA_API MouseButton : int32_t {
        UNKNOWN         =  -1,
        B1              =   0,
        B2              =   1,
        B3              =   2,
        B4              =   3,
        B5              =   4,
        B6              =   5,
        B7              =   6,
        B8              =   7,
        LAST            =  B8,
        LEFT            =  B1,
        RIGHT           =  B2,
        MIDDLE          =  B3,
    };

}  // namespace Lambda
