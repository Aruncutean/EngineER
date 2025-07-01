//
// Created by arunc on 01/07/2025.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <iostream>
#include "esc/component/ComponentFactory.hpp"
#include "esc/component/ComponentBase.hpp"

namespace Entity {
	class Entity {
	public:
		std::string Name = "Unnamed";

		template<typename T, typename... Args>
		std::enable_if_t<std::is_base_of<Component::ComponentBase, T>::value>
			AddComponent(Args&&... args) {
			auto component = std::make_shared<T>(std::forward<Args>(args)...);
			components[typeid(T)] = component;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of<Component::ComponentBase, T>::value>>
		std::shared_ptr<T> GetComponent() const {
			auto it = components.find(typeid(T));
			if (it != components.end()) {
				return std::dynamic_pointer_cast<T>(it->second);
			}
			return nullptr;
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of<Component::ComponentBase, T>::value>>
		bool HasComponent() const {
			return components.find(typeid(T)) != components.end();
		}

		boost::property_tree::ptree ToPtree() const {
			boost::property_tree::ptree root;
			root.put("Name", Name);

			boost::property_tree::ptree componentsNode;
			for (const auto& [type, compPtr] : components) {
				if (compPtr) {
					boost::property_tree::ptree compTree = compPtr->ToPtree();
					componentsNode.add_child(compPtr->GetTypeName(), compTree);
				}
			}
			root.add_child("Components", componentsNode);

			return root;
		}

		void FromPtree(const boost::property_tree::ptree& root) {
			Name = root.get<std::string>("Name", "Unnamed");

			auto componentsNode = root.get_child_optional("Components");
			if (componentsNode) {
				for (const auto& [typeName, pt] : *componentsNode) {
					auto component = Component::ComponentFactory::Instance().Create(typeName);
					if (component) {
						component->FromPtree(pt);
						components[std::type_index(typeid(*component))] = component;
					}
				}
			}
		}

	private:
		std::unordered_map<std::type_index, std::shared_ptr<Component::ComponentBase>> components;
	};
}
#endif //ENTITY_HPP
