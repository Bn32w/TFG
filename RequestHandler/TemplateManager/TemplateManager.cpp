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
    std::vector<CodeBlock> code_blocks = this->SplitIntoCodeBlocks(processed_data);
    while(!this->IsCodeBlockFinished(code_blocks)){
        std::vector<CodeBlock> operated_code_blocks;
        for(auto it = code_blocks.begin(); it!=code_blocks.end();it++){
            if(it->type==IF_STATEMENT){
                if(it->code.size()==1 && !it->condition){
                }else{
                    std::vector<CodeBlock> aux_code_blocks = this->SplitIntoCodeBlocks(it->condition?it->code[0]:it->code[1]);
                    operated_code_blocks.insert(operated_code_blocks.end(),aux_code_blocks.begin(),aux_code_blocks.end());
                }
            }else{
                operated_code_blocks.push_back(*it);
            }
        }
        code_blocks = operated_code_blocks;
    }
    processed_data = this->MergeCodeBlocks(code_blocks);
    processed_data = this->CleanEscapedStrings(processed_data);
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

std::vector<CodeBlock> TemplateManager::SplitIntoCodeBlocks(std::vector<uint8_t> data) {
    std::vector<CodeBlock> code_blocks;
    CodeBlock aux = {.condition_string="",.condition=true,.code={{}},.type=NORMAL_BLOCK};
    bool hasCurlyBracket = false;
    while(!data.empty()){
        uint8_t value = data[0];
        data.erase(data.begin());
        if(hasCurlyBracket){
            hasCurlyBracket = false;
            if(value=='&'){
                code_blocks.push_back(aux);
                aux = ReadIfStatement(data);
                if(aux.type!=ERROR)
                    code_blocks.push_back(aux);
                aux = {.condition_string="",.condition=true,.code={{}},.type=NORMAL_BLOCK};
                continue;
            }
            else{
                aux.code[0].push_back('{');
                aux.code[0].push_back(value);
            }
        }else{
            if(value=='{'){
                hasCurlyBracket = true;
                continue;
            }
            aux.code[0].push_back(value);
        }


    }
    if(!aux.code[0].empty()){
        code_blocks.push_back(aux);
    }
    return code_blocks;
}

std::vector<uint8_t> TemplateManager::CleanEscapedStrings(std::vector<uint8_t> original_data) {
    std::vector<uint8_t> processed_data;
    bool hasCurlyBracket = false;
    while(!original_data.empty()){
        uint8_t value = original_data[0];
        original_data.erase(original_data.begin());
        if(hasCurlyBracket){
            hasCurlyBracket = false;
            if(value=='{'){
                processed_data.push_back('{');
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

CodeBlock TemplateManager::ReadIfStatement(std::vector<uint8_t>& data) {
    CodeBlock code_block = {.type = IF_STATEMENT};
    code_block.condition_string = this->GetConditionString(data);
    if(code_block.condition_string.empty() || data[0]!='&'){
        code_block.type=ERROR;
        return code_block;
    }
    code_block.code.emplace_back();
    data.erase(data.begin());
    data.erase(data.begin());
    IfFillingState ifState = FillVectorForIf(code_block.code[0],data);
    if(ifState==INVALID_IF_SYNTAX){
        code_block.type=ERROR;
        return code_block;
    }
    if(ifState==ELSE){
        for(int i=0;i<5;i++){
            data.erase(data.begin());
        }
        if(!(data[0]==' ' && data[1]=='&' && data[2]=='}')){
            code_block.type==ERROR;
            return code_block;
        }
        data.erase(data.begin());
        data.erase(data.begin());
        data.erase(data.begin());
        code_block.code.emplace_back();
        IfFillingState elseState = FillVectorForIf(code_block.code[1],data);
        if(elseState==ELSE || elseState == INVALID_IF_SYNTAX){
            code_block.type=ERROR;
            return code_block;
        }
    }
    code_block.condition = this->ParseCondition(code_block.condition_string);
    for(int i=0;i<6;i++){
        data.erase(data.begin());
    }
    if(!(data[0]==' ' && data[1]=='&' && data[2]=='}')){
        code_block.type==ERROR;
        return code_block;
    }
    data.erase(data.begin());
    data.erase(data.begin());
    data.erase(data.begin());
    return code_block;
}

std::vector<uint8_t> TemplateManager::MergeCodeBlocks(std::vector<CodeBlock> code_block) {
    std::vector<uint8_t> data;
    for(auto code : code_block){
        data.insert(data.end(),code.code[0].begin(),code.code[0].end());
    }
    return data;
}

std::string TemplateManager::GetConditionString(std::vector<uint8_t>& data) {
    std::string condition_string;
    uint8_t value = data[0];
    data.erase(data.begin());
    if(value!=' '){
        return "";
    }
    value = data[0];
    data.erase(data.begin());
    bool ok = false;
    if(value=='I' && data[0]!='F'){
        return "";
    }else if(value=='I'){
        ok = true;
        data.erase(data.begin());
    }

    if(!ok) return "";
    if(data[0]!=' '){
        return "";
    }
    data.erase(data.begin());
    int number_of_valid_spaces = 0;
    while(number_of_valid_spaces!=3){
        value = data[0];
        data.erase(data.begin());
        if(value==' '){
            number_of_valid_spaces++;
        }
        if(number_of_valid_spaces==1 && (condition_string=="true" || condition_string=="false")){
            return condition_string;
        }
        condition_string += (char) value;
    }
    if(condition_string.empty()){
        return "";
    }
    condition_string.erase(std::prev(condition_string.end()));
    if(data[1]!='}'){
        return "";
    }
    return condition_string;
}

IfFillingState TemplateManager::FillVectorForIf(std::vector<uint8_t> &if_data, std::vector<uint8_t>& data) {
    int depth = 1;
    uint8_t value;
    bool hasBracket = false;
    while(depth>0){
        if(data.empty()){
            return INVALID_IF_SYNTAX;
        }
        value = data[0];
        data.erase(data.begin());
        if(hasBracket){
            hasBracket = false;
            if(value=='&'){
                if(data[0]==' '){
                    if(data[1]=='I' && data[2]=='F'){
                        depth++;
                        if_data.push_back('{');
                        if_data.push_back('&');
                        continue;
                    }else if(data[1]=='E'){
                        if(depth==1 && data[2]=='L' && data[3]=='S' && data[4]=='E'){
                            return ELSE;
                        }else if (data[2]=='L' && data[3]=='S' && data[4]=='E'){
                            if_data.push_back('{');
                            if_data.push_back('&');
                            continue;
                        }else if(depth==1 && data[2]=='N' && data[3]=='D' && data[4]=='I' && data[5]=='F'){
                            depth--;
                            break;
                        }
                        else if(data[2]=='N' && data[3]=='D' && data[4]=='I' && data[5]=='F'){
                            depth--;
                            if_data.push_back('{');
                            if_data.push_back('&');
                        }
                        else{
                            return INVALID_IF_SYNTAX;
                        }
                    }else{
                        return INVALID_IF_SYNTAX;
                    }
                } else{
                    return INVALID_IF_SYNTAX;
                }
            } else{
                if_data.push_back('{');
                if_data.push_back(value);
            }
        }else{
            if(value=='{'){
                hasBracket=true;
                continue;
            }
            if_data.push_back(value);
        }
    }
    return ENDIF;
}

bool TemplateManager::ParseCondition(std::string condition) {
    if(condition=="true") return true;
    if(condition=="false") return false;
    size_t pos = 0;
    pos = condition.find(' ');
    std::string first_value = condition.substr(0,pos);
    condition.erase(0,pos+1);
    pos = condition.find(' ');
    std::string op = condition.substr(0,pos);
    condition.erase(0,pos+1);
    float f1,f2;
    try{
        f1 = std::stof(first_value);
        f2 = std::stof(condition);
    }catch (std::invalid_argument& e ){
        return false;
    } catch (std::out_of_range& e1) {
        return false;
    }
    if(op==">"){
        return f1>f2;
    }
    else if(op==">="){
        return f1>=f2;
    }
    else if(op=="<"){
        return f1<f2;
    }
    else if(op=="<="){
        return f1<=f2;
    }
    else if(op=="=="){
        return f1==f2;
    }else if(op=="!="){
        return f1!=f2;
    }
    return false;
}

bool TemplateManager::IsCodeBlockFinished(std::vector<CodeBlock> code_blocks) {

    for(const auto& code : code_blocks){
        if(code.type!=NORMAL_BLOCK){
            return false;
        }
    }

    return true;
}
