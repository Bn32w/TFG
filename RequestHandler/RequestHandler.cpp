//
// Created by bn32w on 17/7/21.
//

#include <algorithm>
#include "RequestHandler.h"

Response RequestHandler::HandleRequest(Request* request) {

    if(request->GetMethod() == "get") return this->OnGetRequest(request);
    if(request->GetMethod() == "post") return this->OnPostRequest(request);
    if(request->GetMethod() == "put") return this->OnPutRequest(request);
    if(request->GetMethod() == "delete") return this->OnDeleteRequest(request);
    return this->OnUnknownRequest(request);
}
