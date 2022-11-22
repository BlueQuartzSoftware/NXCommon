find_package(Catch2 CONFIG REQUIRED)

include(Catch)

add_executable(NXCommon_test
    NXCommon_test_main.cpp
    BitTest.cpp
    UuidTest.cpp
)

target_link_libraries(NXCommon_test
    PRIVATE
    NXCommon
    Catch2::Catch2
)

set_target_properties(NXCommon_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:NXCommon>
)

target_compile_options(NXCommon_test
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

target_include_directories(NXCommon_test PRIVATE ${COMPLEX_GENERATED_DIR})

catch_discover_tests(NXCommon_test)