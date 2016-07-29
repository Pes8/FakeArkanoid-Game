#pragma once

class OSHelper {
public:
    virtual void sleep_uS(unsigned long _microSeconds) = 0;
    static OSHelper * getInstance() = delete; // Problem: Abstract Factory & Singleton ... Child must implement the singleton construction
protected:
    static OSHelper * instance;
};