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

#include <gtest/gtest.h>
#include <sqrat.h>
#include "Fixture.h"

using namespace Sqrat;

class Speaker {
public:
    int Echo() {
        return 0;
    }
    int Echo(int val) {
        return val;
    }
};

int GlobalEcho() {
    return 0;
}
int GlobalEcho(int val) {
    return val;
}

TEST_F(SqratTest, OverloadedMemberFunction) {
    DefaultVM::Set(vm);

    // Member function overloads
    RootTable().Bind(_SC("Speaker"),
                     Class<Speaker>()
                     .Overload<int (Speaker::*)()>(_SC("Echo"), &Speaker::Echo)
                     .Overload<int (Speaker::*)(int)>(_SC("Echo"), &Speaker::Echo)
                    );

    // Global Function overloads
    RootTable().Overload<int (*)()>(_SC("GlobalEcho"), &GlobalEcho);
    RootTable().Overload<int (*)(int)>(_SC("GlobalEcho"), &GlobalEcho);

    Script script;

    try {
        script.CompileString(_SC(" \
			s <- Speaker(); \
			\
			gTest.EXPECT_INT_EQ(0, s.Echo()); \
			gTest.EXPECT_INT_EQ(1, s.Echo(1)); \
			gTest.EXPECT_INT_EQ(0, GlobalEcho()); \
			gTest.EXPECT_INT_EQ(1, GlobalEcho(1)); \
			"));
    } catch(Exception ex) {
        FAIL() << _SC("Compile Failed: ") << ex.Message();
    }

    try {
        script.Run();
    } catch(Exception ex) {
        FAIL() << _SC("Run Failed: ") << ex.Message();
    }
}

//
// Overload test with const functions, based on scenario provided by emeyex
//

class Entity {
public:
    unsigned int QueryEnumValue( unsigned int enumKey, unsigned int enumValueDefault ) const {
        return enumKey;
    }
    unsigned int QueryEnumValue( unsigned int enumKey ) const {
        return QueryEnumValue( enumKey, 0 );
    }
};

TEST_F(SqratTest, ConstOverloadTest) {
    DefaultVM::Set(vm);

    // Member function overloads
    RootTable().Bind(_SC("Entity"),
                     Class<Entity>()
                     .Overload<unsigned int (Entity::*)(unsigned int, unsigned int) const>(_SC("QueryEnumValue"), &Entity::QueryEnumValue)
                     .Overload<unsigned int (Entity::*)(unsigned int) const>(_SC("QueryEnumValue"), &Entity::QueryEnumValue)
                    );

    Script script;

    try {
        script.CompileString(_SC(" \
			e <- Entity(); \
			\
			gTest.EXPECT_INT_EQ(1, e.QueryEnumValue(1, 0)); \
			gTest.EXPECT_INT_EQ(2, e.QueryEnumValue(2)); \
			"));
    } catch(Exception ex) {
        FAIL() << _SC("Compile Failed: ") << ex.Message();
    }

    try {
        script.Run();
    } catch(Exception ex) {
        FAIL() << _SC("Run Failed: ") << ex.Message();
    }
}

