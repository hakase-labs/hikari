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
#ifndef GCN_HIKARI_LABELEX_HPP
#define GCN_HIKARI_LABELEX_HPP

#include <string>
#include <vector>

#include "guichan/graphics.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
     * Implementation of a label capable of displaying a caption. Captions may
     * contain newline characters and they will be reflected at render-time.
     */
    class GCN_EXTENSION_DECLSPEC LabelEx: public Widget
    {
    public:
        /**
         * Constructor.
         */
        LabelEx();

        /**
         * Constructor. The label will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the label.
         */
        LabelEx(const std::string& caption);

        /**
         * Gets the caption of the label.
         *
         * @return The caption of the label.
         * @see setCaption
         */
        const std::string &getCaption() const;

        /**
         * Sets the caption of the label. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * label's size to fit the caption.
         *
         * @param caption The caption of the label.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the label.
         *
         * @param alignemnt The alignment of the caption of the label.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption. The alignment is relative to
         * the center of the label.
         *
         * @return The alignment of caption of the label.
         * @see setAlignmentm Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Adjusts the label's size to fit the caption.
         */
        void adjustSize();


        // Inherited from Widget

        virtual void draw(Graphics* graphics);

    protected:
        /**
         * Holds the caption of the label.
         */
        std::string mCaption;

        /**
         * Holds the caption, split into separate parts if it contains newline
         * characters. Any time a newline character occurrs in the caption,
         * a substring is created and added to this list.
         */
        std::vector<std::string> mCaptionSections;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;
    };
}

#endif // end GCN_HIKARI_LABELEX_HPP
