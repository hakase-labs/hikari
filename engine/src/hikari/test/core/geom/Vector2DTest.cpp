#include "hikari/test/core/geom/Vector2DTests.hpp"
#include "hikari/core/math/Vector2.hpp"
#include "hikari/core/util/Log.hpp"

namespace hikari {
namespace testing {

    void Vector2DTests::runAllTests() {
        HIKARI_LOG(debug) << "**** Running all Vector 2D tests ****";
        testConstructors();
        testEqualityAndInequality();
        HIKARI_LOG(debug) << "**** Finished running all Vector 2D tests ****";
    }

    void Vector2DTests::testConstructors() {
        HIKARI_LOG(debug) << ">>> Testing constructors";
        Vector2<float> vecDefault = Vector2<float>();
        HIKARI_LOG(debug) << "Default constructed Vector2<float> = " << vecDefault;
        Vector2<float> vecXandY = Vector2<float>(1.0f, 2.0f);
        HIKARI_LOG(debug) << "Argument constructed Vector2<float> = " << vecXandY;
        Vector2<float> vecCopy = Vector2<float>(vecXandY);
        HIKARI_LOG(debug) << "Copy constructed Vector2<float> = " << vecCopy;
        HIKARI_LOG(debug) << ">>> Finished testing constructors";
    }

    void Vector2DTests::testEqualityAndInequality() {
        HIKARI_LOG(debug) << ">>> Testing equality and inequality";
        
        Vector2<float> a = Vector2<float>(1.3f, 4.55f);
        Vector2<float> b = Vector2<float>(1.3f, 4.55f);
        Vector2<float> c = Vector2<float>(1.5f, 12.1f);

        HIKARI_LOG(debug) << "a == a = " << (a == a);
        HIKARI_LOG(debug) << "a == b = " << (a == b);
        HIKARI_LOG(debug) << "b == a = " << (b == a);
        HIKARI_LOG(debug) << "a == c = " << (a == c);
        HIKARI_LOG(debug) << "a != c = " << (a != c);

        HIKARI_LOG(debug) << ">>> Finished testing equality and inequality";
    }

} // hikari::testing
} // hikari