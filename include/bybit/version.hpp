#ifndef BYBIT_VERSION_HPP
#define BYBIT_VERSION_HPP

#define BYBIT_VERSION_MAJOR 0
#define BYBIT_VERSION_MINOR 1
#define BYBIT_VERSION_PATCH 0

#define BYBIT_VERSION_STRING "0.1.0"

#define BYBIT_VERSION (BYBIT_VERSION_MAJOR * 10000 + BYBIT_VERSION_MINOR * 100 + BYBIT_VERSION_PATCH)

namespace bybit {

inline constexpr int version_major = BYBIT_VERSION_MAJOR;
inline constexpr int version_minor = BYBIT_VERSION_MINOR;
inline constexpr int version_patch = BYBIT_VERSION_PATCH;
inline constexpr const char* version_string = BYBIT_VERSION_STRING;

}  // namespace bybit

#endif  // BYBIT_VERSION_HPP
