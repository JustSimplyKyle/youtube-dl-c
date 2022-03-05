//general
#include <iostream>
#include <string>
//for exec command
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdio>

using std::string;
using std::cout;
using std::cin;
using std::endl;

string exec(const char* cmd, const string& error_message="Error!") {
    std::array<char, 128> buffer;
    std::string result;

    auto pipe = popen(cmd, "r"); // get rid of shared_ptr

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = pclose(pipe);

    if (rc != EXIT_SUCCESS) { // == 0
        throw std::runtime_error(error_message);
    }
    return result;
}
void exec_print(const char* cmd, const string& error_message="Error!") {
    std::array<char, 64> buffer;
    string result;

    auto pipe = popen(cmd, "r"); // get rid of shared_ptr

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 64, pipe) != nullptr) {
            result += buffer.data();
            cout << result;
        }
    }

    auto rc = pclose(pipe);

    if (rc != EXIT_SUCCESS) { // == 0
        throw std::runtime_error(error_message);
    }
}

int main(){

    string cmd;
    //get web url from user
    cout << "Enter a web url: ";
    string url;
    cin >> url;
    cmd = "yt-dlp -F " + url;
    cout.flush();
    exec_print(cmd.c_str());
    //ask format from user
    cout << "Enter a format: \n"
         << "Ex. sb0+251+313\n"
         << "Ex. 251+313\n"
         << "Press enter for the best format\n";
    string format;
    cin.ignore();
    std::getline(cin, format);
    if(format.empty()){
        format = "sb0+bestvideo+bestaudio";
    }
    cmd = "yt-dlp --list-subs " + url;
    exec_print(cmd.c_str());
    cout << "Do you want to download subtitles? (y/N) ";
    string subs;
    std::getline(cin, subs);
    if(subs.empty()){
        subs = "N";
    }
    if(subs == "y"||subs == "Y"){
        cout << "Enter a subtitle: \n";
        cout << "Ex. en-CA,ja\n";
        string sub;
        std::getline(cin, sub);
        cmd = "yt-dlp -f " + format + " --merge-output-format mkv --sub-langs " +sub + " --embed-subs --embed-thumbnail -o %\\(title\\)s " + url;
        exec_print(cmd.c_str());
    }
    else{
        cmd = "yt-dlp -f " + format + " --merge-output-format mkv --embed-thumbnail -o %\\(title\\)s " + url;
        exec_print(cmd.c_str());
    }
    cmd = "yt-dlp -o %\\(title\\)s --get-filename " + url;
    string filename;
    filename = exec(cmd.c_str());
    string path;
    path = exec("pwd");
    path[path.size()-1] = '/';
    filename.erase(filename.size()-1);
    cout << "The file path is: \n" << path << filename << ".mkv\n";
    cout << "Do you want to open it? (Y/n)(using mpv) ";
    string open;
    std::getline(cin, open);
    if(open.empty()){
        open = "Y";
    }
    if(open == "Y"||open == "y"){
        cmd = "mpv \"" + path + filename + ".mkv\"";
        exec_print(cmd.c_str());
    }

    return 0;
}