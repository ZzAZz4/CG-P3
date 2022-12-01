

#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include <string_view>

struct Texture2D {
    unsigned int id = 0;

    Texture2D() = default;
    Texture2D(const unsigned char* data, int width, int height, int channels);
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&& o) noexcept;
    Texture2D& operator=(Texture2D&& o) noexcept;
    ~Texture2D();

    bool IsValid() const;
    void Construct(const unsigned char* data, int width, int height, int channels);
    static Texture2D FromFile(std::string_view path);


    void Bind() const;
    void Bind(unsigned int slot) const;
    static void UnbindAny();
    static void UnbindAny(unsigned int slot);

};

#endif // OGL_TEXTURE_H
