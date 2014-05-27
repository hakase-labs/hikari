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

#include "guichan/hakase/labelex.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"

 #include <iostream>
 #include <string>
 #include <sstream>

namespace gcn
{
    LabelEx::LabelEx()
    {
        mAlignment = Graphics::Left;
    }

    LabelEx::LabelEx(const std::string& caption)
    {
        setCaption(caption);
        setAlignment(Graphics::Left);
        adjustSize();
    }

    const std::string &LabelEx::getCaption() const
    {
        return mCaption;
    }

    void LabelEx::setCaption(const std::string& caption)
    {
        mCaption = caption;

        mCaptionSections.clear();

        if(caption.find('\n') != std::string::npos)
        {
            std::stringstream ss(mCaption);
            std::string item;
            while(std::getline(ss, item, '\n'))
            {
                mCaptionSections.push_back(item);
            }
        }
        else
        {
            mCaptionSections.push_back(caption);
        }
    }

    void LabelEx::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment LabelEx::getAlignment() const
    {
        return mAlignment;
    }

    void LabelEx::draw(Graphics* graphics)
    {
        for(int i = 0; i < mCaptionSections.size(); ++i)
        {
            int textX;
            int textY = getFont()->getHeight() * i;

            switch (getAlignment())
            {
              case Graphics::Left:
                  textX = 0;
                  break;
              case Graphics::Center:
                  textX = getWidth() / 2;
                  break;
              case Graphics::Right:
                  textX = getWidth();
                  break;
              default:
                  throw GCN_EXCEPTION("Unknown alignment.");
            }

            graphics->setFont(getFont());
            graphics->setColor(getForegroundColor());
            graphics->drawText(mCaptionSections.at(i), textX, textY, getAlignment());
        }
    }

    void LabelEx::adjustSize()
    {
        int longestCaption = 0;
        for(int i = 0; i < mCaptionSections.size(); ++i)
        {
            int captionWidth = getFont()->getWidth(mCaptionSections.at(i));

            if(captionWidth > longestCaption)
            {
                longestCaption = captionWidth;
            }
        }

        setWidth(longestCaption);
        setHeight(getFont()->getHeight() * mCaptionSections.size());
    }
}
