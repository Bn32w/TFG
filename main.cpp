#include <iostream>
#include "Configuration/Configuration.h"
#include "HttpInterface/HttpsInterface.h"

int main() {
    HttpsInterface interface = HttpsInterface(PORT,CONNECTION_FLAG,handler_configuration);
    (void) getchar();
    return 0;
}
