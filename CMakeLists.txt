cmake_minimum_required(VERSION 3.14)
project(PhoneDirectory)

set(CMAKE_CXX_STANDARD 20)

# Find the Crow and nlohmann_json packages
find_package(Crow REQUIRED)
find_package(libpqxx REQUIRED)


add_executable(PhoneDirectory main.cpp)

# Link with Crow and PostgreSQL
target_link_libraries(PhoneDirectory PRIVATE
        Crow::Crow
        libpqxx::pqxx)
