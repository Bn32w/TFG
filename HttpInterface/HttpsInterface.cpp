//
// Created by bn32w on 17/7/21.
//

#include "HttpsInterface.h"

#include <utility>
#include "../RequestHandlers/BasicRequest.h"
#include "../RequestHandler/FileHandlingClass/FileRequest.h"

std::map<std::string,RequestHandler*> HttpsInterface::request_handlers = std::map<std::string ,RequestHandler*>();
RequestHandler* HttpsInterface::file_request_handler = new FileRequest();

HttpsInterface::HttpsInterface(int port,enum MHD_FLAG flag,std::map<std::string,RequestHandler*> request_handlers_config) {
    this->daemon = MHD_start_daemon(MHD_USE_AUTO | flag, port, nullptr,nullptr,
                                    &HttpsInterface::manage_request,nullptr,
                                    MHD_OPTION_NOTIFY_COMPLETED, HttpsInterface::request_completed,nullptr,MHD_OPTION_END);
    if(this->daemon== nullptr){
        exit(1);
    }
    HttpsInterface::request_handlers = std::move(request_handlers_config);
    std::cout<<"HTTP SERVER LAUNCHED AT PORT: "<<port<<std::endl;
}

enum MHD_Result
HttpsInterface::manage_request(void *cls, struct MHD_Connection *connection, const char *url, const char *method,
                                const char *version, const char *upload_data, size_t *upload_data_size,
                                void **con_cls) {
    local_data* data;
    if(*con_cls == nullptr){
        data = (local_data*)malloc(sizeof(local_data));
        data->request = nullptr;
        data->requestHandler = nullptr;
        *con_cls = (void*)data;
    }else{
        data = (local_data*)*con_cls;
    }
    if(*upload_data_size==0 && data->request == nullptr){
        char* escaped_url = (char*)url, *escaped_method = (char*)method, *escaped_version = (char*)version;
        MHD_http_unescape(escaped_url);
        MHD_http_unescape(escaped_method);
        MHD_http_unescape(escaped_version);
        data->request= new Request(std::string(escaped_url),std::string(escaped_method),std::string(escaped_version));
        MHD_get_connection_values(connection,MHD_HEADER_KIND,&HttpsInterface::get_header,data->request);
        MHD_get_connection_values(connection,MHD_COOKIE_KIND,&HttpsInterface::get_cookies,data->request);
        MHD_get_connection_values(connection,MHD_GET_ARGUMENT_KIND,&HttpsInterface::get_args,data->request);
        *con_cls = (void*)data;
        return MHD_YES;
    }
    if(*upload_data_size==0 && data->request!=nullptr){
        std::string last_element_on_path = data->request->GetPath().substr(data->request->GetPath().rfind('/')+1);
        Response resp;
        if(last_element_on_path.find('.')!=std::string::npos){
            if(data->requestHandler== nullptr){
                data->requestHandler=file_request_handler->CreateRequestHandler();
            }
            resp = data->requestHandler->HandleRequest(data->request);
        }else{
            if(data->requestHandler==nullptr){
                data->requestHandler = HttpsInterface::request_handlers[data->request->GetPath()]->CreateRequestHandler();
            }
            resp = data->requestHandler->HandleRequest(data->request);
        }
        enum MHD_Result ret;
        struct MHD_Response* response = resp.GetResponse();
        ret = MHD_queue_response(connection,resp.GetStatus(),response);
        MHD_destroy_response(response);
        delete data->request;
        data->request = nullptr;
        *con_cls = (void*)data;
        return ret;
    }
    data->request->AddDataToBody(upload_data,*upload_data_size);
    *upload_data_size = 0;
    *con_cls = (void*)data;
    return MHD_YES;
}

HttpsInterface::~HttpsInterface() {
    std::cout<<"STOPPING SERVER"<<std::endl;
    MHD_stop_daemon(daemon);
}

enum MHD_Result HttpsInterface::get_args(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
    if(kind==MHD_GET_ARGUMENT_KIND){
        auto* request = (Request*)cls;
        request->AddParameter(std::string(key),std::string(value));
    }
    return MHD_YES;
}

enum MHD_Result HttpsInterface::get_header(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
    if(kind==MHD_HEADER_KIND){
        auto* request = (Request*)cls;
        request->AddHeader(std::string(key),std::string(value));
    }
    return MHD_YES;
}

enum MHD_Result HttpsInterface::get_cookies(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
    if(kind==MHD_COOKIE_KIND){
        auto* request = (Request*)cls;
        request->AddCookie(std::string(key),std::string(value));
    }
    return MHD_YES;
}

void HttpsInterface::request_completed(void *cls, struct MHD_Connection *connection, void **con_cls,
                                       enum MHD_RequestTerminationCode toe) {
    auto* data = (local_data*)*con_cls;
    if(data== nullptr) return;
    delete data->request;
    delete data->requestHandler;
    free(data);
}
