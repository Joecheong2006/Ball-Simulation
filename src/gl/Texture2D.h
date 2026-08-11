#pragma once

#include <cstddef>
#include <glad/gl.h> 

namespace gl {
    struct Texture2D {
        unsigned int id;
        int slot = 0;

        Texture2D(const Texture2D&) = delete;
        Texture2D &operator=(const Texture2D&) = delete;

        Texture2D() {
            glGenTextures(1, &id);
        }

        void setWrapping(int wrap_s, int wrap_t, int min_filter, int mag_filter) const {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        }

        void genTexture(Resources::Image &img) const {
            Resources::ImageDescriptor &desc = img.desc;

            int internalFormat = GetInternalFormat(desc.channels, desc.bits);
            int format = GetFormat(desc.channels);
            int type = GetType(desc.bits);

            genTexture(internalFormat, desc.width, desc.height, format, type, img.data);
        }

        void genTexture(int internalFormat, int width, int height, int format, int type, void *data) const {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        void setSlot(int unit) {
            glActiveTexture(GL_TEXTURE0 + unit);
        }

        void bind() const { glBindTexture(GL_TEXTURE_2D, id); }

        inline static int GetInternalFormat(int channels, int bits) {
            if (channels == 4) {
                if (bits == 32) return GL_RGBA32F;
                if (bits == 16) return GL_RGBA16;
                return GL_RGBA8;
            }
            if (channels == 3) {
                if (bits == 32) return GL_RGB32F;
                return GL_RGB8; // 16-bit RGB is rarely supported natively on modern GPUs
            }
            if (bits == 32) return GL_R32F;
            if (bits == 16) return GL_R16;
            return GL_R8;
        }

        inline static int GetFormat(int channels) {
            if (channels == 4) return GL_RGBA;
            if (channels == 3) return GL_RGB;
            return GL_RED;
        }

        inline static int GetType(int bits) {
            if (bits == 32) return GL_FLOAT;
            if (bits == 16) return GL_UNSIGNED_SHORT;
            return GL_UNSIGNED_BYTE;
        }

    };

};
