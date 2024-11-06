#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include "dynamicstack.h"

using namespace std;
namespace fs = filesystem;

class CommandHandler{
    public :
        virtual void execute_command(const fs::path& path1="", const fs::path& path2="") = 0;
        virtual ~CommandHandler() = default;

};

class GppCompileCommand : public CommandHandler{

};

class CompileCommand : public GppCompileCommand{
    void execute_command(const fs::path& path1, const fs::path& path2) override{



        string source = path1.string();
        string dest = path2.string();

        const string command = "g++ " + source + " -o " + dest; 
        int result = system(command.c_str());

        if(result==0){
            cout<<"\033[32m"<<path2<<"\033[0m"<<endl;
        }
        else{
            throw GppCompilationError();
        }
    }
}; 

class RunGppExecutableCommand : public GppCompileCommand{
    void execute_command(const fs::path& path1, const fs::path& path2) override{

        string command = path2.string();
        int result = system(command.c_str());

        if(result!=0){
            throw GppCompilationError();
        }
    }
};

class ExitCommand : public CommandHandler {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        exit(0);
    }
};

class HelpCommand : public CommandHandler {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        cout<<"Command <src> <dest>"<<endl;
    }
};

class FileSystemCommand : public CommandHandler{

};

class CopyCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        try{
            fs::copy(path1, path2, fs::copy_options::overwrite_existing);
        }
        catch(const fs::filesystem_error& e){
            cerr<<"Error copying the file : "<<e.what()<<endl;
        }
    }
};

class MoveCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        try{
            fs::rename(path1, path2);
        }
        catch(const fs::filesystem_error& e){
            cerr<<"Error moving file : "<<e.what()<<endl;
        }
    }
};

class RenameCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        try{
            fs::copy(path1, path2, fs::copy_options::overwrite_existing);
        }
        catch(const fs::filesystem_error& e){
            cerr<<"Error copying the file : "<<e.what()<<endl;
        }
    }
};

class CreateDirectoryCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2="") override{
        try{
            fs::create_directory(path1);
        }
        catch(const fs::filesystem_error& e){
            cerr<<"Error creating directory : "<<e.what()<<endl;
        }
    }
};

class CreateFileCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2="") override{
        if(!fs::exists(path1)){
            ofstream file(path1);
        }
        else{
            cerr<<"A file with this name already exists"<<endl;
        }
    }    
};

class DisplayPermissionCommand : public FileSystemCommand {

        void execute_command(const fs::path& path1, const fs::path& path2) {

            try{
                fs::perms permissions = fs::status(path1).permissions();

                fs::perms permission_collection[] = {
                    fs::perms::owner_read, fs::perms::owner_write, fs::perms::owner_exec,
                    fs::perms::group_read, fs::perms::group_write, fs::perms::group_exec,
                    fs::perms::others_read, fs::perms::others_write, fs::perms::others_exec
                };

                string permission_keywords = "rwxrwxrwx";

                for(int i=0; i<9; i++){
                    cout<<((permissions & permission_collection[i]) != fs::perms::none ? permission_keywords[i] : '-');
                }

                cout<<endl;

            }
            catch(const fs::filesystem_error& e){
                cout<<"Error displaying permission : "<<e.what()<<endl;
            }
        }        
};

class ChangeDirectoryCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2="") override{
        try{
            if(path1==".."){
                fs::current_path(fs::current_path().parent_path());
            }
            else{
                fs::current_path(path1);
            }
        }
        catch(const fs::filesystem_error& e){
            cerr<<e.what()<<endl;
        }
    }
};

class GetCurrentDirCommand : public FileSystemCommand {
    void execute_command(const fs::path& path1, const fs::path& path2) override{
        cout<<"Current working path : "<<"\033[32m"<<fs::current_path().string()<<"\033[0m"<<endl;
    }
};

class ListDirCommand : public FileSystemCommand{
    void execute_command(const fs::path& path1, const fs::path& path2) override {
        try{
            if(fs::exists(fs::current_path()) && fs::is_directory(fs::current_path())){
                for(const auto& entry : fs::directory_iterator(fs::current_path())){
                    cout<<entry.path().filename().string()<<endl;
                }
            }
        }
        catch(fs::filesystem_error& e){
            cout<<e.what()<<endl;
        }
    }
};

class CommandProcessor{

    DynamicStack<CommandHandler*> operations;
    DynamicStack<string> commands;

    string strip_quotes(const string& path){

        size_t first = path.find_first_not_of(' ');
        size_t last = path.find_last_not_of(' ');

        if(first==string::npos || last==string::npos){
            return "";
        }

        string trimmed_path = path.substr(first, (last-first+1));

        if(path.front()=='"' && path.back()=='"'){
            return trimmed_path.substr(1, path.size() - 2);
        }
        else{
            return trimmed_path;
        }
    }

    bool execute_registered_command(const string& cmd, const fs::path& path1, const fs::path& path2){

        DynamicStack<CommandHandler*> temp_operations;
        DynamicStack<string> temp_commands;
        bool found = false;

        while(!commands.is_empty()){

            CommandHandler* current_operation = operations.peek();
            string current_command = commands.peek();

            if(cmd == current_command){
                current_operation->execute_command(path1, path2);
                found = true;
            }

            temp_operations.push(current_operation);
            temp_commands.push(current_command);

            operations.pop();
            commands.pop();
        }

        while(!temp_commands.is_empty()){
            operations.push(temp_operations.peek());
            temp_operations.pop();
            commands.push(temp_commands.peek());
            temp_commands.pop();
        }

        return found;
        }

    public : 

        CommandProcessor(){
                register_command("copy", new CopyCommand());
                register_command("move", new MoveCommand());
                register_command("rename", new RenameCommand());
                register_command("dcreate", new CreateDirectoryCommand());
                register_command("fcreate", new CreateFileCommand());
                register_command("perm", new DisplayPermissionCommand());
                register_command("cd", new ChangeDirectoryCommand());
                register_command("curr", new GetCurrentDirCommand());
                register_command("compile", new CompileCommand());
                register_command("run", new RunGppExecutableCommand());
                register_command("ls", new ListDirCommand());
                register_command("--help", new HelpCommand());
                register_command("-h", new HelpCommand());
                register_command("exit", new ExitCommand());
        }

                
        void display_starting_message(){
            cout<<"Welcome to Custom Shell"<<endl;
            cout<<"For help, type --help or -h"<<endl;
            cout<<endl;
        }

        
        void register_command(const string& cmd, CommandHandler* fh){
            operations.push(fh);
            commands.push(cmd);
        }

        void interpret_input(string line, string command, string arg1, string arg2){

            istringstream iss(line);
            iss>>command;

            getline(iss, arg1, '"');
            getline(iss, arg1, '"');
            getline(iss, arg2, '"');
            getline(iss, arg2, '"');

            arg1 = strip_quotes(arg1);
            arg2 = strip_quotes(arg2);
                
            if(!execute_registered_command(command, arg1, arg2)){
                cout<<"Unknown command : "<<command<<endl;
            }

        }

};

class Shell{

    public:

        void run_shell(){

            CommandProcessor processor;
            processor.display_starting_message();
            string line, command, arg1, arg2;
            
            while(true){

                cout<<fs::current_path().string()<<">> ";
                getline(cin, line);

                if(line.empty()){
                    continue;
                }

                processor.interpret_input(line, command, arg1, arg2);
            }
        }
};

int main(){

    Shell cs;
    cs.run_shell();

    return 0;
}
