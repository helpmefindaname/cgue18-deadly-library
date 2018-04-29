#include "../libimport/glm.h"
#include "../input/inputHandler.h"

class Player {
public:
	Player();
	~Player();

private:
	glm::vec3 position;
	float direction;
	float speed;
	

public:
	glm::vec3 getPosition();
	void update(InputHandler& inputHandler);


private:

};
