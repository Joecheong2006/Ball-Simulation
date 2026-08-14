#pragma once

#include <tuple>
#include <utility>

template <typename... AllowedTypes>
class TaggedPointer {
public:
    TaggedPointer() = default;

    template <typename T>
    explicit TaggedPointer(T *raw_ptr) : raw_ptr(raw_ptr) {
        type_id = GetIndex<T, AllowedTypes...>::value;
    }

    struct TotalTypes {
        inline static constexpr int value = sizeof...(AllowedTypes);
    };

protected:
    template <typename Functor>
        auto Dispatch(Functor &&func) const {
            using FirstType = typename std::tuple_element<0, std::tuple<AllowedTypes...>>::type;
            using ReturnType = decltype(func(std::declval<FirstType*>()));
            return dispatch_flat<0, ReturnType>(std::forward<Functor>(func));
        }

private:
    template <typename T, typename... List>
    struct GetIndex;

    template <typename T, typename... Rest>
    struct GetIndex<T, T, Rest...> {
        static constexpr int value = 0;
    };

    template <typename T, typename First, typename... Rest>
    struct GetIndex<T, First, Rest...> {
        static constexpr int value = 1 + GetIndex<T, Rest...>::value;
    };

    template <int Index, typename ReturnType, typename Functor>
    inline ReturnType dispatch_flat(Functor &&func) const {
        if constexpr (Index < sizeof...(AllowedTypes)) {
            if (type_id == Index) {
                using SpecificType = typename std::tuple_element<Index, std::tuple<AllowedTypes...>>::type;
                return func(static_cast<SpecificType*>(raw_ptr));
            }
            return dispatch_flat<Index + 1, ReturnType>(std::forward<Functor>(func));
        }
        if constexpr (!std::is_void_v<ReturnType>) return ReturnType{};
    }

    void *raw_ptr;
    int type_id;

};
