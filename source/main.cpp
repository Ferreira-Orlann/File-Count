#include <stdio.h>
#include <ctime>

#include "main.hpp"
#include "app.hpp"

int main(int argc, char** argv, char** envp) {
    /* C Code for evironment variables
    for (char** env = envp; *env != 0; env++)
    {
        char* thisEnv = *env;
        printf("%s\n", thisEnv);
    }
    */


    {
        App app = App();

        while (app.ShouldRun()) {
            app.Render();
        }
    }
    

    fs::path path;

    if (argc == 2) {
        path = { argv[argc - 1] };
    }
    else {
        bool exit = false;
        while (!exit) {
            char sPath[256];
            std::cout << "Path: ";
            std::cin.getline(sPath, 256);
            path = { sPath };
            if (!fs::exists(path)) {
                std::cout << "Il n'y a rien a ce chemin ! " << path.string() << std::endl;
                continue;
            }
            if (!fs::is_directory(path)) {
                std::cout << "Ce chemin n'est pas le chemin d'un dossier !" << std::endl;
                continue;
            }
            exit = !exit;
        }
    }

    fprintf(stdout, "Time: %d\n", (int) std::time(nullptr));

    Directory root = Directory(path);

    fprintf(stdout, "Time: %d\n", (int) std::time(nullptr));

    std::cout << "Directory Count: " << root.getDirectoryCount() << "\nFile Count: " << root.getFileCount() << "\nSize: " <<  std::endl;

    Directory* dirArray = root.getDirectories();
    for (int i = 0; i < root.getDirectoryCount(); i++) {
        Directory dir = dirArray[i];
        fs::path path = dir.getPath();
        std::cout << "Directory Name: " << path.filename() << std::endl;
    }

    fs::path p2 = { "C:" };
    std::cout << "Name: " << p2.filename() << std::endl;

    fs::path p3 = fs::current_path();
    std::cout << "The current path " << p3 << " decomposes into:\n"
        << "root-path " << p3.root_path() << '\n'
        << "relative path " << p3.relative_path() << '\n';

    std::cout << "Sizeof: " << sizeof(root) << std::endl;


}

/*4
https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
*/