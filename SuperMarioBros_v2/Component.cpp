#include "Component.h"
#include "Object.h"

/*
void CGraphics::LateUpdate(float deltaTime)
{
	shape.setPosition(owner->transform->getPosition());
}

void CGraphics::setTexture(const std::string& path)
{
	texture.loadFromFile(path);

	shape.setSize(sf::Vector2f(48.0f, 48.0f));
	shape.setTexture(&texture);
	shape.setPosition(sf::Vector2f(0.0f, 0.0f));
}

void CGraphics::Draw(Window& window)
{
	window.draw(shape);
}
*/

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

			// We haven’t created this method yet but we’ll do that shortly.
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

CTransform::CTransform(Object * owner) : Component(owner), position(0.0f, 0.0f)
{
}

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