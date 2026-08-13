#pragma once

#include "aggreate.h"

#include <vector>
#include <numeric>
#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    namespace detail {
        template<typename T> struct get_type        { static constexpr int type = 0; };
        template <>          struct get_type<int>   { static constexpr int type = GL_INT; };
        template <>          struct get_type<float> { static constexpr int type = GL_FLOAT; };
    }

    struct BufferLayout {
        unsigned int Vao;
        int currentLocation = 0;

        BufferLayout() {
            glGenVertexArrays(1, &Vao);
        }

        void bind() const {
            glBindVertexArray(Vao);
        }

        void unbind() const {
            glBindVertexArray(0);
        }

        void setDivisor(int divisor) const {
            glVertexAttribDivisor(currentLocation, divisor);
        }

        template <typename BaseType>
        struct Struct {
            std::vector<int> itemsCount;
            int type = detail::get_type<BaseType>::type;
            int stride = 0;

            Struct(const std::initializer_list<int> &itemsCount)
                : itemsCount(itemsCount)
            {
                stride = std::reduce(itemsCount.begin(), itemsCount.end(), 0);
            }

            Struct(const std::vector<int> &itemsCount)
                : itemsCount(itemsCount)
            {
                stride = std::reduce(itemsCount.begin(), itemsCount.end(), 0);
            }
        };

        template <typename T, typename BaseType>
        struct Aggregate : public Struct<BaseType> {
            Aggregate()
                : Struct<BaseType>([]() {
                                       auto itemsByte = aggregate::get_field_sizes<T>();
                                       for (auto &e : itemsByte) {
                                           e /= sizeof(BaseType);
                                       }
                                       return itemsByte;
                                   }())
            {}
        };

        template <typename BaseType>
        void set(Struct<BaseType> structLayout, int divisor = 0) {
            size_t offset = 0;
            for (const auto &itemCount : structLayout.itemsCount) {
                glEnableVertexAttribArray(currentLocation);
                glVertexAttribPointer(currentLocation, itemCount, structLayout.type, GL_FALSE,
                        structLayout.stride * sizeof(BaseType), (void*)offset);
                setDivisor(divisor);
                ++currentLocation;

                offset += itemCount * sizeof(BaseType);
            }
        }

        template <typename BaseType>
        void set(int offset, int size, int stride, int divisor = 0) {
            glEnableVertexAttribArray(currentLocation);
            glVertexAttribPointer(currentLocation, size, detail::get_type<BaseType>::type, GL_FALSE,
                    stride * sizeof(BaseType), (void*)(offset * sizeof(BaseType)));
            setDivisor(divisor);
            ++currentLocation;
        }

    };

}
