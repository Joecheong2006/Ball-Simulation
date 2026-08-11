#pragma once
#include <utility>
#include <type_traits>
#include <vector>

namespace aggregate {
    struct any_type {
        template <typename T> operator T(); 
    };

    template <typename T, typename... Args>
    auto is_constructible_from_n(int) -> decltype(T{std::declval<Args>()...}, std::true_type{});

    template <typename T, typename... Args>
    auto is_constructible_from_n(long) -> std::false_type;

    template <typename T, std::size_t N, typename = std::make_index_sequence<N>>
    struct check_count;

    template <typename T, std::size_t N, std::size_t... Is>
    struct check_count<T, N, std::index_sequence<Is...>> {
        using type = decltype(is_constructible_from_n<T, decltype((void)Is, any_type{})...>(0));
    };

    template <typename T, std::size_t N = 0>
    constexpr std::size_t count_fields() {
        if constexpr (typename check_count<T, N + 1>::type{}) {
            return count_fields<T, N + 1>(); // Try the next size up
        } else {
            return N;
        }
    }

    template <typename T>
    std::vector<int> get_field_sizes() {
        static_assert(std::is_aggregate_v<T>, "Type T must be an aggregate.");

        constexpr auto N = count_fields<T>();

        alignas(T) char buffer[sizeof(T)] = {};
        const T& mock = *reinterpret_cast<const T*>(buffer);

        if constexpr (N == 1) {
            auto& [m1] = mock;
            return std::vector<int> {
                sizeof(m1)
            };
        }
        else if constexpr (N == 2) {
            auto& [m1, m2] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2)
            };
        }
        else if constexpr (N == 3) {
            auto& [m1, m2, m3] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3)
            };
        }
        else if constexpr (N == 4) {
            auto& [m1, m2, m3, m4] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3), sizeof(m4)
            };
        }
        else if constexpr (N == 5) {
            auto& [m1, m2, m3, m4, m5] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3), sizeof(m4),
                sizeof(m5)
            };
        }
        else if constexpr (N == 6) {
            auto& [m1, m2, m3, m4, m5, m6] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3), sizeof(m4),
                sizeof(m5), sizeof(m6)
            };
        }
        else if constexpr (N == 7) {
            auto& [m1, m2, m3, m4, m5, m6, m7] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3), sizeof(m4),
                sizeof(m5), sizeof(m6), sizeof(m7)
            };
        }
        else if constexpr (N == 8) {
            auto& [m1, m2, m3, m4, m5, m6, m7, m8] = mock;
            return std::vector<int> {
                sizeof(m1), sizeof(m2), sizeof(m3), sizeof(m4),
                sizeof(m5), sizeof(m6), sizeof(m7), sizeof(m8)
            };
        }

        static_assert(true, "get_sizes only support N up to 8.");
    }
};

