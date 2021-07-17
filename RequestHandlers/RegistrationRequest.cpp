//
// Created by bn32w on 17/7/21.
//

#include "RegistrationRequest.h"

RequestHandler *RegistrationRequest::CreateRequestHandler() {
    return new RegistrationRequest();
}

Response RegistrationRequest::OnGetRequest(Request *request) {
    Response response;
    response.SetBodyFromString(R"(<html lang="en"><body> <form method="POST" action="/registration_form"> <input type="text" name="name"/> <br/> <button type="submit">submit</button> </form></body></html>)");
    return response;
}

Response RegistrationRequest::OnPostRequest(Request *request) {
    Response response;
    response.SetStatus(301);
    response.SetHeader("Location","http://localhost:8888/");
    return response;
}

Response RegistrationRequest::OnPutRequest(Request *request) {
    return RequestHandler::OnPutRequest(request);
}

Response RegistrationRequest::OnDeleteRequest(Request *request) {
    return RequestHandler::OnDeleteRequest(request);
}

Response RegistrationRequest::OnUnknownRequest(Request *request) {
    return RequestHandler::OnUnknownRequest(request);
}
