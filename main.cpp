#include <iostream>
#include "Configuration/Configuration.h"
#include "HttpInterface/HttpsInterface.h"
#include "RequestHandler/TemplateManager/TemplateManager.h"

int main() {
    TemplateManager::GetInstance(TEMPLATE_FILE_PATH);
    HttpsInterface interface = HttpsInterface(PORT,CONNECTION_FLAG,handler_configuration);
    (void) getchar();
    return 0;
}
