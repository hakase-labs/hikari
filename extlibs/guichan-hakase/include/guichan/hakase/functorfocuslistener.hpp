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

#ifndef GCN_HIKARI_FUNCTORFOCUSLISTENER_HPP
#define GCN_HIKARI_FUNCTORFOCUSLISTENER_HPP

#include <functional>
#include <string>

#include "guichan/event.hpp"
#include "guichan/platform.hpp"
#include "guichan/focuslistener.hpp"

namespace gcn
{
    typedef std::function<void (const Event&)> FocusListenerCallback;

    /**
     * Adapter for attaching a function to a Widget as a FocusListener.
     *
     * @see Widget::addFocusListener, Widget::removeFocusListener
     * @author Zack Mulgrew
     */
    class GCN_EXTENSION_DECLSPEC FunctorFocusListener : public FocusListener
    {
    public:
        /**
         * Constructor. Uses same callback for focus gained and lost events. 
         * Callback is optional and defaults to no-op function.
         */
        FunctorFocusListener(const FocusListenerCallback & callback = FocusListenerCallback());

        /**
         * Constructor. Allows different callbacks for focus gained and lost 
         * events.
         */
        FunctorFocusListener(const FocusListenerCallback & focusGainedCallback,
                const FocusListenerCallback & focusLostCallback);

        /**
         * Destructor.
         */
        virtual ~FunctorFocusListener() { }

        const FocusListenerCallback& getFocusGainedCallback() const;

        void setFocusGainedCallback(const FocusListenerCallback & callback);

        void setFocusGainedCallback(const FocusListenerCallback && callback);

        const FocusListenerCallback& getFocusLostCallback() const;

        void setFocusLostCallback(const FocusListenerCallback & callback);

        void setFocusLostCallback(const FocusListenerCallback && callback);

        /**
         * Called when a widget gains focus. 
         *
         * @param event Discribes the event.
         */
        virtual void focusGained(const Event& event);

        /**
         * Called when a widget loses focus. 
         *
         * @param event Discribes the event.
         */
        virtual void focusLost(const Event& event);

    private:
        /**
         * Holds the function to call when "focusGained" event is triggered.
         */
        FocusListenerCallback mFocusGainedCallback;

        /**
         * Holds the function to call when "focusLost" event is triggered.
         */
        FocusListenerCallback mFocusLostCallback;
    };
}

#endif // end GCN_HIKARI_FUNCTORFOCUSLISTENER_HPP
