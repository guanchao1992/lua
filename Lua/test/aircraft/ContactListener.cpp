#include "ContactListener.h"
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include "Craft.h"


namespace aircraft
{
	ContactListener::ContactListener()
	{

	}

	ContactListener:: ~ContactListener()
	{
	}

	void ContactListener::BeginContact(b2Contact* contact)
	{
		Craft *craftA = (Craft *)(contact->GetFixtureA()->GetBody()->GetUserData());
		Craft *craftB = (Craft *)(contact->GetFixtureB()->GetBody()->GetUserData());
		if (NULL != craftA)
		{
			craftA->contactCraft(craftB);
		}
		if (NULL != craftB)
		{
			craftB->contactCraft(craftA);
		}
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
	}
}
