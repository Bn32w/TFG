//
// Created by bn32w on 8/8/21.
//

#include "TemplateManager.h"



TemplateManager* TemplateManager::instance = nullptr;

TemplateManager::TemplateManager(std::string BaseTemplateURL) {
    this->BaseTemplateURL = std::move(BaseTemplateURL);
}

std::vector<uint8_t> TemplateManager::GetDataFromTemplate(const std::string& template_name, std::map<std::string, std::string> variables) {
    std::vector<uint8_t> template_data = this->LoadDataIntoMemory(template_name);
    return this->GetProcessedData(template_data,variables);
}

TemplateManager *TemplateManager::GetInstance(std::string BaseTemplateURL) {
    if(TemplateManager::instance==nullptr){
        TemplateManager::instance = new TemplateManager(std::move(BaseTemplateURL));
    }
    return TemplateManager::instance;
}

std::vector<uint8_t> TemplateManager::LoadDataIntoMemory(const std::string& template_name) {
    std::string path = this->BaseTemplateURL+"/"+template_name+".bnt";
    if(!TemplateManager::FileExists(path)){
        return{};
    }
    std::ifstream is(path);
    is.unsetf(std::ios_base::skipws);
    std::istream_iterator<uint8_t> start(is),end;
    std::vector<uint8_t> file_data(start,end);
    return file_data;
}


void TemplateManager::DoStringReplacement(std::vector<uint8_t> &original_data, std::vector<uint8_t>& processed_data, std::map<std::string,std::string> variables) {
    if(original_data[0]!=' '){
        processed_data.clear();
        return;
    }
    original_data.erase(original_data.begin());
    std::string var_name;
    uint8_t value = original_data[0];
    while (value!=' '){
        var_name+=(char)value;
        original_data.erase(original_data.begin());
        if(original_data.empty()){
            processed_data.clear();
            return;
        }
        value=original_data[0];
    }
    original_data.erase(original_data.begin());
    if(variables.find(var_name)==variables.end()){
        processed_data.clear();
        return;
    }
    for(char chara : variables[var_name]){
        processed_data.push_back(chara);
    }
    if(original_data[0]!='%'){
        processed_data.clear();
        return;
    }
    original_data.erase(original_data.begin());
    if(original_data[0]!='}'){
        processed_data.clear();
        return;
    }
    original_data.erase(original_data.begin());

}

std::vector<uint8_t> TemplateManager::GetProcessedData(std::vector<uint8_t> original_data, std::map<std::string, std::string> variables) {
    std::vector<uint8_t> processed_data = this->TreatStrings(original_data,variables);

    return processed_data;
}

bool TemplateManager::FileExists(const std::string& file_path) {
    struct stat buffer{};
    return (stat(file_path.c_str(),&buffer) == 0);
}

std::vector<uint8_t> TemplateManager::TreatStrings(std::vector<uint8_t> original_data, std::map<std::string, std::string> variables) {
    std::vector<uint8_t> processed_data;
    bool hasCurlyBracket = false;
    while(!original_data.empty()){
        uint8_t value = original_data[0];
        original_data.erase(original_data.begin());
        if(hasCurlyBracket){
            hasCurlyBracket = false;
            if(value=='%'){
                this->DoStringReplacement(original_data,processed_data,variables);
            }
            else if(value=='{'){
                processed_data.push_back('{');
            }
            else{
                processed_data.push_back('{');
                processed_data.push_back(value);
            }
            if(processed_data.empty()){
                return {};
            }
        }else{
            if(value=='{'){
                hasCurlyBracket = true;
                continue;
            }
            processed_data.push_back(value);
        }
    }
    return processed_data;
}



