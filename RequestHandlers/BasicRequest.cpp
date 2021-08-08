//
// Created by bn32w on 17/7/21.
//

#include "BasicRequest.h"

RequestHandler *BasicRequest::CreateRequestHandler() {
    return new BasicRequest();
}

Response BasicRequest::OnGetRequest(Request* request) {
    Response response;
    response.SetBody(TemplateManager::GetInstance()->GetDataFromTemplate("template",{{"title","Redone"},
                                                                                     {"form_action","/registration_form"},
                                                                                     {"number","4"}
                                                                                }));
    return response;
}

Response BasicRequest::OnPostRequest(Request* request) {
    Response response;
    nlohmann::json body_data = request->GetBodyAsJson();
    std::string sql_command = body_data["sql"];
    nlohmann::json response_data;
    response_data["sql_redone"] = sql_command;
    response.SetBodyFromString(response_data.dump(4));
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
