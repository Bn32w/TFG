//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_BASICREQUEST_H
#define HTTP_LIBRARY_BASICREQUEST_H


#include "../RequestHandler/RequestHandler.h"

class BasicRequest : public RequestHandler{
public:
    RequestHandler* CreateRequestHandler() override;
    Response OnGetRequest(Request* request) override;
    Response OnPostRequest(Request* request) override;
    Response OnPutRequest(Request* request) override;
    Response OnDeleteRequest(Request* request) override;
    Response OnUnknownRequest(Request* request) override;

};


#endif //HTTP_LIBRARY_BASICREQUEST_H
