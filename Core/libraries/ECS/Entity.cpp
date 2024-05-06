#include "Entity.h"
namespace Zero
{
	Entity::Entity(int custom_id)
	{
		static unsigned int newId = 0;
		if(custom_id!=-1)
			Id = (unsigned int)custom_id;
		else
			Id = ++newId;
	}
}