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
#ifndef GCN_HIKARI_FIXEDIMAGEFONT_HPP
#define GCN_HIKARI_FIXEDIMAGEFONT_HPP

#include <string>
#include <map>

#include "guichan/font.hpp"
#include "guichan/platform.hpp"
#include "guichan/rectangle.hpp"

namespace gcn
{
    class Image;

    /**
     * A font using an image containing fixed-width font data. ImageFont can be 
     * used with any image supported by the currently used ImageLoader.
     *
     * @author Zack Mulgrew
     */
    class GCN_EXTENSION_DECLSPEC FixedImageFont : public Font
    {
    public:

        /**
         * Constructor.
         *
         * @param image The image where glyphs can be found.
         * @param glyphWidth The width of the glyphs.
         * @param glyphHeight The height of the glyphs.
         * @param filename The glyph sequence found in the image.
         */
        FixedImageFont(const Image* image, const unsigned int& glyphWidth, const unsigned int& glyphHeight, const std::string& glyphs);

        /**
         * Constructor. Assumes that glyphs are square.
         *
         * @param image The image where glyphs can be found.
         * @param glyphSize The size of the glyphs.
         * @param filename The glyph sequence found in the image.
         */
        FixedImageFont(const Image* image, const unsigned int& glyphSize, const std::string& glyphs);

        /**
         * Destructor.
         */
        virtual ~FixedImageFont() { }

        // Inherited from Font

        virtual int getWidth(const std::string& text) const;

        virtual void drawString(Graphics* graphics, const std::string& text,
                                int x, int y);

        virtual int getHeight() const;

        virtual int getStringIndexAt(const std::string& text, int x) const;
    
    private:
        /**
         * Fills the font's glyph map by constructing rectangles representing 
         * each glyph.
         */
        void fillGlyphMap(const std::string& glyphs);

        /**
         * The width if an individual glyph.
         */
        unsigned int mGlyphWidth;

        /**
         * The height if an individual glyph.
         */
        unsigned int mGlyphHeight;

        /**
         * Holds the image with the font data.
         */
        const Image* mGlyphImage;
        
        /**
         * Holds the glyphs areas in the image.
         */
        std::map<char, Rectangle> mGlyphMap;
    };
}

#endif // end GCN_HIKARI_FIXEDIMAGEFONT_HPP
