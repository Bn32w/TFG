//
// Created by bn32w on 17/7/21.
//

#include "Request.h"

#include <utility>
#include <algorithm>

Request::Request(std::string url, std::string method,std::string version) {
    this->path = std::move(url);
    this->method = std::move(method);
    std::for_each(this->method.begin(),this->method.end(),[](char &c){
        c = (char)::tolower(c);
    });

    this->version = std::move(version);
}

void Request::AddParameter(const std::string& name, std::string value) {
    this->params[name] = std::move(value);
}

void Request::AddHeader(const std::string &name, std::string value) {
    this->headers[name] = std::move(value);
}

void Request::AddCookie(const std::string &name, std::string value) {
    this->cookies[name] = std::move(value);
}

void Request::AddDataToBody(const char *upload_data, size_t upload_data_size) {
    for(size_t i = 0; i<upload_data_size;i++){
        this->body.push_back((uint8_t)upload_data[i]);
    }
}

std::string Request::GetMethod() {
    return this->method;
}

std::string Request::GetPath() {
    return this->path;
}

nlohmann::json Request::GetBodyAsJson() {
    return nlohmann::json::parse(this->body);
}

std::string Request::GetHeader(const std::string& header_name) {
    return this->headers[header_name];
}

std::string Request::GetCookie(const std::string& cookie_name) {
    return this->cookies[cookie_name];
}

std::map<std::string, std::string> Request::GetParams() {
    return this->params;
}

std::vector<uint8_t> Request::GetRawBody() {
    return this->body;
}

