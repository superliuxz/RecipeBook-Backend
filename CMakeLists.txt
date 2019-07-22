cmake_minimum_required(VERSION 3.14)
project(backend)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(backend main.cpp MyMiddleware.cpp MyMiddleware.h Helpers.h)

# Boost
set(BOOST_ROOT /usr/local/Cellar/boost@1.60/1.60.0)
find_package(Boost 1.60.0 COMPONENTS system filesystem REQUIRED)
SET(Boost_USE_MULTITHREADED ON)

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif ()
if (Boost_FOUND)

    target_link_libraries(backend ${Boost_LIBRARIES})

endif ()

# crow
include_directories(crow/include)

# openssl
set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2s)
find_package(OpenSSL REQUIRED)
target_link_libraries(backend OpenSSL::SSL)

# curl
find_package(CURL)
target_link_libraries(backend ${CURL_LIBRARY})

# cpr
add_subdirectory(cpr)
target_link_libraries(backend ${CPR_LIBRARIES})
