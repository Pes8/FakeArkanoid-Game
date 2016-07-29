#include "WindowsHelper.h"

void WindowsHelper::sleep_uS(unsigned long _microSeconds) {
    Sleep(_microSeconds / 1000);
}

OSHelper * WindowsHelper::getInstance() {
    if (OSHelper::instance == nullptr) {
        OSHelper::instance = new WindowsHelper();
    }
    return OSHelper::instance;
}
