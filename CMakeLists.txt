project(spn CXX)

find_package(sqlite3)

set(MECAB_PREFIX "" CACHE STRING "mecab-config --prefix")
set(CMAKE_CXX_STANDARD 11)
message("MeCab prefix: ${MECAB_PREFIX}")

add_subdirectory(spn)
