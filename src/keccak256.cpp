
#include <eEVM/keccak256.h>
#include <eEVM/util.h>

extern "C"
{
#include "keccak/KeccakHash.h"
}
namespace eevm
{
    Keccak256::Keccak256(const uint8_t* data, size_t size)
    {
        Keccak_HashInstance hi;
        Keccak_HashInitialize(&hi, 1088, 512, 256, 0x01);
        Keccak_HashUpdate(&hi, data, size * std::numeric_limits<unsigned char>::digits);
        Keccak_HashFinal(&hi, hash.data());
    }
    Keccak256::Keccak256(const std::vector<uint8_t>& vec) : Keccak256(vec.data(), vec.size()) {}
    Keccak256::Keccak256(const std::string& str) : Keccak256((const uint8_t*)str.data(), str.size()) {}

    template <size_t N>
    Keccak256::Keccak256(const std::array<uint8_t, N>& a) : Keccak256(a.data(), N)
    {}

    Keccak256 Keccak256::from_skip(const std::string& str, size_t skip)
    {
        skip = std::min(skip, str.size());
        return Keccak256((const uint8_t*)str.data() + skip, str.size() - skip);
    }
    std::ostream& operator<<(std::ostream& os, const Keccak256& h)
    {
        for (unsigned i = 0; i < Keccak256::SIZE; i++)
        {
            os << std::hex << static_cast<int>(h.hash[i]);
        }

        return os;
    }
    std::string Keccak256::hex_str() const
    {
        return to_hex_string(hash);
    }

    Keccak256 Keccak256::from_hex(const std::string& str)
    {
        Keccak256 digest;
        auto s = strip(str);
        if (s.empty())
            return digest;
        for (size_t i = 0, x = 0; i < 32; i++, x += 2)
        {
            digest.hash.at(i) = strtol(s.substr(x, 2).c_str(), 0, 16);
        }
        return digest;
    }

    Keccak256 Keccak256::from_hex(const std::vector<uint8_t>& data)
    {
        Keccak256 digest;
        std::copy(data.begin(), data.end(), digest.hash.begin());
        return digest;
    }

    bool operator==(const Keccak256& lhs, const Keccak256& rhs)
    {
        for (unsigned i = 0; i < Keccak256::SIZE; i++)
        {
            if (lhs.hash[i] != rhs.hash[i])
            {
                return false;
            }
        }
        return true;
    }

    void to_json(nlohmann::json& j, const Keccak256& hash)
    {
        j = hash.hex_str();
    }

    void from_json(const nlohmann::json& j, Keccak256& hash)
    {
        auto value = j.get<std::string>();
        hash = Keccak256::from_hex(value);
    }
}

FMT_BEGIN_NAMESPACE
template <>
struct formatter<eevm::Keccak256>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const eevm::Keccak256& p, FormatContext& ctx)
    {
        return format_to(ctx.out(), "0x{:02x}", fmt::join(p.hash, ""));
    }
};
FMT_END_NAMESPACE