/*
 * Guichan Hakase Extensions
 *
 * Copyright (c) 2013 Hakase Labs
 *
 *
 * Zack Mulgrew a.k.a ZackTheHuman
 *
 * Visit: https://github.com/hakase-labs/guichan-hakase
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/hakase/fixedimagefont.hpp"

#include "guichan/exception.hpp"
#include "guichan/graphics.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{
    FixedImageFont::FixedImageFont(const Image* image, const unsigned int& glyphWidth, const unsigned int& glyphHeight, const std::string& glyphs)
            : mGlyphWidth(glyphWidth),
              mGlyphHeight(glyphHeight),
              mGlyphImage(image)
    {
        fillGlyphMap(glyphs);
    }

    FixedImageFont::FixedImageFont(const Image* image, const unsigned int& glyphSize, const std::string& glyphs)
            : mGlyphWidth(glyphSize),
              mGlyphHeight(glyphSize),
              mGlyphImage(image)
    {
        fillGlyphMap(glyphs);
    }
    
    void FixedImageFont::fillGlyphMap(const std::string& glyphs)
    {
        mGlyphMap.clear();

        int i = 0;

        for (auto itr = glyphs.cbegin(), end = glyphs.cend(); itr != end; itr++, i++)
        {
            auto glyph = (*itr);
            
            mGlyphMap.emplace(
                std::make_pair(
                    glyph, 
                    Rectangle(i * mGlyphWidth, 0, mGlyphWidth, mGlyphHeight)));
        }
    }

    int FixedImageFont::getWidth(const std::string& text) const
    {
        return static_cast<int>(mGlyphWidth * text.size());
    }

    void FixedImageFont::drawString(Graphics* graphics, const std::string& text, int x, int y)
    {
        if (mGlyphImage)
        {
            if (graphics)
            {
                int i = 0;
                int dx = x;
                int dy = y;

                for (auto itr = text.cbegin(), end = text.cend(); itr != end; itr++, i++)
                {
                    const auto & glyph = (*itr);
                    const auto & glyphRectanglePair = mGlyphMap.find(glyph);

                    if (glyphRectanglePair != mGlyphMap.end())
                    {
                        const auto & glyphRectangle = glyphRectanglePair->second;

                        // No need to push a clip rectangle because gcn::Graphics::drawImage does it for us.

                        graphics->drawImage(
                                mGlyphImage, 
                                glyphRectangle.x, 
                                glyphRectangle.y, 
                                dx, 
                                dy, 
                                mGlyphWidth, 
                                mGlyphHeight);
                    }

                    dx += mGlyphWidth;
                }

                
            }
        }
    }

    int FixedImageFont::getHeight() const
    {
        return static_cast<int>(mGlyphHeight);
    }

    int FixedImageFont::getStringIndexAt(const std::string& text, int x) const
    {
        if (x < 0)
        {
            return -1;
        }
        else if (x == 0)
        {
            return 0;
        }

        const int width = getWidth(text);

        if (x >= width)
        {
            return text.size();
        }

        return x / mGlyphWidth;
    }
}