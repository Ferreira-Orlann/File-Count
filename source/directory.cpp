#include "directory.hpp"


int Directory::dirnb = 0;

namespace fs = std::filesystem;

template <typename First, typename... T>
// Function that checks given variable
// first in the list t[]
bool is_in(First&& first, T&&... t)
{
	return ((first == t) || ...);
}

static int dirnb = 0;

void Directory::init(const fs::path& path, Directory* parent_p) {
	File::init(path, parent_p);
	dirnb++;

	int directoryCount = 0;
	int fileCount = 0;
	for (auto const& dir_entry : fs::directory_iterator{ path, fs::directory_options::skip_permission_denied }) {
		try {
			if (dir_entry.is_directory())
				directoryCount++;
		}
		catch (...) { continue; }
		if (dir_entry.is_regular_file())
			fileCount++;
	}

	this->directories = std::vector<Directory>(directoryCount);
	this->files = std::vector<File>(fileCount);

	int fileP = 0;
	int dirP = 0;
	for (auto const& dir_entry : fs::directory_iterator{ path, fs::directory_options::skip_permission_denied }) {
		try {
			if (dir_entry.is_directory()) {
				this->directories[dirP].init(dir_entry.path(), this);
				dirP++;
			}
		}
		catch (...) { continue; }
		if (dir_entry.is_regular_file()) {
			fs::path path = dir_entry.path();
			this->files[fileP].init(path, this);
			fileP++;
		}
	}
}

std::vector<Directory>* Directory::GetDirectories()
{
	return &this->directories;
}

std::vector<File>* Directory::GetFiles()
{
	return &this->files;
}

int Directory::Size()
{
	return this->directories.size() + this->files.size();
}

Directory::Directory(fs::path& path) :File(path) {
	init(path, nullptr);
}