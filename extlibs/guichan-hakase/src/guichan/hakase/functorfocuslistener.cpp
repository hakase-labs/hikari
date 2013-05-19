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

#include "guichan/hakase/functorfocuslistener.hpp"

namespace gcn
{
    FunctorFocusListener::FunctorFocusListener(const FocusListenerCallback & callback)
        : mFocusGainedCallback(callback),
          mFocusLostCallback(callback)
    {
    }

    FunctorFocusListener::FunctorFocusListener(const FocusListenerCallback & focusGainedCallback,
                                               const FocusListenerCallback & focusLostCallback)
        : mFocusGainedCallback(focusGainedCallback),
          mFocusLostCallback(focusLostCallback)
    {
    }

    const FocusListenerCallback& FunctorFocusListener::getFocusGainedCallback() const
    {
        return mFocusGainedCallback;
    }

    void FunctorFocusListener::setFocusGainedCallback(const FocusListenerCallback& callback)
    {
        mFocusGainedCallback = callback;
    }

    void FunctorFocusListener::setFocusGainedCallback(const FocusListenerCallback && callback)
    {
        mFocusGainedCallback = std::move(callback);
    }

    const FocusListenerCallback& FunctorFocusListener::getFocusLostCallback() const
    {
        return mFocusLostCallback;
    }

    void FunctorFocusListener::setFocusLostCallback(const FocusListenerCallback& callback)
    {
        mFocusLostCallback = callback;
    }

    void FunctorFocusListener::setFocusLostCallback(const FocusListenerCallback && callback)
    {
        mFocusLostCallback = std::move(callback);
    }

    void FunctorFocusListener::focusGained(const Event& event)
    {
        if (mFocusGainedCallback)
        {
            mFocusGainedCallback(event);
        }
    }

    void FunctorFocusListener::focusLost(const Event& event)
    {
        if (mFocusLostCallback)
        {
            mFocusLostCallback(event);
        }
    }
}
