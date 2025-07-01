//
// Created by arunc on 01/07/2025.
//

#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP
#include <bullet/btBulletDynamicsCommon.h>
#include <functional>
#include <vector>
#include "model/World.hpp"
#include "esc/component/PhysicsComponent.hpp"
#include "esc/component/TransformComponent.hpp"
#include <gtc/quaternion.hpp>
#include "service/EditorService.hpp"
#include "esc/process/Input.hpp"
#include "service/WindowService.hpp"
#include "esc/component/MeshComponent.hpp"

namespace System {
	class PhysicsSystem : public std::enable_shared_from_this<PhysicsSystem> {
	public:
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

		using CollisionCallback = std::function<void(btRigidBody* a, btRigidBody* b)>;
		std::vector<CollisionCallback> collisionCallbacks;

		PhysicsSystem() {
			broadphase = new btDbvtBroadphase();
			collisionConfiguration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			solver = new btSequentialImpulseConstraintSolver;
			dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
			dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

		}

		void InitCallbacks() {
			std::weak_ptr<PhysicsSystem> weakSelf = shared_from_this();
			Input::Instance().MouseClicked.push_back([weakSelf](Input::MouseButton mouse) {
				if (auto self = weakSelf.lock()) {
					if (mouse == Input::MouseButton::Left) {
						glm::mat4 InverseProjectionMatrix = glm::inverse(Service::EditorService::Instance().getProjection());


						glm::mat4 InverseViewMatrix = glm::inverse(Service::EditorService::Instance().getView());

						glm::vec4 lRayStart_camera = InverseProjectionMatrix * self->lRayStart_NDC;
						lRayStart_camera /= lRayStart_camera.w;
						glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera;
						lRayStart_world /= lRayStart_world.w;
						glm::vec4 lRayEnd_camera = InverseProjectionMatrix * self->lRayEnd_NDC;
						lRayEnd_camera /= lRayEnd_camera.w;
						glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;
						lRayEnd_world /= lRayEnd_world.w;

						glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
						lRayDir_world = glm::normalize(lRayDir_world);

						auto out_origin = glm::vec3(lRayStart_world);
						glm::vec3 out_direction = glm::vec3(0, 1, 0);
						if (glm::length(lRayDir_world) > 0.0001f) {
							out_direction = glm::normalize(lRayDir_world);
						}
						else {
							out_direction = glm::vec3(0, 1, 0);
						}

						glm::vec3 out_end = out_origin + out_direction * 1000.0f;

						btCollisionWorld::ClosestRayResultCallback RayCallback(
							btVector3(out_origin.x, out_origin.y, out_origin.z),
							btVector3(out_end.x, out_end.y, out_end.z)
						);


						self->dynamicsWorld->rayTest(
							btVector3(out_origin.x, out_origin.y, out_origin.z),
							btVector3(out_end.x, out_end.y, out_end.z),
							RayCallback
						);

						if (RayCallback.hasHit()) {
							const btRigidBody* hitObject = btRigidBody::upcast(RayCallback.m_collisionObject);
							if (hitObject) {

								void* userPtr = hitObject->getUserPointer();
								if (userPtr) {
									auto* hitEntity = static_cast<Entity::Entity*>(hitObject->getUserPointer());
									//	std::cout << "Hit entity with name: " << hitEntity->Name << std::endl;
									Service::EditorService::Instance().setSelectedEntity(hitEntity);
								}


							}
						}

					}
				}
				}
			);

			Input::Instance().MouseReleased.push_back([this](Input::MouseButton mouse) {

				});



			Input::Instance().MouseMove.push_back([weakSelf](glm::vec2 pos) {
				if (auto self = weakSelf.lock()) {
					if (!self->dynamicsWorld) return;

					self->lRayStart_NDC = glm::vec4(
						(2.0f * pos.x) / WindowService::getInstance()->getWidth() - 1.0f,
						1.0f - (2.0f * pos.y) / WindowService::getInstance()->getHeight(),
						-1.0f,
						1.0f
					);

					self->lRayEnd_NDC = glm::vec4(
						(2.0f * pos.x) / WindowService::getInstance()->getWidth() - 1.0f,
						1.0f - (2.0f * pos.y) / WindowService::getInstance()->getHeight(),
						0.0f,
						1.0f
					);



				}
				});


		}

		void AddPhysicsBody(const std::shared_ptr<Entity::Entity>& entity)
		{
			std::shared_ptr<Component::PhysicsComponent> phys = entity->GetComponent<Component::PhysicsComponent>();
			if (!phys)
			{
				return;
			}
			std::shared_ptr<Component::TransformComponent> transform = entity->GetComponent<Component::TransformComponent>();
			if (!transform)
			{
				return;
			}

			std::shared_ptr<Component::MeshComponent> mesh = entity->GetComponent<Component::MeshComponent>();
			if (!transform)
			{
				return;
			}
			btConvexHullShape* shape = new btConvexHullShape();
			mesh->LoadMesh();

			for (const auto& v : mesh->runtimeMesh->positions) {
				shape->addPoint(btVector3(v.x, v.y, v.z));
			}
			shape->optimizeConvexHull();
			shape->initializePolyhedralFeatures();


			shape->setLocalScaling(btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
			//btBoxShape* shape = new btBoxShape(btVector3(1, 1, 1));

			btTransform offset;
			offset.setIdentity();
			offset.setOrigin(btVector3(0, -0.5f, 0));

			btCompoundShape* compoundShape = new btCompoundShape(shape);
			btVector3 position(transform->position.x, transform->position.y, transform->position.z);

			btQuaternion rotationQuat(
				transform->rotation.x,
				transform->rotation.y,
				transform->rotation.z,
				transform->rotation.w
			);

			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(position);
			startTransform.setRotation(rotationQuat);

			btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

			btVector3 inertia(0, 0, 0);
			if (!phys->isKinematic && phys->mass > 0)
				shape->calculateLocalInertia(phys->mass, inertia);

			btRigidBody::btRigidBodyConstructionInfo rbInfo(phys->mass, motionState, shape, inertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			body->setUserPointer(entity.get());
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

			dynamicsWorld->addRigidBody(body);

			phys->rigidBody = body;
			phys->motionState = motionState;
			phys->shape = shape;
		}


		void Step(World* scene, float deltaTime) {
			dynamicsWorld->stepSimulation(deltaTime);

			for (auto& entity : scene->GetEntities()) {
				auto physicsComponent = entity->GetComponent<Component::PhysicsComponent>();
				auto transformComponent = entity->GetComponent<Component::TransformComponent>();
				if (physicsComponent && transformComponent) {
					SyncTransformFromPhysics(*transformComponent, *physicsComponent);
				}
			}
		}

		void SyncTransformFromPhysics(Component::TransformComponent& transform, const Component::PhysicsComponent& physics)
		{
			if (!physics.rigidBody) return;

			btTransform trans;
			physics.rigidBody->getMotionState()->getWorldTransform(trans);

			btVector3 origin = trans.getOrigin();
			transform.position = glm::vec3(origin.getX(), origin.getY(), origin.getZ());

			btQuaternion rot = trans.getRotation();
			glm::quat q(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
			transform.rotation = q;
		}


		~PhysicsSystem() {
			delete dynamicsWorld;
			delete solver;
			delete dispatcher;
			delete collisionConfiguration;
			delete broadphase;
		}

	private:
		glm::vec4 lRayStart_NDC;
		glm::vec4 lRayEnd_NDC;
	};
}
#endif //PHYSICSSYSTEM_HPP
