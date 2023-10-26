set(httplib httplib.h )

add_library(httplib ${httplib})

include_directories(app PUBLIC 
                                "${PROJECT_SOURCE_DIR}"
                                "${PROJECT_SOURCE_DIR}/libOnvif"
                                "${PROJECT_SOURCE_DIR}/include")
set(CXXFLAGS  "-DWITH_OPENSSL -Wall -g -DWITH_DOM -DWITH_ZLIB -I. -lcrypto -lssl -lz -ljsoncpp -lpthread -std=c++11")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")