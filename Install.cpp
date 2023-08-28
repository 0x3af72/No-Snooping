#include <iostream>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

void ReplaceAll(std::string& src, std::string target, std::string rep) {
    size_t pos = 0;
    while ((pos = src.find(target, pos)) != std::string::npos) {
        src.replace(pos, target.length(), rep);
        pos += rep.length();
    }
}

std::vector<std::string> FILES_TO_WRITE = {
    "Monitor.exe", "Monitor.exe.manifest", "Monitor.bat",
    "No Snooping.exe", "No Snooping.exe.manifest",
    "libfreetype-6.dll", "libpng16-16.dll", "libtiff-5.dll", "zlib1.dll",
    "SDL2.dll", "SDL2_image.dll", "SDL2_ttf.dll",
    "lib/libjpeg-9.dll", "lib/libSDL2_image.a", "lib/libSDL2_image.la", "lib/libSDL2_test.a",
    "lib/libSDL2_test.la", "lib/libSDL2_ttf.a", "lib/libSDL2_ttf.dll.a", "lib/libSDL2_ttf.la",
    "lib/libSDL2.a", "lib/libSDL2.dll.a", "lib/libSDL2.la", "lib/libSDL2main.a", "lib/libSDL2main.la",
    "fonts/verdana.ttf"
};

int main() {

    std::string tmp;
    std::cout << "Install No Snooping?\nWARNING: If you already have No Snooping installed, your past data will be WIPED!\nPress ENTER to continue...";
    std::getline(std::cin, tmp);

    // Create No Snooping folder
    std::cout << "Creating C:/Program Files/No Snooping\n";
    system("taskkill /F /IM Monitor.exe");
    std::filesystem::remove_all("C:/Program Files/No Snooping");
    std::filesystem::create_directories("C:/Program Files/No Snooping/lib");
    std::filesystem::create_directories("C:/Program Files/No Snooping/fonts");

    // Reinstall latest version of program from main folder into folder in Program Files
    std::cout << "Writing files to C:/Program Files/No Snooping\n";
    for (std::string file: FILES_TO_WRITE) {
        std::filesystem::copy_file(file, "C:/Program Files/No Snooping/" + file);
    }

    /*
    Schedule Monitor.exe to run onlogon
    1. Create task
    2. Save task data to xml file
    3. Change AC start value in XML file to false
    4. Rewrite task, using the edited XML file
    */
    std::cout << "Scheduling to run on logon\n";
    system("schtasks /create /tn \"Monitor\" /tr \"\\\"C:\\Program Files\\No Snooping\\Monitor.bat\\\"\" /sc onlogon /rl highest /f");
    system("schtasks /query /tn \"Monitor\" /xml > \"C:\\Program Files\\No Snooping\\Monitor.xml\"");
    std::ifstream rXml("C:/Program Files/No Snooping/Monitor.xml");
    std::stringstream buf;
    buf << rXml.rdbuf();
    rXml.close();
    std::string sXml = buf.str();
    ReplaceAll(sXml, "<DisallowStartIfOnBatteries>true</DisallowStartIfOnBatteries>", "<DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>");
    ReplaceAll(sXml, "<StopIfGoingOnBatteries>true</StopIfGoingOnBatteries>", "<StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>");
    std::ofstream wXml("C:/Program Files/No Snooping/Monitor.xml");
    wXml << sXml;
    wXml.close();
    system("schtasks /create /tn \"Monitor\" /xml \"C:\\Program Files\\No Snooping\\Monitor.xml\" /f");
    ShellExecute(NULL, "open", "C:/Program Files/No Snooping/Monitor.exe", NULL, "C:/Program Files/No Snooping", SW_SHOW);

    // Start No Snooping.exe
    std::cout << "Done! You can delete this folder after this -> Press ENTER to launch the app...";
    std::getline(std::cin, tmp);
    ShellExecute(NULL, "open", "C:/Program Files/No Snooping/No Snooping.exe", NULL, "C:/Program Files/No Snooping", SW_SHOW);

    return 0;
}