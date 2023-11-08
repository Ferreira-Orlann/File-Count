#pragma once

#include <filesystem>
#include <vector>

#include "file.hpp"

class Directory : public File {
private:
	std::vector<Directory> directories;
	std::vector<File> files;
public:
	Directory(std::filesystem::path&);
	Directory() {};
	void init(const std::filesystem::path& path, Directory* parent_p);
	std::vector<Directory>* GetDirectories();
	std::vector<File>* GetFiles();
	int Size();


	static int dirnb;
};