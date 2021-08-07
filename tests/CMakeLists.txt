enable_testing()

find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /constexpr:steps100000000 /bigobj /EHsc /arch:AVX2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconstexpr-ops-limit=3355443200 -ftemplate-depth=9000")

file(GLOB SOURCES *.h *.cpp)
add_executable(ConstGATest ${SOURCES})
target_link_libraries(ConstGATest GTest::GTest GTest::Main ConstGA)

add_test(ConstGATest ConstGATest)