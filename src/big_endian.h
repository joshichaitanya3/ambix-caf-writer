#ifndef AMBIX_CAF_WRITER_BIG_ENDIAN_H_
#define AMBIX_CAF_WRITER_BIG_ENDIAN_H_

#include <cstring> // For memcpy
#include <fstream>

namespace big_endian
{
    // Covert arithmetic value to big endian order
    template<typename T>
    constexpr T ToBigEndian(T val) {
        if constexpr (std::endian::native == std::endian::big) {
            return val;
        }

        if constexpr (std::is_floating_point_v<T>) {
            // Handle floating point types by reinterpreting as integer
            if constexpr (sizeof(T) == 4) {
                uint32_t tmp;
                std::memcpy(&tmp, &val, sizeof(tmp));
                tmp = ToBigEndian(tmp);  // Recursive call to integer version
                std::memcpy(&val, &tmp, sizeof(val));
                return val;
            } else if constexpr (sizeof(T) == 8) {
                uint64_t tmp;
                std::memcpy(&tmp, &val, sizeof(tmp));
                tmp = ToBigEndian(tmp);
                std::memcpy(&val, &tmp, sizeof(val));
                return val;
            }
        } else if constexpr (std::is_integral_v<T>) {
            // Handle integer types with bit operations
            #if __cplusplus >= 202302L
            // Use C++23 features
            return std::byteswap(val);
            #else
            if constexpr (sizeof(T) == 1) {
                return val;  // Single byte, no swap needed
            } else if constexpr (sizeof(T) == 2) {
                return static_cast<T>((val >> 8) | (val << 8));
            } else if constexpr (sizeof(T) == 4) {
                return static_cast<T>(
                    ((val & 0x000000FFu) << 24) |
                    ((val & 0x0000FF00u) <<  8) |
                    ((val & 0x00FF0000u) >>  8) |
                    ((val & 0xFF000000u) >> 24)
                );
            } else if constexpr (sizeof(T) == 8) {
                return static_cast<T>(
                    ((val & 0x00000000000000FFull) << 56) |
                    ((val & 0x000000000000FF00ull) << 40) |
                    ((val & 0x0000000000FF0000ull) << 24) |
                    ((val & 0x00000000FF000000ull) <<  8) |
                    ((val & 0x000000FF00000000ull) >>  8) |
                    ((val & 0x0000FF0000000000ull) >> 24) |
                    ((val & 0x00FF000000000000ull) >> 40) |
                    ((val & 0xFF00000000000000ull) >> 56)
                );
            }
            #endif
        }

        static_assert(std::is_arithmetic_v<T>, "ToBigEndian only supports arithmetic types");
        return val;
    }

    // Write value in big endian order to file
    template<typename T>
    void Write(std::ofstream& out, T val) {
        T be = ToBigEndian(val);
        out.write(reinterpret_cast<const char*>(&be), sizeof(T));
    }

    // Create a uint32_t four character code 'abcd'
    constexpr uint32_t MakeFourCC(char a, char b, char c, char d) {
        return (static_cast<uint32_t>(a) << 24) | 
            (static_cast<uint32_t>(b) << 16) | 
            (static_cast<uint32_t>(c) << 8) | 
            static_cast<uint32_t>(d);
    }

} // namespace big_endian

#endif // AMBIX_CAF_WRITER_BIG_ENDIAN_H_
