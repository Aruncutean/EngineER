//
// Created by arunc on 03/07/2025.
//

#ifndef VIEWSERVICE_HPP
#define VIEWSERVICE_HPP
#include <algorithm>
#include <vector>
#include <memory>
#include "component/View.hpp"


namespace service_editor {
    class ViewService {
    public:
        static ViewService &Instance() {
            static ViewService instance;
            return instance;
        }

        void AddView(std::shared_ptr<View> view) {
            views.push_back(view);
        }

        bool GetViewByName(const std::string &name) const {
            for (auto &view: views) {
                if (view != nullptr && view->isOpen) {
                    if (view->name == name) {
                        return true;
                    }
                }
            }
            return false;
        }

        void RemoveView(std::shared_ptr<View> view) {
            views.erase(
                std::remove_if(views.begin(), views.end(),
                               [&](const std::shared_ptr<View> &v) {
                                   return *v == *view;
                               }),
                views.end()
            );
        }

        void AddComponent(std::shared_ptr<component::IComponent> component) {
            components.push_back(component);
        }

        void RemoveComponent(std::shared_ptr<component::IComponent> component) {
            components.erase(
                std::remove_if(components.begin(), components.end(),
                               [&](const std::shared_ptr<component::IComponent> &c) {
                                   return c == component;
                               }),
                components.end()
            );
        }

        void RenderViews() {
            for (auto &view: views) {
                if (view != nullptr && view->isOpen) {
                    view->render();
                }
            }

            for (auto &component: components) {
                if (component != nullptr)
                    component->render();
            }
        }

        void ClearViews() {
            views.clear();
        }

    private:
        ViewService() = default;

        ~ViewService() = default;

        ViewService(const ViewService &) = delete;

        ViewService &operator=(const ViewService &) = delete;


        std::vector<std::shared_ptr<View> > views;

        std::vector<std::shared_ptr<component::IComponent> > components;
    };
}
#endif
