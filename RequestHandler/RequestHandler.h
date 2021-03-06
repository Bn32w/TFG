//
// Created by bn32w on 17/7/21.
//

#ifndef HTTP_LIBRARY_REQUESTHANDLER_H
#define HTTP_LIBRARY_REQUESTHANDLER_H

#include "Response/Response.h"
#include "Request/Request.h"
#include "JsonParser/json.hpp"
#include "TemplateManager/TemplateManager.h"

class RequestHandler {
public:
    virtual RequestHandler* CreateRequestHandler() = 0;

    Response HandleRequest(Request* request);
    virtual Response OnGetRequest(Request* request){return Response::GetNotFound();}
    virtual Response OnPostRequest(Request* request){return Response::GetNotFound();}
    virtual Response OnPutRequest(Request* request){return Response::GetNotFound();}
    virtual Response OnDeleteRequest(Request* request){return Response::GetNotFound();}
    virtual Response OnUnknownRequest(Request* request){return Response::GetNotFound();}
    virtual ~RequestHandler()= default;
};



#endif //HTTP_LIBRARY_REQUESTHANDLER_H
