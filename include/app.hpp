#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "file.hpp"
#include "directory.hpp"

class App {
private:
	GLFWwindow* window;
	Directory* selectedDirectory = nullptr;
	File* selectedFile = nullptr;
public:
	App();
	~App();
	bool ShouldRun();
	File* GetSelectedFile();
	void SetSelectedFile(File& file);
	Directory* GetSelectedDirectory();
	void SetSelectedDirectory(Directory& diretory);
	void RenderInitialization();
	void Render();
	void RenderFinalization();
};