//
// Created by bn32w on 17/7/21.
//

#include "BasicRequest.h"

RequestHandler *BasicRequest::CreateRequestHandler() {
    return new BasicRequest();
}

Response BasicRequest::OnGetRequest(Request* request) {
    Response response;
    response.SetBodyFromString("<html><body>HOLA Q HACE</body></html>");
    return response;
}

Response BasicRequest::OnPostRequest(Request* request) {
    Response response;
    response.SetBodyFromString(R"({"rest": "Soy rest"})");
    response.SetHeader("Content-Type","application/json");
    return response;
}

Response BasicRequest::OnPutRequest(Request* request) {
    return RequestHandler::OnPutRequest(request);
}

Response BasicRequest::OnDeleteRequest(Request* request) {
    return RequestHandler::OnDeleteRequest(request);
}

Response BasicRequest::OnUnknownRequest(Request* request) {
    return RequestHandler::OnUnknownRequest(request);
}
