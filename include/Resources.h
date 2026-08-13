#pragma once
#include <string_view>

namespace Resources {
    constexpr std::string_view root = PROJECT_ROOT;

    struct ImageDescriptor {
        int width, height, channels, bits;
    };

    struct Image;
    void FreeImage(Image&);

    struct Image {
        Image() = default;
        Image(Image &&rhs);
        ~Image();

        Image &operator=(Image &&rhs);
        ImageDescriptor desc{};
        void *data = nullptr;
    };

    void FreeImage(Image &image);
    Image LoadImage(std::string_view path);

};

