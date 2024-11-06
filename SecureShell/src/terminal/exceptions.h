#include <iostream>

class StackEmptyException{

    private:
        std::string msg;

    public:

        StackEmptyException(const std::string& msg) : msg(msg) {}

        StackEmptyException() {
            msg = "Stack empty!";
        }

        std::string what() const{
            return this->msg;
        }
};

class GppCompilationError{
    private:
        std::string msg;

    public:

        GppCompilationError(const std::string& msg) : msg(msg) {}

        GppCompilationError() {
            msg = "File could not be compiled!";
        }

        std::string what() const{
            return this->msg;
        }
};