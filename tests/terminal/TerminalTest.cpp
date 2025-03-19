#include "terminal/Terminal.h"
#include "../TestFramework.h"
#include <thread>
#include <chrono>

class TerminalTest {
public:
    static bool testTerminalRunningState() {
        Terminal terminal;

        ASSERT_TRUE(!terminal.isRunning());

        std::thread terminalThread([&terminal]() {
            terminal.start();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(terminal.isRunning());

        terminal.stop();

        if (terminalThread.joinable()) {
            terminalThread.join();
        }

        ASSERT_TRUE(!terminal.isRunning());

        return true;
    }
};
