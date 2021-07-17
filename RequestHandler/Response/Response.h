//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_RESPONSE_H
#define HTTP_LIBRARY_RESPONSE_H


#include <vector>
#include <map>
#include <string>
#include <microhttpd.h>
#include "../Request/Request.h"

class Response {
public:
    static Response GetNotFound();

    Response();
    void SetStatus(unsigned int status);
    void SetBody(std::vector<uint8_t> data);
    void SetBodyFromString(const std::string&);
    void RemoveHeader(const std::string& header);
    void SetHeader(const std::string& header, std::string value);
    void RemoveCookie(const std::string& cookie);
    void SetCookie(const std::string& cookie, std::string value);
    void SetResponseAsUnfulfilled();
    void UnsetResponseAsUnfulfilled();
    struct MHD_Response* GetResponse();
    unsigned int GetStatus() const;

private:
    bool unfulfilled;
    unsigned int status_code;
    std::vector<uint8_t> body;
    std::map<std::string,std::string> headers;
    std::map<std::string,std::string> cookies;
    std::string cookie_string;
};


#endif //HTTP_LIBRARY_RESPONSE_H
