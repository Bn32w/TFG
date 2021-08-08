//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_REQUEST_H
#define HTTP_LIBRARY_REQUEST_H

#include <map>
#include <string>
#include <microhttpd.h>
#include <vector>
#include <iostream>
#include "../JsonParser/json.hpp"

class Request {
public:
    Request(std::string url, std::string method,std::string version);
    void AddParameter(const std::string& name, std::string value);
    void AddHeader(const std::string& name, std::string value);
    void AddCookie(const std::string& name, std::string value);
    void AddDataToBody(const char *upload_data, size_t upload_data_size );
    std::string GetMethod();
    std::string GetPath();
    nlohmann::json GetBodyAsJson();
private:
    std::map<std::string,std::string> headers;
    std::map<std::string,std::string> params;
    std::map<std::string,std::string> cookies;
    std::string method;
    std::vector<uint8_t> body;
    std::string path;
    std::string version;
};


#endif //HTTP_LIBRARY_REQUEST_H
