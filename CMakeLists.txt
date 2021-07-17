cmake_minimum_required(VERSION 3.16)
project(http_library)

set(CMAKE_CXX_STANDARD 14)
add_executable(http_library main.cpp HttpInterface/HttpsInterface.cpp HttpInterface/HttpsInterface.h Configuration/Configuration.h RequestHandler/Request/Request.cpp RequestHandler/Request/Request.h RequestHandler/Response/Response.cpp RequestHandler/Response/Response.h RequestHandler/RequestHandler.cpp RequestHandler/RequestHandler.h RequestHandlers/BasicRequest.cpp RequestHandlers/BasicRequest.h RequestHandlers/RegistrationRequest.cpp RequestHandlers/RegistrationRequest.h RequestHandler/FileHandlingClass/FileRequest.cpp RequestHandler/FileHandlingClass/FileRequest.h)
target_link_libraries(http_library microhttpd)