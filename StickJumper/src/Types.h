#pragma once

#include <cstdint>
#include <memory>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using error_t = u8;

// Scope as well as above in case I decide to make my own version of unique ptr
template<typename T>
using scope = std::unique_ptr<T>;

// Ref as well as above in case I decide to make my own version of shared ptr
template<typename T>
using ref = std::shared_ptr<T>;


template<typename T, typename... Args>
constexpr scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

inline auto operator""_KB(const size_t x)
{
    return x * 1024u;
}
inline auto operator""_MB(const size_t x)
{
    // x * 1024 * 1024
    return x * 1048576u;
}
inline auto operator""_GB(const size_t x)
{
    // x * 1024 * 1024 * 1024
    return x * 1073741824u;
}

inline u32 operator""_KBu(const size_t x)
{
    return (u32) x * 1024u;
}
inline u32 operator""_MBu(const size_t x)
{
    // x * 1024 * 1024
    return (u32) x * 1048576u;
}
inline u32 operator""_GBu(const size_t x)
{
    // x * 1024 * 1024 * 1024
    return (u32) x * 1073741824u;
}