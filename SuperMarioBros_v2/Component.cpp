#include "Component.h"
#include "Object.h"

CSprite::CSprite(Object* owner) : Component(owner), currentTextureID(-1) // Set current texture id to -1 
{}

void CSprite::SetTextureAllocator(ResourceManager<sf::Texture>* allocator)
{
	this->allocator = allocator;
}

void CSprite::Load(int id)
{
	// Check its not already our current texture.
	if (id >= 0 && id != currentTextureID)
	{
		currentTextureID = id;
		std::shared_ptr<sf::Texture> texture = allocator->get(id);
		sprite.setTexture(*texture);
	}
}

void CSprite::Load(const std::string& filePath)
{
	if (allocator)
	{
		int textureID = allocator->add(filePath);

		// Check its not already our current texture.
		if (textureID >= 0 && textureID != currentTextureID)
		{
			currentTextureID = textureID;
			std::shared_ptr<sf::Texture> texture = allocator->get(textureID);
			sprite.setTexture(*texture);
		}
	}
}

void CSprite::Draw(Window& window)
{
	window.draw(sprite);
}

void CSprite::LateUpdate(float deltaTime)
{
	sprite.setPosition(owner->transform->getPosition());
}

void CSprite::SetTextureRect(int x, int y, int width, int height)
{
	sprite.setTextureRect(sf::IntRect(x, y, width, height));
}

void CSprite::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

void CSprite::SetScale(unsigned int x, unsigned int y)
{
	sprite.setScale(x,y);
}

Animation::Animation(FaceDirection direction) : frames(0), currentFrameIndex(0), currentFrameTime(0.0f), direction(direction) {}

void Animation::AddFrame(int textureID, int x, int y, int height, int width, float frameTime)
{
	FrameData data;
	data.id = textureID;
	data.x = x;
	data.y = y;
	data.height = height;
	data.width = width;
	data.displayTimeSeconds = frameTime;

	frames.push_back(data);
}

const FrameData* Animation::GetCurrentFrame() const
{
	if (frames.size() > 0)
	{
		return &frames[currentFrameIndex];
	}

	return nullptr;
}

bool Animation::UpdateFrame(float deltaTime)
{
	if (frames.size() > 0)
	{
		currentFrameTime += deltaTime;

		if (currentFrameTime >= frames[currentFrameIndex].displayTimeSeconds)
		{
			currentFrameTime = 0.0f;
			IncrementFrame();
			return true;
		}
	}

	return false;
}

void Animation::IncrementFrame()
{
	currentFrameIndex = (currentFrameIndex + 1) % frames.size();
}

void Animation::Reset()
{
	currentFrameIndex = 0;
	currentFrameTime = 0.0f;
}

void Animation::SetDirection(FaceDirection direction)
{
	if (direction != this->direction)
	{
		this->direction = direction;
		for (auto& f : frames)
		{
			f.x += f.width;
			f.width *= -1;
		}
	}
}

FaceDirection Animation::GetDirection() const
{
	return direction;
}

CAnimation::CAnimation(Object* owner) : Component(owner), currentAnimation(AnimationState::None, nullptr)
{

}

void CAnimation::Awake()
{
	sprite = owner->getComponent<CSprite>();
}

void CAnimation::Update(float deltaTime)
{
	if (currentAnimation.first != AnimationState::None)
	{
		bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

		if (newFrame)
		{
			const FrameData* data = currentAnimation.second->GetCurrentFrame();

			sprite->Load(data->id); // 1   

			// We haven�t created this method yet but we�ll do that shortly.
			sprite->SetTextureRect(data->x, data->y, data->width, data->height);
		}
	}
}

void CAnimation::AddAnimation(AnimationState state, std::shared_ptr<Animation> animation)
{
	auto inserted = animations.insert(std::make_pair(state, animation));

	if (currentAnimation.first == AnimationState::None)
	{
		SetAnimationState(state);
	}
}

void CAnimation::SetAnimationState(AnimationState state)
{
	// We only set an animation of it is not already 
	// our current animation.
	if (currentAnimation.first == state)
	{
		return;
	}

	auto animation = animations.find(state);
	if (animation != animations.end())
	{
		currentAnimation.first = animation->first;
		currentAnimation.second = animation->second;

		currentAnimation.second->Reset();
	}
}

void CAnimation::SetAnimationDirection(FaceDirection direction)
{
	if (currentAnimation.first != AnimationState::None)
	{
		currentAnimation.second->SetDirection(direction);
	}
}

const AnimationState& CAnimation::GetAnimationState() const
{
	// Returns out current animation state. We can use this 
	// to compare the objects current state to a desired state.
	return currentAnimation.first;
}

CTransform::CTransform(Object * owner) : Component(owner), position(0.0f, 0.0f), isStaticTransform(false)
{
}

void CTransform::SetStatic(bool isStatic) { isStaticTransform = isStatic; }

bool CTransform::isStatic() const { return isStaticTransform; }

void CTransform::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void CTransform::setPosition(sf::Vector2f pos)
{
	position = pos;
}

void CTransform::addPosition(float x, float y)
{
	position.x += x;
	position.y += y;
}

void CTransform::addPosition(sf::Vector2f pos)
{
	position += pos;
}

void CTransform::setX(float x)
{
	position.x = x;
}

void CTransform::setY(float y)
{
	position.y = y;
}

void CTransform::addX(float x)
{
	position.x += x;
}

void CTransform::addY(float y)
{
	position.y += y;
}

const sf::Vector2f & CTransform::getPosition() const
{
	return position;
}

CKeyboardMovement::CKeyboardMovement(Object* owner)
	: Component(owner), moveSpeed(100) {}

void CKeyboardMovement::Awake()
{
	animation = owner->getComponent<CAnimation>();
}

void CKeyboardMovement::setInput(Input* input)
{
	this->input = input;
}

void CKeyboardMovement::setMovementSpeed(int moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

void CKeyboardMovement::Update(float deltaTime)
{
	if (input == nullptr)
	{
		return;
	}

	int xMove = 0;
	if (input->isKeyPressed(Input::Key::Left))
	{
		animation->SetAnimationDirection(FaceDirection::Left);
		xMove = -moveSpeed;
	}
	else if (input->isKeyPressed(Input::Key::Right))
	{
		animation->SetAnimationDirection(FaceDirection::Right);
		xMove = moveSpeed;
	}

	int yMove = 0;
	if (input->isKeyPressed(Input::Key::Up))
	{
		yMove = -moveSpeed;
	}
	else if (input->isKeyPressed(Input::Key::Down))
	{
		yMove = moveSpeed;
	}

	float xFrameMove = xMove * deltaTime;
	float yFrameMove = yMove * deltaTime;


	if (xMove == 0 && yMove == 0)
		animation->SetAnimationState(AnimationState::Idle);
	else
		animation->SetAnimationState(AnimationState::Walk);

	owner->transform->addPosition(xFrameMove, yFrameMove);
}

CDrawable::CDrawable() : sortOrder(0) {}

CDrawable::~CDrawable() {}

void CDrawable::Draw(Window & window)
{
}

void CDrawable::SetSortOrder(int order)
{
	sortOrder = order;
}

int CDrawable::GetSortOrder() const
{
	return sortOrder;
}

void SDrawable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto o : objects)
	{
		Add(o);
	}

	Sort();
}

void SDrawable::Add(std::shared_ptr<Object> object)
{
	std::shared_ptr<CDrawable> draw = object->GetDrawable();

	if (draw)
	{
		drawables.emplace_back(object);
	}
}

void SDrawable::Sort()
{
	std::sort(drawables.begin(), drawables.end(), [](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool
	{
		return a->GetDrawable()->GetSortOrder() < b->GetDrawable()->GetSortOrder();
	}
	);
}

void SDrawable::ProcessRemovals()
{
	auto objIterator = drawables.begin();
	while (objIterator != drawables.end())
	{
		auto obj = *objIterator;

		if (obj->IsQueuedForRemoval())
		{
			objIterator = drawables.erase(objIterator);
		}
		else
		{
			++objIterator;
		}
	}
}

void SDrawable::Draw(Window& window)
{
	for (auto& d : drawables)
	{
		d->Draw(window);
	}
}

CCollider::CCollider(Object* owner)
	: Component(owner), layer(CollisionLayer::Default) {}

CCollider::~CCollider() {}

CollisionLayer CCollider::GetLayer() const
{
	return layer;
}

void CCollider::SetLayer(CollisionLayer layer)
{
	this->layer = layer;
}

CBoxCollider::CBoxCollider(Object* owner) : CCollider(owner),
offset(sf::Vector2f(0.f, 0.f)) {}

void CBoxCollider::SetCollidable(const sf::FloatRect& rect)
{
	AABB = rect;
	SetPosition();
}

const sf::FloatRect& CBoxCollider::GetCollidable()
{
	SetPosition();
	return AABB;
}

void CBoxCollider::SetPosition()
{
	const sf::Vector2f& pos = owner->transform->getPosition();

	AABB.left = pos.x - (AABB.width / 2) + offset.x;
	AABB.top = pos.y - (AABB.height / 2) + offset.y;
}

Manifold CBoxCollider::Intersects(std::shared_ptr<CCollider> other)
{
	Manifold m;
	m.colliding = false; // We default to not colliding.

	std::shared_ptr<CBoxCollider> boxCollider
		= std::dynamic_pointer_cast<CBoxCollider>(other); // 1

	if (boxCollider) // 2
	{
		const sf::FloatRect& rect1 = GetCollidable();
		const sf::FloatRect& rect2 = boxCollider->GetCollidable();

		if (rect1.intersects(rect2)) // 3
		{
			m.colliding = true; // 4
			m.other = &rect2;
		}
	}

	return m;
}

void CBoxCollider::ResolveOverlap(const Manifold& m)
{
	auto transform = owner->transform;

	if (transform->isStatic()) { return; } // 1

	const sf::FloatRect& rect1 = GetCollidable();
	const sf::FloatRect* rect2 = m.other;

	float resolve = 0; // 2
	float xDiff = (rect1.left + (rect1.width * 0.5f))
		- (rect2->left + (rect2->width * 0.5f)); // 3
	float yDiff = (rect1.top + (rect1.height * 0.5f))
		- (rect2->top + (rect2->height * 0.5f));

	if (fabs(xDiff) > fabs(yDiff)) // 4
	{
		if (xDiff > 0) // Colliding on the left.
		{
			// We add a positive x value to move the object to the right.
			resolve = (rect2->left + rect2->width) - rect1.left;
		}
		else // Colliding on the right.
		{
			// We add a negative x value to move the object to the left.
			resolve = -((rect1.left + rect1.width) - rect2->left);
		}

		transform->addPosition(resolve, 0); // 5
	}
	else
	{
		if (yDiff > 0) // Colliding above.
		{
			// We add a positive y value to move the object down.
			resolve = (rect2->top + rect2->height) - rect1.top;
		}
		else // Colliding below
		{
			// We add a negative y value to move the object up.
			resolve = -((rect1.top + rect1.height) - rect2->top);
		}

		transform->addPosition(0, resolve); // 5
	}
}

int C_InstanceID::count = 0;

C_InstanceID::C_InstanceID(Object* owner) : Component(owner), id(count++) {}

C_InstanceID::~C_InstanceID() {}

int C_InstanceID::Get() const
{
	return id;
}

Quadtree::Quadtree() : Quadtree(5, 5, 0, { 0.f, 0.f, 1920, 1080 },
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
				std::vector<std::shared_ptr<CBoxCollider>> objs;
				objs.push_back(collider);

				collidables.insert(std::make_pair(layer, objs));
			}
		}
	}
}
void S_Collidable::ProcessRemovals()
{
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

