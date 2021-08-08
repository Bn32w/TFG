//
// Created by bn32w on 8/8/21.
//

#ifndef HTTP_LIBRARY_TEMPLATEMANAGER_H
#define HTTP_LIBRARY_TEMPLATEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <fstream>
#include <sys/stat.h>



class TemplateManager {
public:
    static TemplateManager* GetInstance(std::string BaseTemplateURL = "");
    std::vector<uint8_t> GetDataFromTemplate(const std::string& template_name, std::map<std::string,std::string> variables);

private:
    explicit TemplateManager(std::string BaseTemplateURL);

    bool FileExists(const std::string& file_path);

    std::vector<uint8_t> LoadDataIntoMemory(const std::string& template_name);
    std::vector<uint8_t> GetProcessedData(std::vector<uint8_t> original_data, std::map<std::string,std::string> variables);
    std::vector<uint8_t> TreatStrings(std::vector<uint8_t> original_data, std::map<std::string,std::string> variables);
    void DoStringReplacement(std::vector<uint8_t>& original_data, std::vector<uint8_t>& processed_data, std::map<std::string,std::string> variables);

    static TemplateManager* instance;
    std::string BaseTemplateURL;
};


#endif //HTTP_LIBRARY_TEMPLATEMANAGER_H
