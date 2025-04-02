#pragma once
#include "modules/other/consts.h"
#include <vector>
#include <string>

extern std::vector<std::string> packetLogs;  // Declare packetLogs as extern

class GUI {
public:
    static void displaySendFilterMenu();
    static void displayBlockingMenu();
    static void displayRecvFilterMenu();
    static void displaySendMenu();
    static void displayRecvMenu();
    static void runModule();
};
