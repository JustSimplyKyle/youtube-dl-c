//general
#include <iostream>
#include <string>
//for exec command
#include <sstream>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdio>

using std::string;
using std::cout;
using std::cin;
using std::endl;

string exec(const char* cmd) {
    std::array<char, 128> buffer;
    string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int main(){

    std::ostringstream oss;
    //get web url from user
    cout << "Enter a web url: ";
    string url;
    cin >> url;
    oss << "yt-dlp ";
    oss << "-F ";
    oss << url;
    system(oss.str().c_str());
    //ask format from user
    cout << "Enter a format: \n";
    cout << "Ex. sb0+251+313\n";
    cout << "Ex. 251+313\n";
    cout << "Press enter for the best format\n";
    string format;
    cin.ignore();
    std::getline(cin, format);
    if(format.empty()){
        format = "sb0+bestvideo+bestaudio";
    }
    oss.str("");
    oss.clear();
    oss << "yt-dlp --list-subs " << url;
    system(oss.str().c_str());
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
        oss.str("");
        oss.clear();
        oss<<"yt-dlp -f " <<format <<" --merge-output-format mkv --sub-langs "<<sub<<" --embed-subs --embed-thumbnail -o %\\(title\\)s " << url;
        system(oss.str().c_str());
    }
    else{
        oss.str("");
        oss.clear();
        oss<<"yt-dlp -f " <<format <<" --merge-output-format mkv --embed-thumbnail -o %\\(title\\)s " << url;
        system(oss.str().c_str());
    }
    oss.str("");
    oss.clear();
    oss << "yt-dlp -o %\\(title\\)s --get-filename " << url;
    string filename;
    filename = exec(oss.str().c_str());
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
        oss.str("");
        oss.clear();
        oss <<"mpv --profile=svp " <<"\""<< path << filename << ".mkv"<<"\"";
        system(oss.str().c_str());
    }
    oss.clear();

    return 0;
}