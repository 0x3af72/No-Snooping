#include <iostream>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>

std::vector<std::string> FILES_TO_WRITE = {
    "Monitor.exe", "Monitor.exe.manifest",
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

    // Start Monitor.exe
    std::cout << "Starting Monitor.exe\n";
    ShellExecute(NULL, "open", "C:/Program Files/No Snooping/Monitor.exe", NULL, "C:/Program Files/No Snooping", SW_HIDE);

    // Schedule to run on logon
    system("schtasks /create /tn \"Monitor\" /tr \"\\\"C:\\Program Files\\No Snooping\\Monitor.exe\\\"\" /sc onlogon /rl highest /f");

    // Start No Snooping.exe
    std::cout << "Done! You can delete this folder after this -> Press ENTER to launch the app...";
    std::getline(std::cin, tmp);
    ShellExecute(NULL, "open", "C:/Program Files/No Snooping/No Snooping.exe", NULL, "C:/Program Files/No Snooping", SW_SHOWDEFAULT);

    return 0;
}