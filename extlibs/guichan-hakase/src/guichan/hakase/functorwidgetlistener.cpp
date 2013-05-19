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

#include <utility>

#include "guichan/hakase/functorwidgetlistener.hpp"

namespace gcn
{
    FunctorWidgetListener::FunctorWidgetListener(const WidgetListenerCallback & callback)
        : mResizedCallback(callback),
          mMovedCallback(callback),
          mHiddenCallback(callback),
          mShownCallback(callback)
    {
    }

    FunctorWidgetListener::FunctorWidgetListener(const WidgetListenerCallback & resizedCallback,
                                                 const WidgetListenerCallback & movedCallback,
                                                 const WidgetListenerCallback & hiddenCallback,
                                                 const WidgetListenerCallback & shownCallback)
        : mResizedCallback(resizedCallback),
          mMovedCallback(movedCallback),
          mHiddenCallback(hiddenCallback),
          mShownCallback(shownCallback)
    {
    }

    const WidgetListenerCallback& FunctorWidgetListener::getResizedCallback() const
    {
        return mResizedCallback;
    }

    const WidgetListenerCallback& FunctorWidgetListener::getMovedCallback() const
    {
        return mMovedCallback;
    }

    const WidgetListenerCallback& FunctorWidgetListener::getHiddenCallback() const
    {
        return mHiddenCallback;
    }

    const WidgetListenerCallback& FunctorWidgetListener::getShownCallback() const
    {
        return mShownCallback;
    }

    void FunctorWidgetListener::setResizedCallback(const WidgetListenerCallback & callback)
    {
        mResizedCallback = callback;
    }

    void FunctorWidgetListener::setResizedCallback(const WidgetListenerCallback && callback)
    {
        mResizedCallback = std::move(callback);
    }

    void FunctorWidgetListener::setMovedCallback(const WidgetListenerCallback & callback)
    {
        mMovedCallback = callback;
    }

    void FunctorWidgetListener::setMovedCallback(const WidgetListenerCallback && callback)
    {
        mMovedCallback = std::move(callback);
    }

    void FunctorWidgetListener::setHiddenCallback(const WidgetListenerCallback & callback)
    {
        mHiddenCallback = callback;
    }

    void FunctorWidgetListener::setHiddenCallback(const WidgetListenerCallback && callback)
    {
        mHiddenCallback = std::move(callback);
    }

    void FunctorWidgetListener::setShownCallback(const WidgetListenerCallback & callback)
    {
        mShownCallback = callback;
    }

    void FunctorWidgetListener::setShownCallback(const WidgetListenerCallback && callback)
    {
        mShownCallback = std::move(callback);
    }

    void FunctorWidgetListener::widgetResized(const Event& event)
    {
        if (mResizedCallback)
        {
            mResizedCallback(event);
        }
    }

    void FunctorWidgetListener::widgetMoved(const Event& event)
    {
        if (mMovedCallback)
        {
            mMovedCallback(event);
        }
    }

    void FunctorWidgetListener::widgetHidden(const Event& event)
    {
        if (mHiddenCallback)
        {
            mHiddenCallback(event);
        }
    }

    void FunctorWidgetListener::widgetShown(const Event& event)
    {
        if (mShownCallback)
        {
            mShownCallback(event);
        }
    }
}
