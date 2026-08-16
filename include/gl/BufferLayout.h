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

        inline int get_type_size(int type) {
            switch (type) {
                case GL_INT: return 4;
                case GL_FLOAT: return 4;
                default: return 0;
            }
        }
    }

    struct BufferLayout {
        unsigned int Vao;
        int currentLocation = 0;

        BufferLayout() = default;

        void initialize() {
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

        struct Attribute {
            int type, offset, size, stride, divisor;
        };

        using Attributes = std::vector<Attribute>;

        template <typename BaseType>
        struct Struct {
            std::vector<int> itemsCount;
            int type = detail::get_type<BaseType>::type,
                type_size = sizeof(BaseType),
                divisor, stride;

            Struct(const std::initializer_list<int> &itemsCount, int divisor = 0)
                : itemsCount(itemsCount), divisor(divisor)
            {
                stride = std::reduce(itemsCount.begin(), itemsCount.end(), 0);
            }

            Struct(const std::vector<int> &itemsCount, int divisor = 0)
                : itemsCount(itemsCount), divisor(divisor)
            {
                stride = std::reduce(itemsCount.begin(), itemsCount.end(), 0);
            }

            Attributes getAttributes() const {
                Attributes attributes;
                int offset = 0;
                for (auto &count : itemsCount) {
                    attributes.push_back({ type, offset, count, stride, divisor });
                    offset += count;
                }
                return attributes;
            }
        };

        template <typename T, typename BaseType>
        struct Aggregate : public Struct<BaseType> {
            Aggregate(int divisor = 0)
                : Struct<BaseType>([]() {
                                       auto itemsByte = aggregate::get_field_sizes<T>();
                                       for (auto &e : itemsByte) {
                                           e /= sizeof(BaseType);
                                       }
                                       return itemsByte;
                                   }(), divisor)
            {}
        };

        template <typename BaseType>
        void set(const Struct<BaseType> &structLayout) {
            Attributes attributes = structLayout.getAttributes();
            for (auto &attri : attributes) {
                set(attri);
            }
        }

        void set(const Attributes &attributes) {
            for (auto &attri : attributes) {
                set(attri);
            }
        }

        void set(const Attribute &attri) {
            const auto &[type, offset, size, stride, divisor] = attri;
            const int type_size = detail::get_type_size(type);

            glEnableVertexAttribArray(currentLocation);
            glVertexAttribPointer(currentLocation, size, type, GL_FALSE,
                    stride * type_size, (void*)(size_t)(offset * type_size));
            setDivisor(divisor);
            ++currentLocation;
        }
    };

}
