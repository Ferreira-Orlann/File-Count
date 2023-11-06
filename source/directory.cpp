#include "main.hpp"

template <typename First, typename... T>
// Function that checks given variable
// first in the list t[]
bool is_in(First&& first, T&&... t)
{
	return ((first == t) || ...);
}

int Directory::getDirectoryCount() {
	return this->directoryCount;
}

int Directory::getFileCount() {
	return this->fileCount;
}

Directory* Directory::getDirectories() {
	return this->directories;
}

File* Directory::getFiles() {
	return this->files;
}

void Directory::init(const fs::path& path, Directory* parent_p) {
	File::init(path, parent_p);

	this->directoryCount = 0;
	this->fileCount = 0;
	for (auto const& dir_entry : fs::directory_iterator{ path, fs::directory_options::skip_permission_denied }) {
		try {
			if (dir_entry.is_directory())
				this->directoryCount++;
		}
		catch (...) { continue; }
		if (dir_entry.is_regular_file())
			this->fileCount++;
	}
	
	if (this->directoryCount == 0)
		this->directories = nullptr;
	else
		this->directories = new Directory[this->directoryCount];
	if (this->fileCount == 0)
		this->files = nullptr;
	else
		this->files = new File[this->fileCount];

	int fileP = 0;
	int dirP = 0;
	Directory* directories = this->directories;
	File* files = this->files;
	for (auto const& dir_entry : fs::directory_iterator{ path, fs::directory_options::skip_permission_denied }) {
		try {
			if (dir_entry.is_directory()) {
				directories[dirP].init(dir_entry.path(), this);
				dirP++;
			}
		}
		catch (...) { continue; }
		if (dir_entry.is_regular_file()) {
			fs::path path = dir_entry.path();
			files[fileP].init(path, this);
			fileP++;
		}
	}
}

Directory::Directory(fs::path& path) :File(path) {
	init(path, nullptr);
}