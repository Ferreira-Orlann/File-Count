#include <stdio.h>
#include <ctime>

#include "main.hpp"
#include "app.hpp"

#include "directory.hpp"
#include "file.hpp"

int main(int argc, char** argv, char** envp) {
    /* C Code for evironment variables
    for (char** env = envp; *env != 0; env++)
    {
        char* thisEnv = *env;
        printf("%s\n", thisEnv);
    }
    */

    fprintf(stdout, "Dir: %lu\n", sizeof(Directory));
    fprintf(stdout, "File: %lu", sizeof(File));

    {
        App app = App();

        while (app.ShouldRun()) {
            app.Render();
        }
    }

    Directory dir = Directory();
    fprintf(stdout, "Dir: %d\n", Directory::dirnb);
    fprintf(stdout, "File: %d", filenb);
}

/*4
https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
*/