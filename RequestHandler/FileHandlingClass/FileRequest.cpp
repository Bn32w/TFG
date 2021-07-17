//
// Created by bn32w on 17/7/21.
//


#include "FileRequest.h"

RequestHandler *FileRequest::CreateRequestHandler() {
    return new FileRequest();
}

Response FileRequest::OnGetRequest(Request *request) {
    std::string file_path = FileRequest::GetCleanPath(request->GetPath());
    if(!FileRequest::FileExists(file_path)){
        Response response = Response::GetNotFound();
        response.SetBodyFromString("<html><body>File: "+request->GetPath()+" was not found</body></html>");
        return response;
    }
    std::ifstream is(file_path);
    is.unsetf(std::ios_base::skipws);
    std::istream_iterator<uint8_t> start(is),end;
    std::vector<uint8_t> file_data(start,end);
    Response response = Response();
    response.SetBody(file_data);
    response.SetHeader(MHD_HTTP_HEADER_CONTENT_TYPE,FileRequest::GetAppropriateMime(file_path));
    return response;
}

Response FileRequest::OnPostRequest(Request *request) {
    return RequestHandler::OnPostRequest(request);
}

Response FileRequest::OnPutRequest(Request *request) {
    return RequestHandler::OnPutRequest(request);
}

Response FileRequest::OnDeleteRequest(Request *request) {
    return RequestHandler::OnDeleteRequest(request);
}

Response FileRequest::OnUnknownRequest(Request *request) {
    return RequestHandler::OnUnknownRequest(request);
}

std::string FileRequest::GetCleanPath(std::string unclean_path) {
    std::string to_remove = "/..";
    std::string data = std::move(unclean_path);
    size_t pos = std::string::npos;
    while((pos = data.find(to_remove))!=std::string::npos){
        data.erase(pos,to_remove.length());
    }
    return STATIC_FILE_PATH+data;
}

bool FileRequest::FileExists(const std::string& file) {
    struct stat buffer{};
    return (stat(file.c_str(),&buffer) == 0);
}

std::string FileRequest::GetAppropriateMime(const std::string& basicString) {
    std::string ext = basicString.substr(basicString.rfind('.')+1);
    if(mime_types.find(ext)!=mime_types.end()){
        return mime_types.at(ext);
    }
    return "application/octet-stream";
}
