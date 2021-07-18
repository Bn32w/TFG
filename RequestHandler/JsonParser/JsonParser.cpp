//
// Created by bn32w on 18/7/21.
//

#include <algorithm>
#include "JsonParser.h"

std::map<char,char> JsonParser::escape_map = {{'b','\b'},{'f','\f'},{'n','\n'},{'r','\r'},
                                                    {'t','\t'},{'"','"'},{'\\','\\'}};
std::vector<char> JsonParser::whitespace = {' ','\r','\n','\t'};

JsonParser::JsonParser(const std::vector<uint8_t>& vector_data) {
    bool inside_double_quotes = false;
    bool escaped = false;
    for(const unsigned char & i : vector_data){
        if(i==whitespace[0]){
            if(!inside_double_quotes && !escaped){
                continue;
            }
        }
        if(std::find(whitespace.begin()+1,whitespace.end(),i)!=whitespace.end() && !escaped){
            continue;
        }
        this->data.push(i);
        if(i=='\\' && !escaped){
            escaped = true;
            continue;
        }
        if(!escaped && !inside_double_quotes && i=='"'){
            inside_double_quotes = true;
            continue;
        }
        if(!escaped && inside_double_quotes && i=='"'){
            inside_double_quotes = false;
            continue;
        }

        if(escaped) escaped = false;
    }
}

json_element JsonParser::GetJsonElement() {
    char value = (char)this->data.front();

    if(value=='"'){
        this->data.pop();
        return this->GetString();
    }
    if(value>='0'&&value<='9'){
        return this->GetNumber();
    }
    if(value=='t' || value=='f'){
        return this->GetBool();
    }
    if(value=='n'){
        return this->GetNull();
    }
    if(value=='['){
        this->data.pop();
        return this->GetArray();
    }
    if(value=='{'){
        this->data.pop();
        return this->GetObject();
    }
    return {.element_type=ERROR,.data={.null=nullptr}};
}

json_element JsonParser::GetString() {
    json_element jse{.element_type = STRING};
    jse.data.str = "";
    bool special_character = false;
    char read_data =(char)this->data.front();
    this->data.pop();
    while(read_data!='"'){
        if(special_character){
            special_character = false;
            if(JsonParser::escape_map.find(read_data)!=JsonParser::escape_map.end()){
                jse.data.str += JsonParser::escape_map[read_data];
            } else{
                return {.element_type=ERROR,.data={.null=nullptr}};
            }
        }else{
            if(read_data=='\\'){
                special_character = true;
            }else{
                jse.data.str += read_data;
            }
        }
        if(this->data.empty()){
            return {.element_type=ERROR,.data={.null=nullptr}};
        }
        read_data = (char)this->data.front();
        this->data.pop();
    }
    if(!this->data.empty()&&(char)this->data.front()==','){
        this->data.pop();
    }
    return jse;
}

json_element JsonParser::GetNumber() {
    std::string temp_storage;
    json_element jse = {.element_type=NUMBER};
    bool allow_dot = false;
    bool first_number = true;
    char read_data = (char)this->data.front();
    this->data.pop();
    if(read_data=='-'){
        temp_storage+=read_data;
        read_data = (char)this->data.front();
        this->data.pop();
    }
    while (read_data!=',' || read_data!= '}'){
        if(allow_dot && read_data=='.'){
            allow_dot = false;
            temp_storage += read_data;
        }
        else if(read_data>='0'&&read_data<='9'){
            if(first_number){
                first_number = false;
                allow_dot = true;
            }
            temp_storage +=read_data;
        }else{
            return {.element_type=ERROR,.data={.null=nullptr}};
        }
        if(this->data.empty()){
            return {.element_type=ERROR,.data={.null=nullptr}};
        }
        read_data = (char)this->data.front();
        if(read_data!='}'){
            this->data.pop();
        }
    }
    if(!this->data.empty() && this->data.front()==','){
        this->data.pop();
    }
    jse.data.number = std::stof(temp_storage);
    return jse;
}

json_element JsonParser::GetBool() {
    return json_element();
}

json_element JsonParser::GetNull() {
    return json_element();
}

json_element JsonParser::GetArray() {
    return json_element();
}

json_element JsonParser::GetObject() {
    return json_element();
}

