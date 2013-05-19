#ifndef GCN_SFML_HPP
#define GCN_SFML_HPP

#include <guichan/sfml/sfmlfont.hpp>
#include <guichan/sfml/sfmlgraphics.hpp>
#include <guichan/sfml/sfmlimage.hpp>
#include <guichan/sfml/sfmlimageloader.hpp>
#include <guichan/sfml/sfmlinput.hpp>

#include "platform.hpp"

extern "C"
{
    /**
     * Exists to be able to check for Guichan SFML with autotools.
     */
    GCN_EXTENSION_DECLSPEC extern void gcnSFML();
}

#endif // end GCN_SFML_HPP
