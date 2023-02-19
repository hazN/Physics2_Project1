#include "AnimationManager.h"

#include <glm/gtx/easing.hpp>

#include <iostream>

#include "globalThings.h"

#define PRINT_DEBUG_INFO

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::Update(const std::vector<cMeshObject*>& gameObjects, float dt)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		cMeshObject* go = gameObjects[i];

		if (!go->Enabled)
			continue;

		if (go->Animation.AnimationType.length() != 0)
		{
			Animation& animation = go->Animation;
			std::map<std::string, AnimationData>::iterator itFind = m_Animations.find(go->Animation.AnimationType);
			const AnimationData& animationData = itFind->second;
			if (itFind != m_Animations.end())
			{
				if (reverse)
				{
					if (animation.IsPlaying && animation.Speed != 0.0f)
					{
						if (animation.AnimationTime == animationData.Duration)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::STARTED;
							}
						}
						else if (animation.AnimationTime < 0.1f && animation.AnimationTime < animationData.Duration)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::ONGOING;
							}
						}
						else if (animation.AnimationTime <= 0.0f)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::FINISHED;
							}
						}
						animation.AnimationTime -= dt * animation_speed;
						if (animation.AnimationTime < 0.0f)
						{
							if (animation.IsLooping)
							{
								if (animation.Speed > 0)
								{
									animation.AnimationTime = animationData.Duration;
								}
								else
								{
									animation.AnimationTime = 0.0f;
								}
							}
							else
							{
								animation.AnimationTime = 0.0f;
								animation.IsPlaying = false;
								for (Event* event : event_handler->events_)
								{
									if (event->animation_name == animation.AnimationType)
										event->status = eStatus::FINISHED;
								}
							}
						}
						else if (animation.AnimationTime > animationData.Duration)
						{
							if (animation.IsLooping)
							{
								if (animation.Speed < 0)
								{
									animation.AnimationTime = 0.0f;
								}
								else
								{
									animation.AnimationTime = animationData.Duration;
								}
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
								animation.IsPlaying = false;
								for (Event* event : event_handler->events_)
								{
									if (event->animation_name == animation.AnimationType)
										event->status = eStatus::FINISHED;
								}
							}
						}
					}
				}
				else
				{
					if (animation.IsPlaying && animation.Speed != 0.0f)
					{
						if (animation.AnimationTime == 0.f)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::STARTED;
							}
						}
						else if (animation.AnimationTime > 0.1f && animation.AnimationTime < animationData.Duration)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::ONGOING;
							}
						}
						else if (animation.AnimationTime >= animationData.Duration)
						{
							for (Event* event : event_handler->events_)
							{
								if (event->animation_name == animation.AnimationType)
									event->status = eStatus::FINISHED;
							}
						}
						animation.AnimationTime += dt * animation_speed;
						if (animation.AnimationTime > animationData.Duration)
						{
							if (animation.IsLooping)
							{
								if (animation.Speed > 0)
								{
									animation.AnimationTime = 0.0f;
								}
								else
								{
									animation.AnimationTime = animationData.Duration;
								}
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
								animation.IsPlaying = false;
								for (Event* event : event_handler->events_)
								{
									if (event->animation_name == animation.AnimationType)
										event->status = eStatus::FINISHED;
								}
							}
						}
						else if (animation.AnimationTime < 0.f)
						{
							if (animation.IsLooping)
							{
								if (animation.Speed < 0)
								{
									animation.AnimationTime = animationData.Duration;
								}
								else
								{
									animation.AnimationTime = 0.f;
								}
							}
							else
							{
								animation.AnimationTime = 0.f;
								animation.IsPlaying = false;
								for (Event* event : event_handler->events_)
								{
									if (event->animation_name == animation.AnimationType)
										event->status = eStatus::FINISHED;
								}
							}
						}
					}
				}
				go->position = GetAnimationPosition(itFind->second, animation.AnimationTime);
				go->scaleXYZ = GetAnimationScale(itFind->second, animation.AnimationTime);
				go->qRotation = GetAnimationRotation(itFind->second, animation.AnimationTime);
				EasingType type = GetEase(itFind->second, animation.AnimationTime);
				if (type == None)
					go->RGBA_colour = glm::vec4(1, 1, 1, 1);
				else if (type == EaseIn)
					go->RGBA_colour = glm::vec4(1, 0, 0, 1);
				else if (type == EaseOut)
					go->RGBA_colour = glm::vec4(1, 1, 0, 1);
				else if (type == EaseInOut)
					go->RGBA_colour = glm::vec4(0, 1, 0.25, 1);
#ifdef PRINT_DEBUG_INFO
				//printf("Position: %.2f, %.2f, %.2f\n", go->Position.x, go->Position.y, go->Position.z);
				//printf("Scale: %.2f, %.2f, %.2f\n", go->Scale.x, go->Scale.y, go->Scale.z);
#endif
			}
		}
	}
}

bool AnimationManager::LoadAnimation(const std::string& name, AnimationData animation)
{
	std::map<std::string, AnimationData>::iterator itFind = m_Animations.find(name);
	if (itFind != m_Animations.end())
	{
#ifdef PRINT_DEBUG_INFO
		printf("Animation is already added with this name!\n");
#endif
		return false;
	}

	m_Animations.insert(std::pair<std::string, AnimationData>(name, animation));

	// Add event to handler
	Event* event = new Event;
	event->animation_name = name;
	event->status = eStatus::NONE;
	event->animation = animation;
	event_handler->events_.push_back(event);

	return true;
}

void AnimationManager::ClearAnimations()
{
	m_Animations.clear();
}

int AnimationManager::FindPositionKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.PositionKeyFrames.size(); i++)
	{
		if (animation.PositionKeyFrames[i].time > time)
			return i - 1;
	}

	// [1, 2, 3, 4, 5] 5 - 2, 3;
	return animation.PositionKeyFrames.size() - 1;
}

int AnimationManager::FindScaleKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.ScaleKeyFrames.size(); i++)
	{
		if (animation.ScaleKeyFrames[i].time > time)
			return i - 1;
	}

	// [1, 2, 3, 4, 5] 5 - 2, 3;
	return animation.ScaleKeyFrames.size() - 1;
}

int AnimationManager::FindRotationKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.RotationKeyFrames.size(); i++)
	{
		if (animation.RotationKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.RotationKeyFrames.size() - 1;
}

EasingType AnimationManager::GetEase(const AnimationData& animation, float time)
{
	// Assert animation.PositionKeyFrames.size() > 0
	EasingType type = None;
	if (animation.PositionKeyFrames.size() == 1)
		return None;

	int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, time);

	if (animation.PositionKeyFrames.size() - 1 == positionKeyFrameIndex)
		return None;

	int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
	PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
	type = positionKeyFrame.type;
	if (type == None)
	{
		if (animation.ScaleKeyFrames.size() != 1)
		{
			int scaleKeyFrameIndex = FindScaleKeyFrameIndex(animation, time);

			if (animation.ScaleKeyFrames.size() - 1 != scaleKeyFrameIndex)
			{
				int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
				ScaleKeyFrame scaleKeyFrame = animation.ScaleKeyFrames[scaleKeyFrameIndex];
				type = scaleKeyFrame.type;
			}
		}
	}
	if (type == None)
	{
		if (animation.RotationKeyFrames.size() != 1)
		{
			int rotationKeyFrameIndex = FindRotationKeyFrameIndex(animation, time);

			if (animation.RotationKeyFrames.size() - 1 != rotationKeyFrameIndex)
			{
				int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
				RotationKeyFrame rotationKeyFrame = animation.RotationKeyFrames[rotationKeyFrameIndex];
				type = rotationKeyFrame.type;
			}
		}
	}
	return type;
}

glm::vec3 AnimationManager::GetAnimationPosition(const AnimationData& animation, float time)
{
	// Assert animation.PositionKeyFrames.size() > 0

	if (animation.PositionKeyFrames.size() == 1)
		return animation.PositionKeyFrames[0].value;

	int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, time);

	if (animation.PositionKeyFrames.size() - 1 == positionKeyFrameIndex)
		return animation.PositionKeyFrames[positionKeyFrameIndex].value;

	int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
	PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
	PositionKeyFrame nextPositionKeyFrame = animation.PositionKeyFrames[nextPositionKeyFrameIndex];
	float difference = nextPositionKeyFrame.time - positionKeyFrame.time;
	float ratio = (time - positionKeyFrame.time) / difference;

	switch (positionKeyFrame.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;

	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;

	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;

	case None:
	default:
		break;
	}

	glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);

#ifdef PRINT_DEBUG_INFO
	printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
		positionKeyFrame.value.x, positionKeyFrame.value.y,
		nextPositionKeyFrame.value.x, nextPositionKeyFrame.value.y,
		result.x, result.y);
#endif

	return result;
}

glm::vec3 AnimationManager::GetAnimationScale(const AnimationData& animation, float time)
{
	// Assert animation.ScaleKeyFrames.size() > 0

	if (animation.ScaleKeyFrames.size() == 1)
		return animation.ScaleKeyFrames[0].value;

	int scaleKeyFrameIndex = FindScaleKeyFrameIndex(animation, time);

	if (animation.ScaleKeyFrames.size() - 1 == scaleKeyFrameIndex)
		return animation.ScaleKeyFrames[scaleKeyFrameIndex].value;

	int nextScaleKeyFrameIndex = scaleKeyFrameIndex + 1;
	ScaleKeyFrame scaleKeyFrame = animation.ScaleKeyFrames[scaleKeyFrameIndex];
	ScaleKeyFrame nextScaleKeyFrame = animation.ScaleKeyFrames[nextScaleKeyFrameIndex];
	float difference = nextScaleKeyFrame.time - scaleKeyFrame.time;
	float ratio = (time - scaleKeyFrame.time) / difference;

	switch (scaleKeyFrame.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;

	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;

	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;

	case None:
	default:
		break;
	}

	glm::vec3 result = glm::mix(scaleKeyFrame.value, nextScaleKeyFrame.value, ratio);

#ifdef PRINT_DEBUG_INFO
	//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
		//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
		//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
		//result.x, result.y);
#endif

	return result;
}

glm::quat AnimationManager::GetAnimationRotation(const AnimationData& animation, float time)
{
	if (animation.RotationKeyFrames.size() == 1)
		return animation.RotationKeyFrames[0].value;

	int rotationKeyFrameIndex = FindRotationKeyFrameIndex(animation, time);

	if (animation.RotationKeyFrames.size() - 1 == rotationKeyFrameIndex)
		return animation.RotationKeyFrames[rotationKeyFrameIndex].value;

	int nextRotationKeyFrameIndex = rotationKeyFrameIndex + 1;
	RotationKeyFrame rotationKeyFrame = animation.RotationKeyFrames[rotationKeyFrameIndex];
	RotationKeyFrame nextRotationKeyFrame = animation.RotationKeyFrames[nextRotationKeyFrameIndex];
	float difference = nextRotationKeyFrame.time - rotationKeyFrame.time;
	float ratio = (time - rotationKeyFrame.time) / difference;

	switch (rotationKeyFrame.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;

	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;

	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;

	case None:
	default:
		break;
	}

	glm::quat result;
	if (rotationKeyFrame.useSlerp)
		result = glm::slerp(rotationKeyFrame.value, nextRotationKeyFrame.value, ratio);
	else
		result = glm::mix(rotationKeyFrame.value, nextRotationKeyFrame.value, ratio);

#ifdef PRINT_DEBUG_INFO
	//printf("[%.2f : %.2f%%] (%.2f, %.2f) -> (%.2f, %.2f) = (%.2f, %.2f)\n", time, ratio,
		//scaleKeyFrame.value.x, scaleKeyFrame.value.y,
		//nextScaleKeyFrame.value.x, nextScaleKeyFrame.value.y,
		//result.x, result.y);
#endif

	return result;
}