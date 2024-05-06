#pragma once
#include <iostream>
namespace Zero
{
	class Entity {
	public:
		Entity(int custom_id=-1);
		unsigned int Id;
		void dos()
		{
			std::cout<<"Hola";
		}
	};	
}
