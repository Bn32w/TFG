//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_FILEREQUEST_H
#define HTTP_LIBRARY_FILEREQUEST_H


#include "../RequestHandler.h"
#include "../../Configuration/Configuration.h"
#include <fstream>
#include <sys/stat.h>
#include <iterator>

class FileRequest : public RequestHandler{
public:
    RequestHandler* CreateRequestHandler() override;
    Response OnGetRequest(Request* request) override;
    Response OnPostRequest(Request* request) override;
    Response OnPutRequest(Request* request) override;
    Response OnDeleteRequest(Request* request) override;
    Response OnUnknownRequest(Request* request) override;

private:
    static std::string GetCleanPath(std::string);
    static bool FileExists(const std::string& file);

    std::string GetAppropriateMime(const std::string& basicString);
};


#endif //HTTP_LIBRARY_FILEREQUEST_H
