#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
public:
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture(Texture&&);
    Texture(const std::string& texturePath);
    ~Texture();

    Texture operator=(const Texture&) = delete;

    inline const unsigned char* const GetTextureData() const { return m_data; }
    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }
private:
    unsigned char *m_data;
    int m_width;
    int m_height;
};

#endif // !MESH_HPP
