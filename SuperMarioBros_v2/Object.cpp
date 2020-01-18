#include "Object.h"

void Object::Awake()
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Awake();
	}
}

void Object::Start()
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Start();
	}
}

void Object::Update(float deltaTime)
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Update(deltaTime);
	}
}

void Object::LateUpdate(float deltaTime)
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->LateUpdate(deltaTime);
	}
}

void Object::Draw(Window& window)
{
		drawable->Draw(window);
}

Object::Object(SharedContext* context) : context(context), queuedForRemoval(false), hitted(false)
{
	transform = addComponent<CTransform>();
	instanceID = addComponent<C_InstanceID>();
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