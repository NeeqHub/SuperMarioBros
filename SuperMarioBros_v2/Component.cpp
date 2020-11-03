#include "Component.h"
#include "Object.h"
#include <iostream>
#include "CAnimation.h"
#include "CVelocity.h"
#include "CCollider.h"





unsigned int C_InstanceID::count = 0;

C_InstanceID::C_InstanceID(Object* owner) : Component(owner), id(count++) {}

C_InstanceID::~C_InstanceID() {}

unsigned int C_InstanceID::Get()
{
	return id;
}

Quadtree::Quadtree() : Quadtree(10, 10, 0, { 0.f, 0.f, 1920, 1080 },
	nullptr) {}

Quadtree::Quadtree(int maxObjects, int maxLevels, int level,
	sf::FloatRect bounds, Quadtree* parent)
	: maxObjects(maxObjects), maxLevels(maxLevels),
	level(level), bounds(bounds), parent(parent) {}

void Quadtree::Insert(std::shared_ptr<CBoxCollider> object)
{
	if (children[0] != nullptr) // 1
	{
		int indexToPlaceObject =
			GetChildIndexForObject(object->GetCollidable()); // 2

		if (indexToPlaceObject != thisTree) // 3
		{
			children[indexToPlaceObject]->Insert(object);
			return;
		}
	}

	objects.emplace_back(object); // 4

	if (objects.size() > maxObjects &&
		level < maxLevels && children[0] == nullptr) // 5
	{
		Split(); // 6

		auto objIterator = objects.begin(); // 7
		while (objIterator != objects.end())
		{
			auto obj = *objIterator;
			int indexToPlaceObject =
				GetChildIndexForObject(obj->GetCollidable());

			if (indexToPlaceObject != thisTree)
			{
				children[indexToPlaceObject]->Insert(obj);
				objIterator = objects.erase(objIterator);

			}
			else
			{
				++objIterator;
			}
		}
	}
}

void Quadtree::Clear()
{
	objects.clear();

	for (int i = 0; i < 4; i++)
	{
		if (children[i] != nullptr)
		{
			children[i]->Clear();
			children[i] = nullptr;
		}
	}
}

void Quadtree::Remove(std::shared_ptr<CBoxCollider> object)
{
	int index = GetChildIndexForObject(object->GetCollidable());

	if (index == thisTree || children[index] == nullptr)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects.at(i)->owner->instanceID->Get()
				== object->owner->instanceID->Get())
			{
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}
	else
	{
		return children[index]->Remove(object);
	}
}

std::vector<std::shared_ptr<CBoxCollider>> Quadtree::Search(const sf::FloatRect& area)
{
	std::vector<std::shared_ptr<CBoxCollider>> possibleOverlaps; // 1
	Search(area, possibleOverlaps); // 2

	std::vector<std::shared_ptr<CBoxCollider>> returnList;

	for (auto collider : possibleOverlaps)
	{
		if (area.intersects(collider->GetCollidable())) // 3
		{
			returnList.emplace_back(collider); // 4
		}
	}

	return returnList;
}

void Quadtree::Search(const sf::FloatRect& area,
	std::vector<std::shared_ptr<CBoxCollider>>& overlappingObjects)
{
	overlappingObjects.insert(overlappingObjects.end(), objects.begin(), objects.end()); // 1

	if (children[0] != nullptr)
	{
		int index = GetChildIndexForObject(area); // 2

		if (index == thisTree) // 3
		{
			for (int i = 0; i < 4; i++)
			{
				if (children[i]->GetBounds().intersects(area))
				{
					children[i]->Search(area, overlappingObjects);
				}
			}
		}
		else // 4
		{
			children[index]->Search(area, overlappingObjects);
		}
	}
}

const sf::FloatRect& Quadtree::GetBounds() const
{
	return bounds;
}

int Quadtree::GetChildIndexForObject(const sf::FloatRect& objectBounds)
{
	int index = -1;

	double verticalDividingLine = bounds.left + bounds.width * 0.5f;
	double horizontalDividingLine = bounds.top + bounds.height * 0.5f;

	bool north = objectBounds.top < horizontalDividingLine
		&& (objectBounds.height + objectBounds.top < horizontalDividingLine);
	bool south = objectBounds.top > horizontalDividingLine;
	bool west = objectBounds.left < verticalDividingLine
		&& (objectBounds.left + objectBounds.width < verticalDividingLine);
	bool east = objectBounds.left > verticalDividingLine;

	if (east)
	{
		if (north)
		{
			index = childNE;
		}
		else if (south)
		{
			index = childSE;
		}
	}
	else if (west)
	{
		if (north)
		{
			index = childNW;
		}
		else if (south)
		{
			index = childSW;
		}
	}

	return index;
}

void Quadtree::Split()
{
	const int childWidth = bounds.width / 2;
	const int childHeight = bounds.height / 2;

	children[childNE] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left + childWidth, bounds.top, childWidth, childHeight),
		this);
	children[childNW] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left, bounds.top, childWidth, childHeight),
		this);
	children[childSW] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left, bounds.top + childHeight, childWidth, childHeight),
		this);
	children[childSE] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left + childWidth, bounds.top + childHeight, childWidth, childHeight),
		this);
}

/*void Quadtree::DrawDebug()
{
	if (children[0] != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			children[i]->DrawDebug();
		}
	}

	Debug::DrawRect(bounds, sf::Color::Red);
}*/

S_Collidable::S_Collidable()
{
	Bitmask defaultCollisions; // 1
	defaultCollisions.setBit((int)CollisionLayer::Default);   // 2 
	collisionLayers.insert(
		std::make_pair(CollisionLayer::Default, defaultCollisions)); // 3

	collisionLayers.insert(std::make_pair(CollisionLayer::Tile, Bitmask(0)));

	Bitmask playerCollisions;
	playerCollisions.setBit((int)CollisionLayer::Default);
	playerCollisions.setBit((int)CollisionLayer::Tile);
	collisionLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));
}

void S_Collidable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto o : objects)
	{
		auto collider = o->getComponent<CBoxCollider>();
		if (collider)
		{
			CollisionLayer layer = collider->GetLayer();

			auto itr = collidables.find(layer);

			if (itr != collidables.end())
			{
				collidables[layer].push_back(collider);
			}
			else
			{
				// Refractored line below.
				collidables.insert(std::make_pair(layer, std::vector<std::shared_ptr<CBoxCollider>>{collider}));
			}

			// Remove line below.
		   // collisionTree.Insert(collider);
		}
	}
}
void S_Collidable::ProcessRemovals()
{
	//We access the layer by reference now.
	for (auto& layer : collidables)
	{
		auto itr = layer.second.begin();
		while (itr != layer.second.end())
		{
			if ((*itr)->owner->IsQueuedForRemoval())
			{
				itr = layer.second.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
void S_Collidable::Update()
{
	//collisionTree.DrawDebug(); // New line.
	ProcessCollidingObjects();

	collisionTree.Clear();
	for (auto maps = collidables.begin();
		maps != collidables.end(); ++maps)
	{
		for (auto collidable : maps->second)
		{
			collisionTree.Insert(collidable);
		}
	}

	Resolve();
}
void S_Collidable::ProcessCollidingObjects()
{
	auto itr = objectsColliding.begin();

	while (itr != objectsColliding.end())
	{
		auto pair = *itr;

		std::shared_ptr<CBoxCollider> first = pair.first;
		std::shared_ptr<CBoxCollider> second = pair.second;

		if (first->owner->IsQueuedForRemoval() || second->owner->IsQueuedForRemoval())
		{
			first->owner->OnCollisionExit(second);
			second->owner->OnCollisionExit(first);

			itr = objectsColliding.erase(itr);

		}
		else
		{
			Manifold m = first->Intersects(second);

			if (!m.colliding)
			{
				first->owner->OnCollisionExit(second);
				second->owner->OnCollisionExit(first);

				itr = objectsColliding.erase(itr);
			}
			else
			{
				first->owner->OnCollisionStay(second, m);
				second->owner->OnCollisionStay(first, m);

				++itr;
			}
		}
	}
}
void S_Collidable::Resolve()
{
	for (auto maps = collidables.begin(); maps != collidables.end(); ++maps) // 1
	{
		// If this layer collides with nothing then no need to 
		// perform any further checks.
		if (collisionLayers[maps->first].getMask() == 0)
		{
			continue;
		}

		for (auto collidable : maps->second) // 1
		{
			// If this collidable is static then no need to check if 
			// it's colliding with other objects.
			if (collidable->owner->transform->isStatic())
			{
				continue;
			}

			std::vector<std::shared_ptr<CBoxCollider>> collisions
				= collisionTree.Search(collidable->GetCollidable()); // 2

			for (auto collision : collisions) // 3
			{
				// Make sure we do not resolve collisions between the same object.
				if (collidable->owner->instanceID->Get()
					== collision->owner->instanceID->Get())
				{
					continue;
				}

				bool layersCollide =
					collisionLayers[collidable->GetLayer()].getBit(((int)collision->GetLayer()));

				if (layersCollide) // 3a
				{
					Manifold m = collidable->Intersects(collision); // 3b

					if (m.colliding)
					{
						//Debug::DrawRect(collision->GetCollidable(), sf::Color::Red);
						//Debug::DrawRect(collidable->GetCollidable(), sf::Color::Red);

						if (collision->owner->transform->isStatic())
						{
							collidable->ResolveOverlap(m);
						}
						else
						{
							collidable->ResolveOverlap(m);
						}

						auto collisionPair = objectsColliding.emplace(std::make_pair(collidable, collision));

						if (collisionPair.second)
						{
							collidable->owner->OnCollisionEnter(collision, m);
							collision->owner->OnCollisionEnter(collidable, m);
						}


						if (collision->owner->transform->isStatic())
						{
							collidable->ResolveOverlap(m); // 3c
						}
						else
						{

							//TODO: How should we handle collisions when both 
							// objects are not static?
							// We could implement rigidbodies and mass.
							collidable->ResolveOverlap(m);
						}

					}
				}
			}

		}
	}
}



C_RemoveObjectOnCollisionEnter::C_RemoveObjectOnCollisionEnter(Object* owner) : Component(owner) {}

void C_RemoveObjectOnCollisionEnter::OnCollisionEnter(std::shared_ptr<CBoxCollider> other)
{
	// Remove the projectile when it collides with any other object    
	//owner->QueueForRemoval();
}





OutputColliders::OutputColliders(Object * owner) : Component(owner)
{
}

void OutputColliders::OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	if (other->GetTag() == Tag::Surprise && m.collisionDirection == CollisionDirection::Bottom)
	{
		owner->getComponent<CVelocity>()->Set(owner->getComponent<CVelocity>()->Get().x, owner->getComponent<CVelocity>()->Get().y + 5.0f);
		other->owner->getComponent<BlocksAnim>()->OnDestory();
	}

	if (other->GetTag() == Tag::Brick && m.collisionDirection == CollisionDirection::Bottom)
	{
		owner->getComponent<CVelocity>()->Set(owner->getComponent<CVelocity>()->Get().x, owner->getComponent<CVelocity>()->Get().y + 5.0f);
		other->owner->getComponent<BlocksAnim>()->OnDestory();
	}
}

void OutputColliders::OnCollisionStay(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	if (m.collisionDirection == CollisionDirection::Top)
		owner->transform->canJump = true;
	else if (m.collisionDirection == CollisionDirection::Bottom || m.collisionDirection == CollisionDirection::Right
		|| m.collisionDirection == CollisionDirection::Left)
		owner->transform->canJump = false;
}

void OutputColliders::OnCollisionExit(std::shared_ptr<CBoxCollider> other)
{
	owner->transform->canJump = false;
}

EnemyMovement::EnemyMovement(Object * owner) : Component(owner), enemyMovementSpeed(100.0f), deathTime(0.0f)
{

}

void EnemyMovement::Awake()
{
	velocity = owner->getComponent<CVelocity>();

}

void EnemyMovement::Update(float deltaTime)
{
	if (owner->hitted == false)
	{
		if (owner->instanceID->Get() == 479)
		{
			if (owner->transform->getPosition().x <= 31.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
			else if (owner->transform->getPosition().x >= 38.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
		}

		if (owner->instanceID->Get() == 480)
		{
			if (owner->transform->getPosition().x <= 41.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
			else if (owner->transform->getPosition().x >= 48.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);

		}

		if (owner->instanceID->Get() == 481)
		{
			if (owner->transform->getPosition().x <= 117.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
			else if (owner->transform->getPosition().x >= 125.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);

		}

		if (owner->instanceID->Get() == 484)
		{
			if (owner->transform->getPosition().x <= 108.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
			else if (owner->transform->getPosition().x >= 115.0f * 48.0f)
				enemyMovementSpeed = enemyMovementSpeed * (-1);
		}

		velocity->SetAcc(enemyMovementSpeed, 0.0f);
		velocity->Set(0.0f, 0.0f);

	}
	else
	{
		velocity->SetAcc(0.0f, 0.0f);
		velocity->Set(0.0f, 0.0f);
		deathTime += deltaTime;

		if (deathTime > 0.25f)
			owner->transform->setPosition(0.0f, 2000.0f);
	}
}

EnemyAnim::EnemyAnim(Object * owner) : Component(owner)
{
}

void EnemyAnim::Awake()
{
	animation = owner->getComponent<CAnimation>();
}

void EnemyAnim::Update(float deltaTime)
{
}

void EnemyAnim::OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	if (other->GetTag() == Tag::Player && m.collisionDirection == CollisionDirection::Top)
	{
		animation->SetAnimationState(AnimationState::Death);
		owner->getComponent<CBoxCollider>()->SetSize(48.0f, 24.0f);
		owner->hitted = true;
	}
	else if (other->GetTag() == Tag::Player && m.collisionDirection == CollisionDirection::Left || m.collisionDirection == CollisionDirection::Right)
	{
		other->owner->hitted = true;
	}
}

EnemyTurtleMovement::EnemyTurtleMovement(Object * owner) : Component(owner), enemyMovementSpeed(2.0f), deathTime(0.0f)
{
}

void EnemyTurtleMovement::Awake()
{
	velocity = owner->getComponent<CVelocity>();
}

void EnemyTurtleMovement::Update(float deltaTime)
{
	//std::cout << owner->instanceID << ":" << velocity->Get().x << std::endl;

	if (owner->isPushedLeft == true)
	{
		velocity->Set(enemyMovementSpeed * 5.0f, 0.0f);
		return;
	}
	else if (owner->isPushedRight == true)
	{
		velocity->Set(-enemyMovementSpeed * 5.0f, 0.0f);
		return;
	}

	if (owner->instanceID->Get() == 482)
	{
		if (owner->hitted == false)
		{
			if (owner->transform->getPosition().x <= 95.0f * 48.0f)
				velocity->Set(enemyMovementSpeed, 0.0f);

			if (owner->transform->getPosition().x >= 105.0f * 48.0f)
				velocity->Set(-enemyMovementSpeed, 0.0f);
		}
			
	}
	else if (owner->instanceID->Get() == 483)
	{
		if (owner->hitted == false)
		{
			if (owner->transform->getPosition().x <= 51.0f * 48.0f)
					velocity->Set(enemyMovementSpeed, 0.0f);

			if (owner->transform->getPosition().x >= 58.0f * 48.0f)
					velocity->Set(-enemyMovementSpeed, 0.0f);
		}		
	}

	if (owner->isPushedLeft == false && owner->isPushedRight == false && owner->hitted == true)
	{
		velocity->SetAcc(0.0f, 0.0f);
		velocity->Set(0.0f, 0.0f);
	}
}
	
void EnemyTurtleMovement::OnCollisionStay(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	owner->transform->canJump = true;
}

void EnemyTurtleMovement::OnCollisionExit(std::shared_ptr<CBoxCollider> other)
{
	owner->transform->canJump = false;
}

EnemyTurtleAnim::EnemyTurtleAnim(Object * owner) : Component(owner)
{
}

void EnemyTurtleAnim::Awake()
{
	velocity = owner->getComponent<CVelocity>();
	animation = owner->getComponent<CAnimation>();
}

void EnemyTurtleAnim::Update(float deltaTime)
{
	const sf::Vector2f currentVelocity = velocity->Get();

	if (currentVelocity.x > 0.0f)
		animation->SetAnimationDirection(FaceDirection::Left);
	else if (currentVelocity.x < 0.0f)
		animation->SetAnimationDirection(FaceDirection::Right);

}

void EnemyTurtleAnim::OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m)
{
	Tag objectTag;
	objectTag = other->GetTag();

	CollisionDirection collisionDir;
	collisionDir = m.collisionDirection;

	switch (objectTag)
	{
	case Tag::Player:
		switch (collisionDir)
		{
		case CollisionDirection::Top:
			animation->SetAnimationState(AnimationState::Death);
			owner->hitted = true;
			break;

		case CollisionDirection::Left:
			if (owner->hitted == true)
			{
				owner->isPushedRight = true;
			}
			else
			{
				other->owner->hitted = true;
			}
			break;

		case CollisionDirection::Right:
			if (owner->hitted == true)
			{
				owner->isPushedLeft = true;
			}
			else
			{
				other->owner->hitted = true;
			}
			break;
		}
		break;

	case Tag::Enemy:
		other->owner->transform->setPosition(0.0f, 2000.0f);
		break;

	case Tag::Defult:
		switch (collisionDir)
		{
		case CollisionDirection::Left:
			if (owner->hitted == true)
			{
				owner->transform->setPosition(0.0f, 2000.0f);
			}
			break;

		case CollisionDirection::Right:
			if (owner->hitted == true)
			{
				owner->transform->setPosition(0.0f, 2000.0f);
			}
			break;
		}
		break;
	}

}

BlocksAnim::BlocksAnim(Object * owner) : Component(owner), currentTime(0.0f)
{
}

void BlocksAnim::Update(float deltaTime)
{
	if (owner->hitted == true && owner->getComponent<CBoxCollider>()->GetTag() == Tag::Brick)
	{
		currentTime += deltaTime;
		owner->transform->setY(currentY - 10.0f);

		if (currentTime >= 0.3f)
		{
			owner->transform->setY(currentY);
			owner->hitted = false;
		}
	}
}

void BlocksAnim::OnDestory()
{
	if (owner->getComponent<CBoxCollider>()->GetTag() == Tag::Surprise)
	{
		owner->getComponent<CSprite>()->SetTextureRect(32.0f, 16.0f, 16.0f, 16.0f);
	}

	if (owner->getComponent<CBoxCollider>()->GetTag() == Tag::Brick)
	{
		owner->hitted = true;
		currentY = owner->transform->getPosition().y;
	}
}

