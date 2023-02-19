#pragma once

#include <string>
#include <map>

#include "Animation.h"
#include "cMeshObject.h"
class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	bool LoadAnimation(const std::string& name, AnimationData animation);
	void ClearAnimations();
	void Update(const std::vector<cMeshObject*>& gameObjects, float dt);

private:
	int FindPositionKeyFrameIndex(const AnimationData& animation, float time);
	int FindScaleKeyFrameIndex(const AnimationData& animation, float time);
	int FindRotationKeyFrameIndex(const AnimationData& animation, float time);

	glm::vec3 GetAnimationPosition(const AnimationData& animation, float time);
	glm::vec3 GetAnimationScale(const AnimationData& animation, float time);
	glm::quat GetAnimationRotation(const AnimationData& animation, float time);
	EasingType GetEase(const AnimationData& animation, float time);

	// BallBounceAnimation will be stored here
	std::map<std::string, AnimationData> m_Animations;
};
