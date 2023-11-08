include(FetchContent)
FetchContent_Declare(
  caph
  GIT_REPOSITORY https://github.com/all3n/caph.git
  GIT_TAG main
)
set(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "")
set(ENABLE_TESTS OFF CACHE INTERNAL "")
set(ENABLE_WEB OFF CACHE INTERNAL "") 
FetchContent_MakeAvailable(caph)
