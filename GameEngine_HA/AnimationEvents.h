#pragma once
#include <list>

#include "Animation.h"
#include "cMeshObject.h"

enum eStatus { NONE, STARTED, ONGOING, FINISHED};
struct Event
{
	eStatus status = NONE;
	AnimationData animation;
	std::string animation_name;
};
struct Watcher
{
	eStatus condition = NONE;
	AnimationData animation;
	std::string animation_name_to_watch;
	std::string animation_name_to_play;
	bool completed = false;
	cMeshObject* object;
};
class AnimationEvents
{
public:
	AnimationEvents() = default;
	~AnimationEvents() = default;

	std::list<Event*> events_;
	std::list<Watcher*> watching;
	void update(float dt);
};
