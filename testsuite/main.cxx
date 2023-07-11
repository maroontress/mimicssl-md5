#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "md5.h"

#include "expect.hxx"

class Driver final {
private:
    std::map<std::string, std::function<void()>> map;
    bool list = false;
    std::optional<std::string> name;

public:
    Driver(const char* const* args) {
        for (auto* a = args; *a != nullptr; ++a) {
            if (std::strcmp(*a, "--gtest_list_tests") == 0) {
                list = true;
                return;
            }
        }
        for (auto* a = args; *a != nullptr; ++a) {
            auto o = std::string {*a};
            auto prefix = std::string {"--gtest_filter=main."};
            if (o.starts_with(prefix)) {
                name = std::make_optional(o.substr(prefix.length()));
                return;
            }
        }
    }

    void add(const std::string& name, const std::function<void()>& testcase) {
        map[name] = testcase;
    }

    int run() const {
        if (list) {
            std::cout << "main.\n";
            for (auto i = map.cbegin(); i != map.cend(); ++i) {
                auto [name, testcase] = *i;
                std::cout << "  " << name << "\n";
            }
            return 0;
        }
        if (name.has_value()) {
            std::string n = name.value();
            auto testcase = map.at(n);
            try {
                testcase();
                std::cout << n << ": succeeded\n";
            } catch (std::runtime_error& e) {
                std::cout << n << ": failed\n";
                std::cout << "  " << e.what();
                return 1;
            }
            return 0;
        }
        auto count = 0;
        for (auto i = map.cbegin(); i != map.cend(); ++i) {
            auto [name, testcase] = *i;
            try {
                testcase();
                std::cout << name << ": succeeded\n";
            } catch (std::runtime_error& e) {
                std::cout << name << ": failed\n";
                std::cout << "  " << e.what();
                ++count;
            }
        }
        if (count == 0) {
            std::cout << "all tests passed\n";
            return 0;
        }
        std::cout << count << " test(s) failed.\n";
        return 1;
    }
};

namespace maroontress::lighter {
    template <>
    std::string toString(std::uint8_t b) {
        auto v = (uint32_t)b;
        std::ostringstream out;
        out << std::dec << v << " (0x" << std::hex << v << ")";
        return out.str();
    }
}

static auto
toArray(const std::string& m) -> std::array<std::uint8_t, 16>
{
    std::uint8_t array[16];

    if (m.length() != 32) {
        throw std::runtime_error("invalid length");
    }
    for (auto k = 0; k < 16; ++k) {
        auto p = m.substr(k * 2, 2);
        array[k] = (std::uint8_t)std::stoull(p, nullptr, 16);
    }
    return std::to_array(array);
}

static auto
withString(const std::string& m, const std::array<uint8_t, 16>& expected)
    -> void
{
    MD5_CTX ctx;
    uint8_t actual[16];

    MD5_Init(&ctx);
    MD5_Update(&ctx, m.c_str(), m.length());
    MD5_Final(actual, &ctx);
    for (auto k = 0; k < 16; ++k) {
        expect(actual[k]) == expected[k];
    }
}

int
main(int ac, char** av) {
    auto driver = Driver {av};
    driver.add("endian", [] {
        expect(std::endian::native) == std::endian::little;
    });
    driver.add("empty", [] {
        MD5_CTX ctx;
        uint8_t actual[16];

        MD5_Init(&ctx);
        MD5_Final(actual, &ctx);

        // MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
        auto expected = toArray("d41d8cd98f00b204e9800998ecf8427e");
        for (auto k = 0; k < 16; ++k) {
            expect(actual[k]) == expected[k];
        }
    });
    driver.add("a", [] {
        // MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
        auto expected = toArray("0cc175b9c0f1b6a831c399e269772661");
        withString("a", expected);
    });
    driver.add("abc", [] {
        // MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
        auto expected = toArray("900150983cd24fb0d6963f7d28e17f72");
        withString("abc", expected);
    });
    driver.add("message digest", [] {
        // MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
        auto expected = toArray("f96b697d7cb7938d525a2f31aaf161d0");
        withString("message digest", expected);
    });
    driver.add("a-z", [] {
        // MD5 ("abcdefghijklmnopqrstuvwxyz")
        // = c3fcd3d76192e4007dfb496cca67e13b
        auto expected = toArray("c3fcd3d76192e4007dfb496cca67e13b");
        withString("abcdefghijklmnopqrstuvwxyz", expected);
    });
    driver.add("A-Za-z0-9", [] {
        // MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
        // 0123456789") = d174ab98d277d9f5a5611c2c9f419d9f
        auto expected = toArray("d174ab98d277d9f5a5611c2c9f419d9f");
        withString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            "0123456789", expected);
    });
    driver.add("1-90-9...0-90", [] {
        // MD5 ("12345678901234567890123456789012345678901234567890123456789
        // 012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a
        auto expected = toArray("57edf4a22be3c955ac49da2e2107b67a");
        withString("1234567890123456789012345678901234567890123456789"
            "0123456789012345678901234567890", expected);
    });

    driver.add("process one byte at a time", [] {
        MD5_CTX ctx;
        uint8_t actual[16];
        uint8_t expected[16];

        MD5_Init(&ctx);
        std::string m {
            "Alice was beginning to get very tired of sitting by her sister "
            "on the bank, and of having nothing to do: once or twice she had "
            "peeped into the book her sister was reading, but it had no "
            "pictures or conversations in it, and where is the use of a "
            "book, thought Alice, without pictures or conversations? So she "
            "was considering in her own mind, (as well as she could, for the "
            "hot day made her feel very sleepy and stupid,) whether the "
            "pleasure of making a daisy-chain would be worth the trouble of "
            "getting up and picking the daisies, when suddenly a white "
            "rabbit with pink eyes ran close by her."};
        std::size_t length = m.length();
        const char* top = m.c_str();
        for (auto k = 0; k < length; ++k) {
            MD5_Update(&ctx, top + k, 1);
        }
        MD5_Final(actual, &ctx);

        MD5_Init(&ctx);
        MD5_Update(&ctx, m.c_str(), length);
        MD5_Final(expected, &ctx);

        for (auto k = 0; k < 16; ++k) {
            expect(actual[k]) == expected[k];
        }
    });
    return driver.run();
}
