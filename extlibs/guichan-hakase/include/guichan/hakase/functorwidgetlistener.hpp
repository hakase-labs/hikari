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
#ifndef GCN_HIKARI_FUNCTORWIDGETLISTENER_HPP
#define GCN_HIKARI_FUNCTORWIDGETLISTENER_HPP

#include <functional>

#include "guichan/event.hpp"
#include "guichan/widgetlistener.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
    typedef std::function<void (const Event&)> WidgetListenerCallback;

    /**
     * Adapter for attaching a function to a Widget as an WidgetListener.
     *
     * @see Widget::addWidgetListener, Widget::removeWidgetListener
     * @author Zack Mulgrew
     */
    class GCN_EXTENSION_DECLSPEC FunctorWidgetListener : public WidgetListener
    {
    public:

        /**
         * Constructor. Accepts a function object to call when all events are 
         * triggered. Defaults to no-op function.
         */
        explicit FunctorWidgetListener(const WidgetListenerCallback & callback = WidgetListenerCallback());

        /**
         * Constructor. Accepts different function objects to call when each 
         * event is triggered.
         */
        FunctorWidgetListener(const WidgetListenerCallback & resizedCallback,
                              const WidgetListenerCallback & movedCallback,
                              const WidgetListenerCallback & hiddenCallback,
                              const WidgetListenerCallback & shownCallback);

        /**
         * Destructor.
         */
        virtual ~FunctorWidgetListener() { }

        const WidgetListenerCallback& getResizedCallback() const;

        const WidgetListenerCallback& getMovedCallback() const;

        const WidgetListenerCallback& getHiddenCallback() const;

        const WidgetListenerCallback& getShownCallback() const;

        /**
         * Sets the callback function for resize events, copies callback by value.
         */
        void setResizedCallback(const WidgetListenerCallback & callback);

        /**
         * Sets the callback function for resize events, uses an rvalue reference.
         */
        void setResizedCallback(const WidgetListenerCallback && callback);

        /**
         * Sets the callback function for moved events, copies callback by value.
         */
        void setMovedCallback(const WidgetListenerCallback & callback);

        /**
         * Sets the callback function for moved events, uses an rvalue reference.
         */
        void setMovedCallback(const WidgetListenerCallback && callback);

        /**
         * Sets the callback function for "hide" events, copies callback by value.
         */
        void setHiddenCallback(const WidgetListenerCallback & callback);

        /**
         * Sets the callback function for "hide" events, uses an rvalue reference.
         */
        void setHiddenCallback(const WidgetListenerCallback && callback);

        /**
         * Sets the callback function for "show" events, copies callback by value.
         */
        void setShownCallback(const WidgetListenerCallback & callback);

        /**
         * Sets the callback function for "show" events, uses an rvalue reference.
         */
        void setShownCallback(const WidgetListenerCallback && callback);

        // Inherited from WidgetListener

        virtual void widgetResized(const Event& event);

        virtual void widgetMoved(const Event& event);

        virtual void widgetHidden(const Event& event);

        virtual void widgetShown(const Event& event);
    
    private:
        /**
         * Holds the function to call when a widget resized event is triggered.
         */
        WidgetListenerCallback mResizedCallback;

        /**
         * Holds the function to call when a widget moved event is triggered.
         */
        WidgetListenerCallback mMovedCallback;

        /**
         * Holds the function to call when a widget "hide" event is triggered.
         */
        WidgetListenerCallback mHiddenCallback;

        /**
         * Holds the function to call when a widget "show" event is triggered.
         */
        WidgetListenerCallback mShownCallback;

    };
}

#endif // end GCN_HIKARI_FUNCTORWIDGETLISTENER_HPP
