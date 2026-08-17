#include "Resources.h"
#include "profiling.h"

#include "stb_image/stb_image.h"
#include <string>

namespace Resources {
    Image::~Image() {
        ZoneScoped;
        if (data) {
            FreeImage(*this);
        }
    }

    Image::Image(Image &&rhs)
        : desc(rhs.desc), data(rhs.data)
    {
        ZoneScoped;
        rhs.data = nullptr;
        rhs.desc = ImageDescriptor{};
    }

    Image &Image::operator=(Image &&rhs) {
        ZoneScoped;
        if (this == &rhs) {
            return *this;
        }

        if (this->data) {
            FreeImage(*this);
        }

        this->data = rhs.data;
        this->desc = rhs.desc;

        rhs.data = nullptr;
        rhs.desc = ImageDescriptor{};

        return *this;
    }

    void FreeImage(Image &image) {
        ZoneScoped;
        stbi_image_free(image.data);
    }

    Image LoadImage(std::string_view path) {
        ZoneScoped;
        std::string real_path;
        real_path.reserve(Resources::root.size() + path.size());
        real_path.append(Resources::root);
        real_path.append(path);

        Image img;
        const char* pathStr = real_path.data();

        if (stbi_is_hdr(pathStr)) {
            img.desc.bits = 32;
            img.data = stbi_loadf(pathStr, &img.desc.width, &img.desc.height, &img.desc.channels, 0);
        }
        else if (stbi_is_16_bit(pathStr)) {
            img.desc.bits = 16;
            img.data = stbi_load_16(pathStr, &img.desc.width, &img.desc.height, &img.desc.channels, 0);
        }

        img.desc.bits = 8;
        img.data = stbi_load(pathStr, &img.desc.width, &img.desc.height, &img.desc.channels, 0);
        return img;
    };

};
