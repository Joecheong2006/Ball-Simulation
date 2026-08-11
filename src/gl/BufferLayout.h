#pragma once

#include "../aggreate.h"

#include <vector>
#include <numeric>
#include <cstddef>
#include <glad/gl.h> 

namespace gl {
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
            int type = GetGLType<BaseType>::type;
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
            glVertexAttribPointer(currentLocation, size, GetGLType<BaseType>::type, GL_FALSE,
                    stride * sizeof(BaseType), (void*)(offset * sizeof(BaseType)));
            setDivisor(divisor);
            ++currentLocation;
        }

        template<typename T>
        struct GetGLType {};

        template <> struct GetGLType<int> { static constexpr int type = GL_INT; };
        template <> struct GetGLType<unsigned int> { static constexpr int type = GL_UNSIGNED_INT; };
        template <> struct GetGLType<float> { static constexpr int type = GL_FLOAT; };

    };

}
