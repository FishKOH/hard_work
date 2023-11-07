set (fuzz_test RliStreamProtocol-fuzz-tests)

add_executable(${fuzz_test}
    "${CMAKE_CURRENT_LIST_DIR}/FuzzingTest.cpp"
)

target_include_directories(${fuzz_test}
    SYSTEM PRIVATE
)

target_link_libraries(${fuzz_test}
    PRIVATE
    RliStreamProtocol
)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Clang is required for libFuzzer!")
endif()
target_compile_options(${fuzz_test} PUBLIC -fsanitize=fuzzer,address)
target_link_options(${fuzz_test} PUBLIC -fsanitize=fuzzer,address)
