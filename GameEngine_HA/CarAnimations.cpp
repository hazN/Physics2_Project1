#include "CarAnimations.h"
#include "Animation.h"
#include <GLFW/glfw3.h>

#include "globalThings.h"
#include "AnimationManager.h"

void CarAnimations::Initialize()
{
	LoadAnimations();
}
std::string AniType[4] = { "Enter", "Drive", "GetOut", "RunOver"};
int index = 0;
void CarAnimations::Update(float dt)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		if (++index == 3)
			index = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		if (--index == 0)
			index = 3;
	}
}

void CarAnimations::LoadAnimations()
{
	AnimationData car_Animation1_EaseInOut;
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 0), 0.0f, EaseIn));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 50), 1.f, None));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 75), 1.5f, None));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 100), 2.f, None));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 125), 2.5f, None));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 150), 3.f, EaseOut));
	car_Animation1_EaseInOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 10, 200), 4.0f));
	car_Animation1_EaseInOut.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.0f), 0.0f));
	car_Animation1_EaseInOut.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(1, 0, 0, 0), 0.0f, None, true));
	car_Animation1_EaseInOut.Duration = 4.f;
	animation_manager->LoadAnimation("car_Animation1_EaseInOut", car_Animation1_EaseInOut);

	AnimationData beholder_Animation1_EaseIn;
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(50, 15, 0), 0.0f, EaseIn));
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15, 0), 0.2f, EaseIn));
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(30, 15, 0), 0.4f, EaseIn));
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(20, 15, 0), 0.6f, EaseIn));
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(10, 15, 0), 0.8f, EaseIn));
	beholder_Animation1_EaseIn.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 15, 0), 1.0f));
	beholder_Animation1_EaseIn.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.0f), 0.0f));
	beholder_Animation1_EaseIn.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.0f), 0.5f));
	beholder_Animation1_EaseIn.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0.0f), 0.55f));
	beholder_Animation1_EaseIn.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, 90, 0)), 0.0f, None, true));
	beholder_Animation1_EaseIn.Duration = 1.0f;
	animation_manager->LoadAnimation("beholder_Animation1_EaseIn", beholder_Animation1_EaseIn);

	AnimationData beholder_ExitCar_EaseOut;
	beholder_ExitCar_EaseOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0, 15, 200), 0.0f, EaseOut));
	beholder_ExitCar_EaseOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(10, 15, 200), 0.25f, EaseOut));
	beholder_ExitCar_EaseOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(20, 15, 200), 0.5f, EaseOut));
	beholder_ExitCar_EaseOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(30, 15, 200), 0.75f, EaseOut));
	beholder_ExitCar_EaseOut.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15, 200), 1.0f));
	beholder_ExitCar_EaseOut.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0.0f), 0.0f));
	beholder_ExitCar_EaseOut.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.0f), 0.5f));
	beholder_ExitCar_EaseOut.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, glm::radians(270.f), 0)), 0.0f, None, true));
	beholder_ExitCar_EaseOut.Duration = 5.0f;
	animation_manager->LoadAnimation("beholder_ExitCar_EaseOut", beholder_ExitCar_EaseOut);

	AnimationData SecondCar_NoEase;
	SecondCar_NoEase.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-200.f, 10.f, 250.f), 0.f));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-150.f, 10.f, 250.f), 1.f));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-100.f, 10.f, 250.f), 2.f, EaseIn));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-50.f, 10.f, 250.f), 3.f));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(0.f, 10.f, 250.f), 4.f));
	SecondCar_NoEase.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(50.f, 10.f, 250.f), 5.f));
	SecondCar_NoEase.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0)), 0.0f, EaseIn, true));
	SecondCar_NoEase.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, glm::radians(90.f), 0)), 5.f, None, true));
	SecondCar_NoEase.Duration = 5.f;
	animation_manager->LoadAnimation("SecondCar_NoEase", SecondCar_NoEase);

	AnimationData Beholder1_Eat;
	Beholder1_Eat.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0.f), 0.f, EaseIn));
	Beholder1_Eat.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 2.f));
	Beholder1_Eat.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 4.f, EaseOut));
	Beholder1_Eat.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 5.f));
	Beholder1_Eat.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15.f, 250.f), 0.f, EaseInOut));
	Beholder1_Eat.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 15.f, 225.f), 2.f, None));
	Beholder1_Eat.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 15.f, 200.f), 4.f, None));
	Beholder1_Eat.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 20.f, 200.f), 8.f, EaseOut));
	Beholder1_Eat.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0)), 0.0f, EaseInOut, true));
	Beholder1_Eat.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0)), 4.f, EaseInOut ,true));
	Beholder1_Eat.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(glm::radians(90.f),0,0)), 5.f, EaseInOut, true));
	Beholder1_Eat.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0,0,0)), 8.f, EaseInOut, true));
	Beholder1_Eat.Duration = 8.f;
	animation_manager->LoadAnimation("Beholder1_Eat", Beholder1_Eat);

	AnimationData Beholder_Disappear;
	Beholder_Disappear.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0.f), 0.f));
	Beholder_Disappear.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0.f), 1.f));
	Beholder_Disappear.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15, 200), 0.0f));
	Beholder_Disappear.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15, 200), 1.0f));
	Beholder_Disappear.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0)), 0.0f, EaseOut, true));
	Beholder_Disappear.Duration = 1.f;
	animation_manager->LoadAnimation("Beholder_Disappear", Beholder_Disappear);

	AnimationData Beholder_GetBackInCar;
	Beholder_GetBackInCar.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f));
	Beholder_GetBackInCar.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 2.f));
	Beholder_GetBackInCar.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(5.f), 4.f));
	Beholder_GetBackInCar.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(0), 5.f));
	Beholder_GetBackInCar.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40, 15.f, 200.f), 0.f, EaseIn));
	Beholder_GetBackInCar.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 15.f, 200.f), 1.f, EaseIn));
	Beholder_GetBackInCar.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 15.f, 225.f), 2.f, None));
	Beholder_GetBackInCar.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(40.f, 20.f, 250.f), 4.f, EaseOut));
	Beholder_GetBackInCar.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, 0, 0)), 0.f, None, true));
	Beholder_GetBackInCar.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, glm::radians(180.f), 0)), 5.f, None, true));
	Beholder_GetBackInCar.Duration = 5.f;
	animation_manager->LoadAnimation("Beholder_GetBackInCar", Beholder_GetBackInCar);

	AnimationData SecondCar_DriveOff;
	SecondCar_DriveOff.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(10.f), 0.f));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(50.f, 10.f, 250.f), 0.f, EaseIn));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(100.f, 10.f, 250.f), 1.f));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(150.f, 10.f, 250.f), 2.f));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(200.f, 10.f, 250.f), 3.f));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(250.f, 10.f, 250.f), 4.f, EaseOut));
	SecondCar_DriveOff.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(300.f, 10.f, 250.f), 5.f));
	SecondCar_DriveOff.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(glm::vec3(0, glm::radians(90.f), 0)), 0.0f, None, true));
	SecondCar_DriveOff.Duration = 5.f;
	animation_manager->LoadAnimation("SecondCar_DriveOff", SecondCar_DriveOff);
}