#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class Text
{
public:
    Text();
    virtual ~Text();

private:
    static FT_library sFreeTypeGlobal;
    static bool sInited;

    FT_face mFace;
};

