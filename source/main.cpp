#include "main.hpp"
	
#include <windows.h>
#include <stdio.h>
#include <psapi.h>


void PrintMemoryInfo(DWORD processID)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Print the process identifier.

	printf("\nProcess ID: %u\n", processID);

	// Print information about the memory usage of the process.

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		printf("\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount);
		printf("\tPeakWorkingSetSize: 0x%08X\n",
			pmc.PeakWorkingSetSize);
		printf("\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize);
		printf("\tQuotaPeakPagedPoolUsage: 0x%08X\n",
			pmc.QuotaPeakPagedPoolUsage);
		printf("\tQuotaPagedPoolUsage: 0x%08X\n",
			pmc.QuotaPagedPoolUsage);
		printf("\tQuotaPeakNonPagedPoolUsage: 0x%08X\n",
			pmc.QuotaPeakNonPagedPoolUsage);
		printf("\tQuotaNonPagedPoolUsage: 0x%08X\n",
			pmc.QuotaNonPagedPoolUsage);
		printf("\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage);
		printf("\tPeakPagefileUsage: 0x%08X\n",
			pmc.PeakPagefileUsage);
	}

	CloseHandle(hProcess);
}


int main(int argc, char** argv, char** envp) {
	/* C Code for evironment variables
	for (char** env = envp; *env != 0; env++)
	{
		char* thisEnv = *env;
		printf("%s\n", thisEnv);
	}
	*/

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

	Directory root = Directory(path);

	std::cout << "Directory Count: " << root.getDirectoryCount() << "\nFile Count: " << root.getFileCount() << "\nSize: " << root.getLineCount() << std::endl;
	
	Directory* dirArray = root.getDirectories();	
	for (int i = 0; i < root.getDirectoryCount(); i++) {
		Directory dir = dirArray[i];
		fs::path path = dir.getPath();
		std::cout << "Directory Name: " << path.filename() << std::endl;
	}

	fs::path p2 = {"C:"};
	std::cout << "Name: " << p2.filename() << std::endl;

	fs::path p3 = fs::current_path();
	std::cout << "The current path " << p3 << " decomposes into:\n"
		<< "root-path " << p3.root_path() << '\n'
		<< "relative path " << p3.relative_path() << '\n';

	std::cout << "Sizeof: " << sizeof(root) << std::endl;

}
