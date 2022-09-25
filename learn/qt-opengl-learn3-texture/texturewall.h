#ifndef TEXTUREWALL_H
#define TEXTUREWALL_H

#include "texturebase.h"
namespace l3_textures{

class TextureWall : public TextureBase
{
public:
    TextureWall();

public:
    virtual const char * GetFragmentShaderSource();
    virtual int initVertex();
    virtual void drawingGL();
    virtual const char* GetTexturePath();
};

}
#endif // TEXTUREWALL_H
