#pragma once
class Directory : public File {
private:
	int fileCount;
	int directoryCount;
	Directory* directories;
	File* files;
public:
	Directory(std::filesystem::path&);
	Directory() {};	
	void init(const std::filesystem::path& path, Directory* parent_p);
	Directory* getDirectories();
	File* getFiles();
	int getFileCount();
	int getDirectoryCount();
};