#pragma once

#include <glad/glad.h>
#include <iostream>
#include "shader.h"




namespace Welol {
    
    enum TextureTarget : unsigned int
    {
        WL_TEX_1D = 0,
        WL_TEX_2D = 1,
        WL_TEX_3D = 2,
        WL_TEX_1D_ARR = 3,
        WL_TEX_2D_ARR = 4,
        WL_TEX_CUBE_MAP = 5,
    };

    enum TextureInternalFormat : unsigned int
    {
        WL_RGB = 0,
        WL_RGBA = 1
    };

    enum CubeMapFaces : unsigned int
    {
        WL_CBM_PX = 0,
        WL_CBM_PY = 1,
        WL_CBM_PZ = 2,
        WL_CBM_NX = 3,
        WL_CBM_NY = 4,
        WL_CBM_NZ = 5
    };

    static GLenum getGLTextureTarget(TextureTarget target)
    {
        switch(target)
        {
            case WL_TEX_1D:
                return GL_TEXTURE_1D;
            case WL_TEX_2D:
                return GL_TEXTURE_2D;
            case WL_TEX_3D:
                return GL_TEXTURE_3D;
            case WL_TEX_1D_ARR:
                return GL_TEXTURE_1D_ARRAY;
            case WL_TEX_2D_ARR:
                return GL_TEXTURE_2D_ARRAY;
            case WL_TEX_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
            default:
                return 0;

        }
    }

    static GLint getGLTextureInternalFormat(TextureInternalFormat format)
    {
        switch(format)
        {
            case WL_RGB:
                return GL_RGB;
            case WL_RGBA:
                return GL_RGBA;
            default:
                return 0;

        }
    }

    static GLenum getGLCubeMapFace(CubeMapFaces face)
    {
        switch(face)
        {
            case WL_CBM_PX:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case WL_CBM_PY:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case WL_CBM_PZ:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            case WL_CBM_NX:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case WL_CBM_NY:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case WL_CBM_NZ:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                break;
            default:
                std::cout << "The cube map face does not exist" << std::endl;
                return 0;
                
        }
    }

    class Texture
    {
    public:
        Texture() = default;
        Texture(TextureTarget tgt, std::string& imagePath, unsigned int mipLvl, std::string& tName, unsigned int tUnit);
        ~Texture();
        void update(Shader& shader);
        void attachImageData(std::string& path);
        void addFaceTextureToCubeMap(CubeMapFaces face, std::string& path);
        unsigned int getTextureID() {return ID;}
    public:
        unsigned int ID;

        // Each texture has an associated texture unit that does the actual GPU work like generating
        // mip maps, apply the filters. This can be done on software but it is order of magnitude faster
        // on hardware.
        std::string name;
        TextureTarget target;
        unsigned int textureUnit;
        unsigned int mipLevel;


        // MipMapping
        /*
            This interpolates between two mip map levels which is the correct way of doing
            mip mapping as opposed to choosing between two levels(as done by nearest mip map filtering). 
        */
        void applyTrilinearMipMapFiltering();
        /*
            This chooses between two mip map levels by taking the nearest mip map level.
        */
        void applyNearestMipMapFiltering();

        /*
            Generating mip map levels allows us to use a lower resolution image image when objects
            are sufficiently far to distinguish. This is more efficient.
        */

        /*
            Bilinear filtering without mip mapping.
        */
        void applyBilinearFiltering();


        void generateMipMapLevels();
        void bind();

        // Tiling filters for when the Texture coordinates are outside the normalized range
        void clampToEdgeTiling();
        void repeatTiling();
        void mirroredRepeatTiling();
    };


    class EnvironmentMap : public Texture
    {
        EnvironmentMap(TextureTarget tgt, TextureInternalFormat internalFormat, std::string& imagePath, unsigned int mipLvl, std::string& tName, unsigned int tUnit);
    };
}