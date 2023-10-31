#include "main.hpp"

std::unordered_map<fs::path, fs::path*> filenames;

void File::init(const fs::path& path, Directory* parent_p) {
	this->parent_p = parent_p;
	if (parent_p == nullptr) {
		this->filename = path;
	}
	else {
		this->filename = path.filename();
		if (this->filename.empty()) {
			this->filename = path.root_name();
		}
	}
	this->lineCount = 0;
	if (!fs::is_regular_file(path)) {
		return;
	}
	/*
	std::ifstream myfile;
	myfile.open(path.string());
	std::string sa;
	while (std::getline(myfile, sa))
	{
		this->lineCount++;
	}
	myfile.close();
	*/
}

void File::setLineCount(int count) {
	this->lineCount = count;
}

fs::path& File::getFilename() {
	return this->filename;
}

File::File() {
	this->parent_p = nullptr;
}

File::File(fs::path& path) {
	init(path, nullptr);
}

File::File(fs::path& path, Directory* parent_p) {
	init(path, parent_p);
}

int File::getLineCount() {
	return this->lineCount;
}

Directory* File::getParent() {
	return this->parent_p;
}

fs::path File::getPath() {
	Directory* parent_p = this->getParent();
	if (parent_p == nullptr) {
		return this->getFilename();
	}

	fs::path path = { this->getFilename() };
	while (parent_p != nullptr)
	{
		path = parent_p->getFilename() / path;
		parent_p = parent_p->getParent();
	}
	return path;
}