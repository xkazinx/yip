// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// kazilib (src\dep\kazilib, headers only): the std names written without std::,
// v += x, magic_enum and Under(x)
#include <stdnames.h>
#include <vector_ops.h>
#include <under.h>
#include <magic_enum.h>
using namespace lib;
using magic_enum::enum_count;
using magic_enum::enum_name;
using magic_enum::enum_values;

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::RectangleShape;
using sf::ContextSettings;
using sf::VideoMode;
using sf::Vector2f;
using Key = sf::Keyboard::Key;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <cmath>
#include <format>
#include <chrono>
namespace this_thread = std::this_thread;
namespace chrono = std::chrono;

using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8  = std::int8_t;

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

using szt = std::size_t;

template<class T, class... Args>
	requires (!std::is_array_v<T>)
inline unique_ptr<T> mk(Args &&... args)
{
	return make_unique<T>(forward<Args>(args)...);
}

constexpr u16 kResX = 800, kResY = 600;

#include "global.h"
using namespace global;
