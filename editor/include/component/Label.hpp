//
// Created by arunc on 03/07/2025.
//

#ifndef LABEL_HPP
#define LABEL_HPP

namespace component {
    class Label : public IComponent {
    public:
        explicit Label(const std::string &text) : text(text) {
        }

        void initialize() override {
        }

        void render() override {
            ImGui::Text("%s", text.c_str());
        }

        void SetText(const std::string &newText) {
            text = newText;
        }

    private:
        std::string text;
    };
}
#endif //LABEL_HPP
