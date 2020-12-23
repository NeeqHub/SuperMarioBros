#include "Animation.h"

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
