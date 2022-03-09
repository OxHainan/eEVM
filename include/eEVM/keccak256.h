
#pragma once
#include "array"
#include "string"
#include "vector"

#include <nlohmann/json.hpp>
namespace eevm
{
    class Keccak256
    {
      public:
        static constexpr size_t SIZE = 256 / 8;
        using KeccakHash = std::array<uint8_t, SIZE>;

        Keccak256() = default;
        Keccak256(const uint8_t* data, size_t size);
        Keccak256(const std::vector<uint8_t>& vec);
        Keccak256(const std::string& str);
        template <size_t N>
        Keccak256(const std::array<uint8_t, N>& a);

        static Keccak256 from_skip(const std::string& str, size_t skip);

        friend std::ostream& operator<<(std::ostream& os, const Keccak256& h);
        std::string hex_str() const;
        static Keccak256 from_hex(const std::string& str);
        static Keccak256 from_hex(const std::vector<uint8_t>& data);

        const uint8_t* data() const
        {
            return hash.data();
        }
        size_t size() const
        {
            return hash.size();
        }

        KeccakHash hash = {0};
        friend void to_json(nlohmann::json& j, const Keccak256& hash);
        friend void from_json(const nlohmann::json& j, Keccak256& hash);
    };

    void to_json(nlohmann::json& j, const Keccak256& hash);

    void from_json(const nlohmann::json& j, Keccak256& hash);

    bool operator==(const Keccak256& lhs, const Keccak256& rhs);
} // namespace eevm
