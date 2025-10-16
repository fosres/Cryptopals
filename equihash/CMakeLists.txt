set(EQUIHASH_SOURCES
    src/equihash.cpp
    src/blake2b.cpp
)

add_library(equihash STATIC ${EQUIHASH_SOURCES})
target_include_directories(equihash PUBLIC include)

target_compile_features(equihash PUBLIC cxx_std_17)

add_executable(test_equihash tests/test_equihash.cpp)
target_link_libraries(test_equihash PRIVATE equihash)

add_test(NAME equihash_test COMMAND test_equihash)
