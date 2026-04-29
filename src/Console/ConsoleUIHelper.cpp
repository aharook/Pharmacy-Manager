#include "ConsoleUIHelper.h"
#include <iostream>

int ConsoleUIHelper::getUserChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    } catch (...) {
        return -1;
    }
}

std::string ConsoleUIHelper::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void ConsoleUIHelper::displayMessage(const std::string& msg) {
    std::cout << "\n✓ " << msg << std::endl;
}

void ConsoleUIHelper::displayError(const std::string& error) {
    std::cout << "\n✗ Error: " << error << std::endl;
}
