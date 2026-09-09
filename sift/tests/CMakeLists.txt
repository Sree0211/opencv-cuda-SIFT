# tests/CMakeLists.txt

find_package(GTest REQUIRED)
include(GoogleTest)

# Gaussian kernel
add_executable(test_gaussian
    test_gaussian.cpp
)

target_link_libraries(test_gaussian
    PRIVATE
    sift 
    GTest::gtest_main
)
target_include_directories(test_gaussian 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/sift
)
gtest_discover_tests(test_gaussian)

# Gaussian Pyramid
add_executable(test_gaussian_pyramid
    test_gaussian_pyramid.cpp
)

target_link_libraries(test_gaussian_pyramid
    PRIVATE
    sift 
    GTest::gtest_main
)

target_include_directories(test_gaussian_pyramid 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/sift
)

gtest_discover_tests(test_gaussian_pyramid)

# Difference of Gaussian
add_executable(test_dog
    test_dog.cpp
)

target_link_libraries(test_dog
    PRIVATE
    sift 
    GTest::gtest_main
)
target_include_directories(test_dog 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/sift
)
gtest_discover_tests(test_dog)
