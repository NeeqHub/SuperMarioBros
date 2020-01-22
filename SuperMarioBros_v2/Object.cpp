#include "Object.h"

Object::Object(SharedContext* context) : context(context), queuedForRemoval(false), hitted(false), disableInput(false), isPushedLeft(false), isPushedRight(false)
{
	transform = addComponent<CTransform>();
	instanceID = addComponent<C_InstanceID>();
}

void Object::Awake()
{
	for (const auto& component : components)
	{
		component->Awake();
	}
}

void Object::Start()
{
	for (const auto& component : components)
	{
		component->Start();
	}
}

void Object::Update(float deltaTime)
{
	for (const auto& component : components)
	{
		component->Update(deltaTime);
	}
}

void Object::LateUpdate(float deltaTime)
{
	for (const auto& component : components)
	{
		component->LateUpdate(deltaTime);
	}
}

void Object::Draw(Window& window)
{
		drawable->Draw(window);
}

void Object::QueueForRemoval()
{
	queuedForRemoval = true;
}

bool Object::IsQueuedForRemoval()
{
	return  queuedForRemoval;
}

std::shared_ptr<CDrawable> Object::GetDrawable()
{
	return drawable;
}

void Object::OnCollisionEnter(std::shared_ptr<CBoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionEnter(other);
	}
}

void Object::OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionEnter(other, m);
	}
}

void Object::OnCollisionStay(std::shared_ptr<CBoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionStay(other);
	}
}

void Object::OnCollisionExit(std::shared_ptr<CBoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionExit(other);
	}
}