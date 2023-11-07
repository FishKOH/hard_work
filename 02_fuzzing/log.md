#### AF++
1. https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.gcc_plugin.md
установлен плагин  
`sudo apt-get install gcc-9-plugin-dev`
1. загружен образ с готовым к использованию AFL++  
`docker pull aflplusplus/aflplusplus`
1. запущен образ с примонтированным кодом  
`docker run -it -v /home/nsv/workspace/ssr/proto/fm-control:/src aflplusplus/aflplusplus`
1. компиляция  
`CC=/usr/local/bin/afl-cc CXX=/usr/local/bin/afl-c++ cmake ../`
1. не найден способ передачи сырых данных 
### libFuzzer
1. проба libFuzzer по мотивам  
https://www.moritz.systems/blog/an-introduction-to-llvm-libfuzzer/
   1. install clang
   1. запуск примера в консоли
   1. запуск примера в IDE
1. настройка Cmake
   ```cmake
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "Clang is required for libFuzzer!")
    endif()
    target_compile_options(${fuzz_test} PUBLIC -fsanitize=fuzzer)
    target_link_options(${fuzz_test} PUBLIC -fsanitize=fuzzer)
    ```
1. подготовка примеров входных бинарных данных 
1. запуск фазз-тестирования
1. добавлен тест фиксирующий поведение при анало
1. правка кода по факту обнаруженной ошибки
1. добавлена в проект инструкция для будущих испоьзований фазз-тестирования

