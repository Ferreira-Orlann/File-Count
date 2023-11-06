#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class Directory;

class File {
private:
	std::filesystem::path filename;
	Directory* parent_p;
public:
	std::filesystem::path& getFilename();
	File();
	File(std::filesystem::path& path);
	File(std::filesystem::path& path, Directory* parent_p);
	void init(const std::filesystem::path&, Directory* parent_p);
	std::filesystem::path getPath();
	Directory* getParent();
};