#pragma once
#include "Box2D/Common/b2Draw.h"
#include <Box2D/Dynamics/b2WorldCallbacks.h>


namespace aircraft
{
	class ContactListener : public b2ContactListener
	{
	public:
		ContactListener();
		~ContactListener();
	public:
		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);

	private:
	};
}
