//
// Created by arunc on 01/07/2025.
//

#include "component/Component.hpp"

#include "SDL_image.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_surface.h>

namespace UI {

	void Text(const std::string& text) {
		ImGui::Text("%s", text.c_str());
	}

	bool InputText(const std::string& label, std::string& value, int bufferSize, bool enabled)
	{
		char* buffer = new char[bufferSize];
		size_t copyLen = std::min(value.size(), static_cast<size_t>(bufferSize - 1));
		std::memcpy(buffer, value.c_str(), copyLen);
		buffer[copyLen] = '\0';

		if (!enabled)
			ImGui::BeginDisabled();

		bool changed = ImGui::InputText(("##" + label).c_str(), buffer, bufferSize);

		if (!enabled)
			ImGui::EndDisabled();

		if (changed)
			value = std::string(buffer);

		delete[] buffer;
		return changed;
	}


	bool Checkbox(const std::string& label, bool& value) {
		return ImGui::Checkbox(label.c_str(), &value);
	}

	void Button(const std::string& label, const std::function<void()>& onClick) {
		if (ImGui::Button(label.c_str())) {
			if (onClick) onClick();
		}
	}

	void ImageButton(
		const std::string& id,
		ImTextureID texture,
		const ImVec2& size,
		const std::function<void()>& onClick
	)
	{
		ImVec2 uv0 = ImVec2(0, 1);
		ImVec2 uv1 = ImVec2(1, 0);
		if (ImGui::ImageButton(id.c_str(), texture, size)) {
			if (onClick) onClick();
		}
	}

	void Window(const std::string& title, bool& isOpen, ImGuiWindowFlags windowFlags, const std::function<void()>& content) {
		if (!isOpen) return;

		if (ImGui::Begin(title.c_str(), &isOpen, windowFlags)) {
			if (content) content();
		}
		ImGui::End();
	}

	void Window(const std::string& title, bool& isOpen, const std::function<void()>& content) {
		Window(title, isOpen, ImGuiWindowFlags_AlwaysAutoResize, content);
	}

	void  ListBoxWithEvents(
		const std::string& label,
		const std::vector<std::string>& items,
		int& selectedIndex,
		const std::function<void(int, const std::string&)>& onSelect,
		const std::function<void(int, const std::string&)>& onDoubleClick,
		const std::function<void(int, const std::string&)>& contextMenuBuilder,
		int heightInItems
	) {
		if (ImGui::BeginListBox(("##" + label).c_str(), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * heightInItems))) {
			for (int i = 0; i < items.size(); ++i) {
				bool isSelected = (selectedIndex == i);

				if (ImGui::Selectable(items[i].c_str(), isSelected)) {
					if (!isSelected) {
						selectedIndex = i;
						if (onSelect) onSelect(i, items[i]);
					}
				}

				if (ImGui::IsItemHovered()) {
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						if (onDoubleClick) onDoubleClick(i, items[i]);
					}
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup(("##context_" + label + "_" + std::to_string(i)).c_str());
					}
				}

				if (ImGui::BeginPopup(("##context_" + label + "_" + std::to_string(i)).c_str())) {
					if (contextMenuBuilder) contextMenuBuilder(i, items[i]);
					ImGui::EndPopup();
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
	}

	GLuint LoadTextureFromFile(const std::string& path)
	{
		if (!std::filesystem::exists(path))
			return 0;

		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) {
			return 0;
		}

		// Convert to RGBA32 format if needed
		SDL_Surface* rgbaSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
		SDL_DestroySurface(surface); // free the original

		if (!rgbaSurface)
			return 0;

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					 rgbaSurface->w, rgbaSurface->h, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_DestroySurface(rgbaSurface); // free the converted surface

		return textureID;
	}

	std::string openDialog() {
		const char* file = tinyfd_openFileDialog(
			"Open File",
			"",
			0,
			nullptr,
			nullptr,
			0);

		if (file) {
			return std::string(file);
		}
		return "";
	}

	std::string openFolderDialog() {
		const char* folder = tinyfd_selectFolderDialog(
			"Select Folder",
			"");

		if (folder) {
			return std::string(folder);
		}
		return "";
	}
}
