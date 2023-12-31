#include "file.hpp"
#include "directory.hpp"

static int filenb = 0;

void File::init(const fs::path& path, Directory* parent_p) {
	filenb++;
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
	fprintf(stdout, "%s\n", this->getPath().string().c_str());
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

Directory* File::getParent() {
	return this->parent_p;
}

bool File::IsChildOf(Directory* directory)
{
	Directory* parent = this->getParent();
	while (parent != nullptr) 
	{
		if (parent == directory) 
		{
			return true;
		}
		parent = parent->getParent();
	}
	return false;
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