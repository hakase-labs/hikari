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

class Employee {
public:
    Employee() : supervisor(NULL) {}

    Employee(const Employee& e) :
        firstName(e.firstName), lastName(e.lastName),
        age(e.age), department(e.department),
        wage(e.wage), supervisor(e.supervisor) {

    }

    void GiveRaise(float percent) {
        wage += (wage * percent);
    }

    const string ToString() const {
        std::basic_stringstream<SQChar> out;
        out << _SC("Employee: ") << lastName << _SC(", ") << firstName << _SC("\n");
        out << _SC("Age: ") << age << _SC("\n");
        out << _SC("Department: ") << department << _SC("\n");
        out << _SC("Hourly Wage: ") << wage << _SC("\n");
        if(supervisor != NULL) {
            out << _SC("Supervisor: ") << supervisor->lastName << _SC(", ") << supervisor->firstName << _SC("\n");
        }
        return out.str();
    }

    string firstName;
    string lastName;
    int age;
    string department;
    float wage;
    SQChar * middleName;
    const SQChar * gender;
    Employee* supervisor;
};

TEST_F(SqratTest, ClassInstances) {
    DefaultVM::Set(vm);

    Class<Employee> employee;
    employee
    .Var(_SC("firstName"), &Employee::firstName)
    .Var(_SC("lastName"), &Employee::lastName)
    .Var(_SC("age"), &Employee::age)
    .Var(_SC("department"), &Employee::department)
    .Var(_SC("wage"), &Employee::wage)
    .Var(_SC("supervisor"), &Employee::supervisor)
    .Var(_SC("middleName"), &Employee::middleName)
    .Var(_SC("gender"), &Employee::gender)

    .Func(_SC("GiveRaise"), &Employee::GiveRaise)
    .Func(_SC("_tostring"), &Employee::ToString)
    ;

    // Bind the class to the root table
    RootTable().Bind(_SC("Employee"), employee);

    // Create an employee and set it as an instance in the script
    Employee bob;
    bob.firstName = _SC("Bob");
    bob.lastName = _SC("Smith");
    bob.age = 42;
    bob.department = _SC("Accounting");
    bob.wage = 21.95f;
    bob.gender = _SC("Male");
    bob.middleName = _SC("A");

    RootTable().SetInstance(_SC("bob"), &bob);

    Script script;

    try {
        script.CompileString(_SC(" \
			steve <- Employee(); \
			steve.firstName = \"Steve\"; \
			steve.lastName = \"Jones\"; \
			steve.age = 34; \
			steve.wage = 35.00; \
			steve.department = \"Management\"; \
			steve.gender = \"male\"; \
			steve.middleName = \"B\"; \
			\
			gTest.EXPECT_INT_EQ(steve.age, 34); \
			gTest.EXPECT_FLOAT_EQ(steve.wage, 35.00); \
			gTest.EXPECT_STR_EQ(steve.lastName, \"Jones\"); \
			gTest.EXPECT_STR_EQ(steve.middleName, \"B\"); \
			gTest.EXPECT_STR_EQ(steve.gender, \"male\"); \
			\
			\
			bob.age += 1; \
			bob.GiveRaise(0.02); \
			bob.supervisor = steve; \
			\
			gTest.EXPECT_INT_EQ(bob.age, 43); \
			gTest.EXPECT_FLOAT_EQ(bob.wage, 22.389); \
			gTest.EXPECT_STR_EQ(bob.lastName, \"Smith\"); \
			gTest.EXPECT_STR_EQ(bob.middleName, \"A\"); \
			gTest.EXPECT_STR_EQ(bob.gender, \"Male\"); \
			\
			// Uncomment the following to see _tostring demonstrated \
			//::print(steve); \
			//::print(\"===========\\n\"); \
			//::print(bob); \
			"));
    } catch(Exception ex) {
        FAIL() << _SC("Compile Failed: ") << ex.Message();
    }

    try {
        script.Run();
    } catch(Exception ex) {
        FAIL() << _SC("Run Failed: ") << ex.Message();
    }

    // Since he was set as an instance, changes to Bob in the script carry through to the native object
    EXPECT_EQ(bob.age, 43);
    EXPECT_FLOAT_EQ(bob.wage, 22.389f);

    // Steve can also be retreived from the script as an employee:
    Object steveObj = RootTable().GetSlot(_SC("steve"));
    ASSERT_FALSE(steveObj.IsNull());

    Employee* steve = steveObj.Cast<Employee*>();
    ASSERT_FALSE(steve == NULL);

    EXPECT_EQ(steve->age, 34);
    EXPECT_FLOAT_EQ(steve->wage, 35.00f);
}