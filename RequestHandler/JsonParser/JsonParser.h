//
// Created by bn32w on 18/7/21.
//

#ifndef HTTP_LIBRARY_JSONPARSER_H
#define HTTP_LIBRARY_JSONPARSER_H

#include <string>
#include <vector>
#include <map>
#include <queue>

enum type{
    STRING,
    NUMBER,
    BOOLEAN,
    NILL,
    ARRAY,
    OBJECT,
    ERROR
};

struct json_element;

struct json_data{
    std::string str;
    float number;
    bool boolean;
    void* null;
    std::vector<json_element> array;
    std::map<std::string,json_element> object;
};

struct json_element{
    enum type element_type;
    json_data data;
};

class JsonParser {
private:
    static std::map<char,char> escape_map;
    static std::vector<char> whitespace;
    std::queue<uint8_t> data;

    json_element GetJsonElement();
    json_element GetString();
    json_element GetNumber();
    json_element GetBool();
    json_element GetNull();
    json_element GetArray();
    json_element GetObject();


public:
    explicit JsonParser(const std::vector<uint8_t>& data);
};


#endif //HTTP_LIBRARY_JSONPARSER_H
