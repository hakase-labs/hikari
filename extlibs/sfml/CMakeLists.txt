message(STATUS "Fetching SFML...")

set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_NETWORK FALSE)

FetchContent_MakeAvailable(sfml)