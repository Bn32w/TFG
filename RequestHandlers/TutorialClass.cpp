//
// Created by bn32w on 12/8/21.
//

#include "TutorialClass.h"
RequestHandler *TutorialClass::CreateRequestHandler() {
    return new TutorialClass();
}

Response TutorialClass::OnGetRequest(Request *request) {
    Response response;
    response.SetBodyFromString("<h1>hola mundo</h1>");
    return response;
}

Response TutorialClass::OnPostRequest(Request *request) {
    int result = 10;
    nlohmann::json response = {{"response", result}};
    Response resp = Response();
    resp.SetBodyFromString(response.dump(4));
    resp.SetHeader("Content-Type","application/json");
    return resp;
}

Response TutorialClass::OnPutRequest(Request *request) {
    return RequestHandler::OnPutRequest(request);
}

Response TutorialClass::OnDeleteRequest(Request *request) {
    return RequestHandler::OnDeleteRequest(request);
}

Response TutorialClass::OnUnknownRequest(Request *request) {
    return RequestHandler::OnUnknownRequest(request);
}