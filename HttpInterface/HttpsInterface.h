//
// Created by bn32w on 17/7/21.
//

#ifndef CODE_HTTPS_INTERFACE_H
#define CODE_HTTPS_INTERFACE_H

#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <cstring>
#include <string>
#include <microhttpd.h>
#include <iostream>
#include <stdio.h>

#include "../RequestHandler/Response/Response.h"
#include "../RequestHandler/Request/Request.h"
#include "../RequestHandler/RequestHandler.h"

typedef struct {
    Request* request;
    RequestHandler* requestHandler;
}local_data;

class HttpsInterface {
public:
    HttpsInterface(int port,  enum MHD_FLAG flag,std::map<std::string,RequestHandler*> request_handlers_config);
    ~HttpsInterface();
private:
    static RequestHandler* file_request_handler;
    static std::map<std::string,RequestHandler*> request_handlers;
    struct MHD_Daemon *daemon;
    static enum MHD_Result manage_request (void *cls, struct MHD_Connection *connection,
                                            const char *url, const char *method,
                                            const char *version, const char *upload_data,
                                            size_t *upload_data_size, void **con_cls);

    static void
    request_completed (void *cls, struct MHD_Connection *connection,
                       void **con_cls, enum MHD_RequestTerminationCode toe);

    static enum MHD_Result get_args(void *cls, enum MHD_ValueKind, const char* key, const char* value);
    static enum MHD_Result get_header(void *cls, enum MHD_ValueKind, const char* key, const char* value);
    static enum MHD_Result get_cookies(void *cls, enum MHD_ValueKind, const char* key, const char* value);

};


#endif //CODE_HTTPS_INTERFACE_H
