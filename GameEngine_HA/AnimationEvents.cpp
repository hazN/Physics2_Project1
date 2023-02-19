#include "AnimationEvents.h"

void AnimationEvents::update(float dt)
{
	if (watching.empty())
	{
		return;
	}
	for (std::list<Watcher*>::iterator watcher = watching.begin(); watcher != watching.end(); ++watcher)
	{
		for (Event* event : events_)
		{
			if ((*watcher)->animation_name_to_watch == event->animation_name)
			{
				if (event->status == (*watcher)->condition)
				{
					(*watcher)->completed = true;
					(*watcher)->object->Animation.AnimationType = (*watcher)->animation_name_to_play;
					(*watcher)->object->Animation.IsPlaying = true;
					(*watcher)->object->Enabled = true;
					(*watcher)->object->Animation.AnimationTime = 0.0f;
					watcher = watching.erase(watcher);
					if (watching.empty())
					{
						return;
					}
				}
			}
		}
	}
}