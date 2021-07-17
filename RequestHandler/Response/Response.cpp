//
// Created by bn32w on 17/7/21.
//

#include "Response.h"

#include <utility>

Response::Response() {
    unfulfilled= false;
    status_code = 200;
    this->headers[MHD_HTTP_HEADER_CONTENT_TYPE] = "text/html";
}

void Response::SetStatus(unsigned int status) {
    this->status_code = status;
}

void Response::SetBody(std::vector<uint8_t> data) {
    this->body = std::vector<uint8_t>(std::move(data));
}

void Response::RemoveHeader(const std::string& header) {
    if(this->headers.find(header)!=this->headers.end()){
        this->headers.erase(header);
    }
}

void Response::SetHeader(const std::string& header, std::string value) {
    this->headers[header] = std::move(value);
}

void Response::SetResponseAsUnfulfilled() {
    this->unfulfilled = true;
}

void Response::UnsetResponseAsUnfulfilled() {
    this->unfulfilled = false;
}

struct MHD_Response *Response::GetResponse() {
    if(this->unfulfilled){
        return nullptr;
    }
    struct MHD_Response* response;
    if(!this->body.empty()){
        response = MHD_create_response_from_buffer(this->body.size(),(void*)&(this->body[0]),MHD_RESPMEM_MUST_COPY);
    }else{
        response = MHD_create_response_from_buffer(0, nullptr,MHD_RESPMEM_PERSISTENT);
    }
    for(const auto& it : this->headers){
        MHD_add_response_header(response,it.first.c_str(),it.second.c_str());
    }
    for(const auto& it : this->cookies){
        std::string cookie = it.first+"="+it.second;
        MHD_add_response_header(response,MHD_HTTP_HEADER_SET_COOKIE,cookie.c_str());
    }

    return response;
}

void Response::SetBodyFromString(const std::string& data) {
    this->body = std::vector<uint8_t>();
    for(char character : data){
        this->body.push_back((uint8_t)character);
    }
}

unsigned int Response::GetStatus() const {
    return this->status_code;
}

void Response::SetCookie(const std::string &cookie, std::string value) {
    this->cookies[cookie] = std::move(value);
}

void Response::RemoveCookie(const std::string &cookie) {
    if(this->cookies.find(cookie)!=this->cookies.end()){
        this->cookies.erase(cookie);
    }
}

Response Response::GetNotFound() {
    Response response;
    response.SetStatus(404);
    return response;
}


