#include <stdio.h>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "app.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "directory.hpp"

#include "imgui_internal.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

App::App() 
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		throw std::exception("GLFW Error");

    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Macg
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    this->window = glfwCreateWindow(mode->width/2, mode->height/2, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (this->window == nullptr)
        throw std::exception("WindowCreation Error");
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

App::~App() 
{
    fprintf(stdout, "~App\n");
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
}

// ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
bool shoulDemo = true;

void App::RenderInitialization() 
{
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // If so, get the main viewport:
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
    // completely covers the main viewport, giving it a "full-screen" feel.
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    // Set the parent window's styles to match that of the main viewport:
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
    ImGui::PopStyleVar(3);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

    ImGui::DockSpace(dockspace_id, ImGui::GetContentRegionAvail());
}

void App::RenderFinalization()
{
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(this->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(this->window);
}

void App::Render() 
{
    this->RenderInitialization();
        
    // Stuff
    static bool searchWin = true;
    static bool infoWin = true;

    if (searchWin)
    {
        ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Search", &searchWin))   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        {

            ImGui::Text("Hello from another window!");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (double)(ImGui::GetIO().Framerate), (double)(ImGui::GetIO().Framerate));

            static std::string inputTextLabel;
            if (ImGui::InputText("Search Directory", &inputTextLabel, ImGuiInputTextFlags_EnterReturnsTrue, nullptr, nullptr) 
                or ImGui::Button("Execute"))
            {
                fs::path path = fs::path(inputTextLabel);
                if (fs::exists(path)) {
                    Directory* dir = new Directory(path);
                    this->SetSelectedDirectory(*dir);
                }
            }

            if (this->GetSelectedDirectory() != nullptr)
            {
                Directory* selectedDirectory = this->GetSelectedDirectory();
                Directory* subDirectories = selectedDirectory->getDirectories();
                File* subFiles = subFiles = selectedDirectory->getFiles();
                if (ImGui::BeginTable("split1", 2,
                    ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                {
                    static int selected = -1;
                    int dirCount = selectedDirectory->getDirectoryCount();
                    for (int i = 0; i < dirCount; i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        Directory directory = subDirectories[i];
                        if (ImGui::Selectable(directory.getFilename().string().c_str(), selected == i, ImGuiSelectableFlags_SpanAllColumns |
                            ImGuiSelectableFlags_AllowDoubleClick)) {
                            if (ImGui::IsMouseDoubleClicked(0))
                            {
                                selected = -1;
                                this->SetSelectedDirectory(subDirectories[i]);
                            }
                            else {
                                selected = i;
                                this->SetSelectedFile(*((File*)&directory));
                                infoWin = true;
                            }
                        }
                        ImGui::TableNextColumn();
                        ImGui::Text("Directory");

                    }
                    for (int i = 0; i < selectedDirectory->getFileCount(); i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        File file = subFiles[i];
                        if (ImGui::Selectable(file.getFilename().string().c_str(), selected + dirCount == i, ImGuiSelectableFlags_SpanAllColumns)) {
                            selected = i + dirCount;
                            this->SetSelectedFile(file);
                            infoWin = true;
                        }
                        ImGui::TableNextColumn();
                        ImGui::Text("File");

                    }
                    fprintf(stdout, "%d\n", dirCount);
                    ImGui::EndTable();
                }
            }   
        }
        ImGui::End();

        if (this->GetSelectedFile() != nullptr && infoWin)
        {
            File* file = this->GetSelectedFile();
            if (ImGui::Begin("Informations", &infoWin))
            {
                ImGui::Text("Test");
            }
            ImGui::End();
        }
    }

    if (shoulDemo) 
    {
        // ImGui::ShowDemoWindow(&shoulDemo);
    }

    this->RenderFinalization();
}

bool App::ShouldRun() 
{
	return !glfwWindowShouldClose(this->window);
}

File* App::GetSelectedFile()
{
    return this->selectedFile;
}

void App::SetSelectedFile(File& file)
{
    this->selectedFile = &file;
}

Directory* App::GetSelectedDirectory()
{
    return this->selectedDirectory;
}

void App::SetSelectedDirectory(Directory& diretory)
{
    this->selectedDirectory = &diretory;
}
