//
// Sqrat: Squirrel C++ Binding Utility
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

/*! \mainpage Sqrat Main Page
 *
 * \section intro_sec Introduction
 *
 * Sqrat is a C++ library for Squirrel that facilitates exposing classes and other native functionality to Squirrel scripts. It models the underlying Squirrel API closely to give access to a wider range of functionality than other binding libraries. In addition to the binding library, Sqrat features a threading library and a module import library.
 *
 * \section install_sec Installation
 *
 * Sqrat only contains C++ headers so for installation you just need to copy the files in the include directory to some common header path.
 *
 * \section discuss_sec Discussion and User Support
 *
 * Discussion about Sqrat happens at the Squirrel language forum, the Bindings section
 * http://squirrel-lang.org/forums/default.aspx?g=topics&f=4
 *
 * \section bug_sec Bug Reporting
 *
 * Bug reports or feature enhancement requests and patches can be submitted at the SourceForge Sqrat site
 * https://sourceforge.net/p/scrat/sqrat/
 */

#if !defined(_SCRAT_MAIN_H_)
#define _SCRAT_MAIN_H_

#include <squirrel.h>

#include "sqrat/sqratTable.h"
#include "sqrat/sqratClass.h"
#include "sqrat/sqratFunction.h"
#include "sqrat/sqratConst.h"
#include "sqrat/sqratUtil.h"
#include "sqrat/sqratScript.h"
#include "sqrat/sqratArray.h"

#endif
