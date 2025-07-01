//
// Created by arunc on 01/07/2025.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP



#include <imgui.h>
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <fstream>

#include <tinyfiledialogs/tinyfiledialogs.h>
#include <glad/glad.h>

namespace UI {

    void Text(const std::string& text);


    bool Checkbox(const std::string& label, bool& value);

    void Button(const std::string& label, const std::function<void()>& onClick);

    void ImageButton(
        const std::string& id,
        ImTextureID texture,
        const ImVec2& size,
        const std::function<void()>& onClick
    );

    bool InputText(const std::string& label, std::string& value, int bufferSize, bool enabled);

    void Window(const std::string& title, bool& isOpen, ImGuiWindowFlags windowFlags, const std::function<void()>& content);

    void Window(const std::string& title, bool& isOpen, const std::function<void()>& content);

    void ListBoxWithEvents(
        const std::string& label,
        const std::vector<std::string>& items,
        int& selectedIndex,
        const std::function<void(int, const std::string&)>& onSelect,
        const std::function<void(int, const std::string&)>& onDoubleClick,
        const std::function<void(int, const std::string&)>& contextMenuBuilder,
        int heightInItems
    );

    GLuint LoadTextureFromFile(const std::string& path);

    std::string openDialog();

    std::string openFolderDialog();
}


#endif //COMPONENT_HPP
