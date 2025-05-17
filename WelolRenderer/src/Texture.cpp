#include "Texture.h"
#include <string>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

namespace Welol {
    Texture::Texture(TextureTarget tgt, TextureInternalFormat internalFormat, std::string& imagePath, unsigned int mipLvl, std::string& tName, unsigned int tUnit)
    :   name{tName},
    target(tgt),
    iFormat(internalFormat),
    textureUnit(tUnit),
    mipLevel(mipLvl)
    {        
        glGenTextures(1, &ID);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &ID);
    }

    void Texture::update(Shader& shader)
    {
        shader.use();
        bind();
        shader.setInt(name.c_str(), textureUnit);

    }

    void Texture::attachImageData(std::string& path)
    {

        int w, h, ch;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);

        bind();
        
        
        
        if(data)
        {
            switch(target)
            {
                case WL_TEX_1D:
                break;
                case WL_TEX_2D:
                    glTexImage2D(getGLTextureTarget(target), mipLevel, getGLTextureInternalFormat(iFormat), w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);   
                    //glTexSubImage2D(GL_TEXTURE_2D, mipLevel, 0, 0, w, h, GL_RGB, GL_RGBA, data);
                    break;
                case WL_TEX_3D:
                    break;
                case WL_TEX_1D_ARR:
                    break;
                case WL_TEX_2D_ARR:
                    break;
                case WL_TEX_CUBE_MAP:
                break;
                default:
                break;
                
            }
        }
        else
        {
            std::cout << "Image could not be loaded" << std::endl;
        }
        
        // Mip map generation and default filtering, nothing will be visible if mip maps are not generated
        generateMipMapLevels();
        applyTrilinearMipMapFiltering();
        clampToEdgeTiling();
        
        stbi_image_free(data);
    }

    void Texture::applyTrilinearMipMapFiltering()
    {   
        // We can generate mip map in any order as long as we do that before using the texture
        // they do not have to generated here.
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }



    void Texture::applyNearestMipMapFiltering()
    {
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void Texture::generateMipMapLevels()
    {
        glGenerateMipmap(getGLTextureTarget(target));
    }

    void Texture::addFaceTextureToCubeMap(CubeMapFaces face, std::string& path)
    {
        bind();
        int w, h, ch;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
        
        if(data)
        {
            glTexImage2D(getGLCubeMapFace(face), mipLevel, getGLTextureInternalFormat(iFormat), w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Image could not be loaded" << std::endl;
        }

        // REVISIT: Can we do these after the images are generated?, They take quite a bit
        // of time.
        generateMipMapLevels();
        applyTrilinearMipMapFiltering();
        clampToEdgeTiling();
        stbi_image_free(data);
    }

    void Texture::bind()
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(getGLTextureTarget(target), ID);
    }


    void Texture::clampToEdgeTiling()
    {
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Texture::repeatTiling()
    {
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::mirroredRepeatTiling()
    {   
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    void Texture::applyBilinearFiltering()
    {
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(getGLTextureTarget(target), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}