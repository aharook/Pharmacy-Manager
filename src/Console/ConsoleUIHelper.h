#ifndef CONSOLE_UI_HELPER_H
#define CONSOLE_UI_HELPER_H

#include <string>

class ConsoleUIHelper {
public:
    static int getUserChoice();
    static std::string getUserInput(const std::string& prompt);
    static void displayMessage(const std::string& msg);
    static void displayError(const std::string& error);
};

#endif
