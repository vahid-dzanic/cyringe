# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(GTest REQUIRED)

add_executable(cyringe_test
  src/cyringe_test.cpp)

target_link_libraries(cyringe_test
  PRIVATE
    ${PROJECT_NAME}
    gtest
    gmock
    gtest_main
    pthread)

add_test(cyringe_test cyringe_test)
