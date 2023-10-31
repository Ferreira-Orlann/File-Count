#pragma once
class Directory;

class File {
private:
	std::filesystem::path filename;
	int lineCount;
	Directory* parent_p;
public:
	int getLineCount();
	std::filesystem::path& getFilename();
	File();
	File(std::filesystem::path& path);
	File(std::filesystem::path& path, Directory* parent_p);
	void init(const std::filesystem::path&, Directory* parent_p);
	std::filesystem::path getPath();
	Directory* getParent();
protected:
	void setLineCount(int count);
};