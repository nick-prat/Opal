#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <Core/gl.hh>

#include <Render/sampler.hh>

#include "resources.hh"

namespace Opal {

    class Texture : protected Resources::RTexture {
    public:
        Texture(const Texture&) = delete;
        Texture(Texture &&texture);
        Texture(Resources::RTexture &&texture);
        ~Texture();

        Texture &operator=(const Texture&) = delete;
        Texture &operator=(Texture&&) = delete;

        std::string getFileName() const;
        void bind() const;

        Sampler &getSampler();
        const Sampler &getSampler() const;

    private:
        Sampler m_sampler;
        GLuint m_glTexture;
        std::string m_filename;
    };

}

#endif // _TEXTURE_H
