#pragma once

class OSHelper {
public:
    virtual void sleep_uS(unsigned long _microSeconds) = 0;
    static OSHelper * getInstance() = delete;
protected:
    static OSHelper * instance;
};