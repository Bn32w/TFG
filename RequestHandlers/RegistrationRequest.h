//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_REGISTRATIONREQUEST_H
#define HTTP_LIBRARY_REGISTRATIONREQUEST_H


#include "../RequestHandler/RequestHandler.h"

class RegistrationRequest : public RequestHandler{
    RequestHandler* CreateRequestHandler() override;
    Response OnGetRequest(Request* request) override;
    Response OnPostRequest(Request* request) override;
    Response OnPutRequest(Request* request) override;
    Response OnDeleteRequest(Request* request) override;
    Response OnUnknownRequest(Request* request) override;
};


#endif //HTTP_LIBRARY_REGISTRATIONREQUEST_H
