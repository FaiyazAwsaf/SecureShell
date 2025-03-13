#include <iostream>
#include <memory>
#include "terminal/Terminal.h"

int main() {
    try {
        auto terminal = std::make_unique<Terminal>();
        terminal->start();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
