#pragma once

#include <stdint.h>
#include <functional>
#include <vector>

#ifdef WINDOWS
#include "WindowsTypes.h"
#elif LINUX
#include "LinuxTypes.h"
#elif MAC
#include "MacTypes.h"
#endif


typedef void(*FP)();
typedef std::function<void()> callback;


enum Button {
    QUIT_BTN,
    PAUSE_BTN,
    ENTER_BTN,
    LEFT_BTN,
    RIGHT_BTN,
    UP_BTN,
    DOWN_BTN,
    NOTHING
};


struct Event {
    std::vector<callback> subscriptions;
    unsigned int subscribe(callback cb) {
        subscriptions.push_back(cb);
        return subscriptions.size();
    }
    void unsubcribe(unsigned int pos) {
        subscriptions.erase(subscriptions.begin() + pos);
    }
    void fire() { for (callback& cb : subscriptions) cb(); }
};

typedef uint8_t ButtonsStatus;