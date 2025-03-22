#pragma once

typedef int i32;
typedef unsigned int u32;

template<typename T> static T from_underlying(u32 value)
{
    return static_cast<T>(value);
}
