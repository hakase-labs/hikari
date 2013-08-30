set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( TEST_BASE_DIR "${PROJECT_SOURCE_DIR}/test" )
set( ENGINE_BASE_DIR "${PROJECT_SOURCE_DIR}/engine" )

set( INCLUDE_DIRS
    ${TEST_BASE_DIR}/include
    ${ENGINE_BASE_DIR}/include
)

set( REQUIRED_HIKARI_SOURCE_FILES
    ${ENGINE_BASE_DIR}/src/hikari/client/game/events/EventBus.cpp
    ${ENGINE_BASE_DIR}/src/hikari/client/game/events/EventBusImpl.cpp
    ${ENGINE_BASE_DIR}/src/hikari/client/game/events/EventListenerDelegate.cpp
    ${ENGINE_BASE_DIR}/src/hikari/client/game/events/EventData.cpp
    ${ENGINE_BASE_DIR}/src/hikari/client/game/events/BaseEventData.cpp
    ${ENGINE_BASE_DIR}/src/hikari/core/util/Log.cpp
)

set( TEST_SOURCE_FILES
    ${REQUIRED_HIKARI_SOURCE_FILES}
    src/test/Main.cpp
    src/test/TestVector2.cpp
    src/test/TestPoint2D.cpp
    src/test/TestRectangle2D.cpp
    src/test/TestBoundingBox.cpp
    src/test/TestGeometryUtils.cpp
    src/test/TestEventBusImpl.cpp
)

include_directories( ${INCLUDE_DIRS} )

add_executable( test ${TEST_SOURCE_FILES} ${INCLUDE_DIRS} )