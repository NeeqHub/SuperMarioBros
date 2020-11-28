#include "CKeyboardMovement.h"
#include "Object.h"

CKeyboardMovement::CKeyboardMovement(Object* owner)
	: Component(owner), moveSpeed(7.5f), currentTime(0.0f), totalTimeInAir(0.0f) {}

void CKeyboardMovement::Awake()
{
	velocity = owner->getComponent<CVelocity>();
}

void CKeyboardMovement::setMovementSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

void CKeyboardMovement::Update(float deltaTime)
{
	sf::Vector2f acc(0.0f, 0.0f);
	sf::Vector2f vel(0.0f, 0.0f);

	acc.y = velocity->acceleration.y;
	//std::cout << "Mario velocity.x: " << velocity->Get().x << std::endl;
	//std::cout << "Mario velocity.y: " << velocity->Get().y << std::endl;
	//std::cout << std::endl;

	//std::cout << owner->transform->canJump << std::endl;

	if (currentTime >= 5.0f)
	{
		//currentTime = 0.0f;
	}

	// Death animation...
	if (owner->disableInput == true)
	{


		currentTime += deltaTime;

		if (currentTime <= 0.5f)
			velocity->SetAcc(0.0f, -250.0f);
		else
			velocity->SetAcc(0.0f, 50.0f);

		velocity->Set(0.0f, 0.0f);
		return;
	}

	if (owner->context->input->isKeyPressed(Input::Key::Down))
	{
		acc.x = 0;
		velocity->SetAcc(acc.x, acc.y);
	}

	if (owner->context->input->isKeyUp(Input::Key::Left) || owner->context->input->isKeyUp(Input::Key::Right))
	{
		acc.x = 0;
		velocity->SetAcc(acc.x, acc.y);
	}

	if (owner->context->input->isKeyPressed(Input::Key::Left))
	{
		acc.x = -moveSpeed;
		if (owner->context->input->isKeyPressed(Input::Key::Shift)) {
			acc.x = -moveSpeed * 1.5;
		}
		velocity->SetAcc(acc.x, acc.y);
	}
	
	if (owner->context->input->isKeyPressed(Input::Key::Right))
	{
		acc.x = moveSpeed;
		if (owner->context->input->isKeyPressed(Input::Key::Shift)) {
			acc.x = moveSpeed * 1.5;
		}
		velocity->SetAcc(acc.x, acc.y);
	}

	if (owner->context->input->isKeyPressed(Input::Key::Up) && owner->transform->canJump == true)
	{
		acc.y = jumpAccel;
		velocity->SetAcc(acc.x, acc.y);
	}
	else if (owner->transform->canJump == true)
	{
		vel.y = 0.2f;
		velocity->Set(velocity->Get().x, vel.y);
	}
	else
	{
		acc.y = GRAVITY;
		velocity->SetAcc(acc.x, acc.y);
	}

	//velocity->SetAcc(acc.x, acc.y);
	//velocity->Set(velocity->Get().x, vel.y);
}