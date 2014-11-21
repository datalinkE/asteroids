#include "Text.h"
#include "platform_asset.h"
#include <string>

Text::sInited(false);
Text::sFreeTypeGlobal;

Text::Text()
{
    if (!sInited)
    {
        int error = FT_Init_FreeType(&sFreeTypeGlobal);
        if (error)
        {
            DLOG() << error;
            CRASH("Freetype initialisation failed!");
        }
    }
    std::string fontData = get_asset_data("Reckoner.ttf");

    error = FT_New_Memory_Face( sFreeTypeGlobal,
                                fontData.data(),    /* first byte in memory */
                                fontData.size(),      /* size in bytes        */
                                0,         /* face_index           */
                                &mFace );
    if (error)
    {
        DLOG() << error;
        CRASH("Font loading failed!");
    }

    FT_Set_Pixel_Sizes(mFace, 0, 48);
    FT_GlyphSlot g = mFace->glyph;
}

Text::~Text() {
    // TODO Auto-generated destructor stub
}

