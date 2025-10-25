#pragma once

#include "Types.h"

namespace stick
{

struct Error
{
    enum ErrorType : error_t
    {
        None       = 0,
        WindowInit = 1 << 0,
        GlfwInit   = 1 << 1,
        GladInit   = 1 << 2,
    };
    
    error_t Value;

    constexpr Error() : Value(0) {}
    explicit constexpr Error(const ErrorType e) : Value(static_cast<error_t>(e)) {}
    explicit constexpr Error(const error_t v) : Value(v) {}

    constexpr operator error_t() const { return Value; }

    // --- Bitwise operators ---
    constexpr Error operator|(const Error other) const noexcept
    {
        return Error(Value | other.Value);
    }

    constexpr Error operator&(const Error other) const noexcept
    {
        return Error(Value & other.Value);
    }

    constexpr Error operator^(const Error other) const noexcept
    {
        return Error(Value ^ other.Value);
    }

    constexpr Error operator~() const noexcept
    {
        return Error(static_cast<error_t>(~Value));
    }

    constexpr Error& operator|=(const Error other) noexcept
    {
        Value |= other.Value;
        return *this;
    }

    constexpr Error& operator&=(const Error other) noexcept
    {
        Value &= other.Value;
        return *this;
    }

    constexpr Error& operator^=(const Error other) noexcept
    {
        Value ^= other.Value;
        return *this;
    }

    // --- Convenience checks ---
    [[nodiscard]] static constexpr bool Has(const error_t err, const ErrorType e) noexcept
    {
        return (err & static_cast<error_t>(e)) != None;
    }

    [[nodiscard]] static constexpr bool Any(const error_t err) noexcept
    {
        return err != None;
    }

    [[nodiscard]] static constexpr bool NoneSet(const error_t err) noexcept
    {
        return err == None;
    }

    // OR a (possibly empty) pack of errors, returning the combined Error (starts from 0)
    template<typename... Es>
    static constexpr Error From(const Es&... errors) noexcept
    {
        error_t result = 0;
        ((result |= static_cast<error_t>(Error(errors))), ...);
        return Error(result);
    }

    static const char* GetName(const error_t err)
    {
        switch (err)
        {
            case None: return "None";
            case GlfwInit: return "GlfwInit";
            case GladInit: return "GladInit";
            case WindowInit: return "WindowInit";
            default: return "Unknown";
        }
    }
};

} // namespace stick