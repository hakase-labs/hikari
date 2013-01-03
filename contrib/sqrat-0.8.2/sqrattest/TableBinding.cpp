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

const Sqrat::string GetGreeting() {
    return _SC("Hello world!");
}

const int AddTwo(int a, int b) {
    return a + b;
}

struct Person {
    string name;
    int age;
};

TEST_F(SqratTest, SimpleTableBinding) {
    DefaultVM::Set(vm);

    string version = _SC("1.0.0");

    // Bind table values and functions
    Table test;
    test
    // Global functions
    .Func(_SC("GetGreeting"), &GetGreeting)
    .Func(_SC("AddTwo"), &AddTwo)

    // Variables
    .SetValue(_SC("version"), version) // Changes to this variable in the script will not propagate back to the native variable
    .SetValue(_SC("author"), _SC("Brandon Jones"))
    .SetValue(_SC("count"), 12);
    ;

    // Bind a class to the table. In this case the table acts somewhat as a namespace
    Class<Person> person;
    person
    .Var(_SC("name"), &Person::name)
    .Var(_SC("age"), &Person::age)
    ;

    test.Bind(_SC("Person"), person);

    // Bind the table to the root table
    RootTable().Bind(_SC("Test"), test);

    Script script;

    try {
        script.CompileString(_SC(" \
			gTest.EXPECT_STR_EQ(Test.version, \"1.0.0\"); \
			gTest.EXPECT_STR_EQ(Test.GetGreeting(), \"Hello world!\"); \
			gTest.EXPECT_INT_EQ(Test.AddTwo(1, 2), 3); \
			Test.count += 3; \
			gTest.EXPECT_INT_EQ(Test.count, 15); \
			\
			p <- Test.Person(); \
			p.name = \"Bobby\"; \
			p.age = 25; \
			gTest.EXPECT_STR_EQ(p.name, \"Bobby\"); \
			gTest.EXPECT_STR_EQ(p.age, 25); \
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