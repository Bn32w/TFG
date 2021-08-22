//
// Created by bn32w on 17/7/21.
//

#ifndef CODE_CONFIGURATION_H
#define CODE_CONFIGURATION_H
#include <microhttpd.h>
#include <map>
#include <string>
#include "../RequestHandler/RequestHandler.h"
#include "../RequestHandlers/BasicRequest.h"
#include "../RequestHandlers/RegistrationRequest.h"
#include "../RequestHandlers/TutorialClass.h"

#define PORT 8888
#define CONNECTION_FLAG MHD_USE_INTERNAL_POLLING_THREAD
#define STATIC_FILE_PATH "/home/bn32w/TFG/static"
#define TEMPLATE_FILE_PATH "/home/bn32w/TFG/template"

const std::map<std::string,RequestHandler*> handler_configuration = {
        {"/",new BasicRequest()},
        {"/registration_form",new RegistrationRequest()},
        {"/login", new RegistrationRequest()},
        {"/tutorial", new TutorialClass()}
};

const std::map<std::string,std::string> mime_types = {
        {"html","text/html"},
        {"htm","text/html"},
        {"css","text/css"},
        {"gif","image/gif"},
        {"jpeg","image/jpeg"},
        {"jpg","image/jpg"},
        {"png","image/png"},
        {"svg","image/svg+xml"},
        {"js","application/javascript"}
};

#endif //CODE_CONFIGURATION_H

