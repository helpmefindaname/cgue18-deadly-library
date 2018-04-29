#include "GameObject.h"

struct gameObjectValues {
	//Geometry
	//UV-Coordinates
	//List? Textures
	bool moving;//if true, float movingSpeed
	bool dynamic;//if true = dynamic actor
};

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

GameObject GameObject::createNewGameObject() {

	//if !dynamic create static actor
	//if dynamic create dynamic actor
	return GameObject();

}
