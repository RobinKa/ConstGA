file(GLOB SOURCES src/*.cpp)
add_library (ConstGA ${SOURCES})
target_include_directories(ConstGA PUBLIC include)
#target_compile_features(ConstGA PUBLIC cxx_std_20)