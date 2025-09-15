include(FetchContent)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY "https://github.com/SFML/SFML"
  GIT_TAG 5383d2b3948f805af55c9f8a4587ac72ec5981d1 # 2.6.2
)

add_subdirectory(sfml)
