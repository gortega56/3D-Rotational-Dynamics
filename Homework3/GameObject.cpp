#include "GameObject.h"

using namespace glm;

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	delete(transform);
	delete(mesh);
	delete(rigidBody);
	delete(material);
}

