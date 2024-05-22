cmake_minimum_required(VERSION 3.20)

set(VCPKG_ROOT "/home/parrot/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(PasswordManager)

# Set the C++ standard to C++1, set the C++ standard to be required, and set the toolchain file to the vcpkg toolchain file
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the CPM.cmake file and the nlohmann/json package
include(cmake/CPM.cmake)
CPMAddPackage("gh:nlohmann/json@3.10.5")

# Find the required packages
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
find_package(bson-1.0 REQUIRED)
find_package(mongoc-1.0 REQUIRED)

# Add the cryptopp-cmake subdirectory
add_subdirectory(cryptopp-cmake-CRYPTOPP_8_9_0)

include_directories("${VCPKG_ROOT}/installed/x64-linux/include/mongocxx/v_noabi")
include_directories("${VCPKG_ROOT}/installed/x64-linux/include/bsoncxx/v_noabi")
include_directories("${VCPKG_ROOT}/installed/x64-linux/include")
include_directories("${VCPKG_ROOT}/installed/x64-linux/lib")


# Add the executable
add_executable(pm main.cpp Account.cpp AESEncryption.cpp PasswordManager.cpp DatabaseHandler.cpp SHAHashing.cpp)

# Link the necessary libraries
target_link_libraries(pm cryptopp::cryptopp nlohmann_json::nlohmann_json mongo::mongocxx_static mongo::mongoc_static mongo::bson_static mongo::bsoncxx_static)

