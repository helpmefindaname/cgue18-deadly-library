#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();
	bool moving;//if true, float movingSpeed
	bool dynamic;

public:
	GameObject GameObject::createNewGameObject();

};

