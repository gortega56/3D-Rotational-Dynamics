#include "CollisionResponse.h"
#include <glm\gtc\type_ptr.hpp>
#include <time.h>
#include <algorithm>
#include "RigidBody.h"
#include "Camera.h"
#include "Collider.h"

const float BOUNDING_BOX_SCALE = 10.0f;
const float MAX_GAMEOBJECT_SCALE = 3.0f;
const int MAX_GAMEOBJECTS = 10;
const int MAX_GAMEOBJECT_VELOCITY = 3;

CollisionResponse::CollisionResponse()
{
	gameObjectsCount = 50;
	color = {0.0f, 0.4f, 0.0f};
	collisionColor = { 0.4f, 0.0f, 0.0f };
	boundingBoxColor = { 0.0f, 0.0f, 0.0f };
	collisionController = CollisionController();
}

CollisionResponse::~CollisionResponse()
{
}

void CollisionResponse::init()
{
	Game::init();


	Geometry* cubeGeo = Geometry::Cube();

	boundingBox = new GameObject();
	boundingBox->transform = new Transform();
	boundingBox->transform->Scale = vec3(BOUNDING_BOX_SCALE);
	
	boundingBox->geometry = cubeGeo;
	boundingBox->mesh = new Mesh(*cubeGeo);
	boundingBox->mesh->RenderMode = GL_TRIANGLES;

	boundingBox->material = new Material();
	boundingBox->material->SetShaderStage("shaders/TextureVertexShader.glsl", GL_VERTEX_SHADER);
	boundingBox->material->SetShaderStage("shaders/TextureFragmentShader.glsl", GL_FRAGMENT_SHADER);
	boundingBox->material->SetShader();

	boundingBox->material->BindMeshAttributes(*boundingBox->mesh, "vertexPosition", NULL, NULL, "texCoord", NULL);
	boundingBox->material->BindUniformAttribute("clip");

	boundingBoxMin = glm::vec3(boundingBox->transform->Position - glm::vec3(0.5f)) * boundingBox->transform->Scale; // Base size * scale of the object
	boundingBoxMax = glm::vec3(boundingBox->transform->Position + glm::vec3(0.5f)) * boundingBox->transform->Scale;

	Mesh* cube = new Mesh(*cubeGeo);
	cube->RenderMode = GL_TRIANGLES;

	material = new Material();
	material->SetShaderStage("shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	material->SetShaderStage("shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	material->SetShader();

	material->BindMeshAttributes(*cube, "vertexPosition", "vertexNormal", NULL, "texCoord", NULL);
	material->BindUniformAttribute("clip");

	glActiveTexture(GL_TEXTURE0); 
	const char* woodTexture = "wood.png";
	boundingBox->material->LoadTextures2D(&woodTexture, 1);

	const char* containerTexture = "container.png";
	material->LoadTextures2D(&containerTexture, 1);

	srand(time(NULL));


	for (int i = 0; i < gameObjectsCount; i++) {
		GameObject* gameObject = new GameObject();
		gameObject->geometry = cubeGeo;
		gameObject->transform = new Transform();
		gameObject->transform->Scale *= 0.5f;
		gameObject->mesh = cube;
		gameObject->material = material;
		gameObject->rigidBody = new RigidBody();
		gameObject->rigidBody->velocity = vec3(rand() % MAX_GAMEOBJECT_VELOCITY, rand() % MAX_GAMEOBJECT_VELOCITY, rand() % MAX_GAMEOBJECT_VELOCITY);
		gameObject->rigidBody->mass = length(gameObject->transform->Scale) * 0.15f;
		gameObject->rigidBody->updateInertiaTensor(gameObject->transform->Scale * 0.5f);
		gameObject->collider = new Collider(gameObject);
		gameObjects.push_back(gameObject);
		//collisionController.OBBs.push_back(OBB());
	}

	camera = new Camera();
}

void CollisionResponse::update(double secondsElapsed)
{
	collisionController.CaptureGeometry(&gameObjects[0], gameObjects.size());
	performIntegration(secondsElapsed);
	collisionController.DispatchCollisions(&gameObjects[0], gameObjects.size());
	detectAndResolveBoundingBoxCollisions();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glUseProgram(boundingBox->material->ShaderID);

	glBindTexture(GL_TEXTURE_2D, boundingBox->material->TextureIDs[0]);
	glUniform1i(glGetUniformLocation(boundingBox->material->ShaderID, "tex1"), 0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.f);
	glm::mat4 view = camera->GetView();

	glm::mat4 PVM = projection * view * boundingBox->transform->GetWorldTransform();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(boundingBox->material->VertexArrayID);
	glUniformMatrix4fv(boundingBox->material->GetUniformAttribute("clip"), 1, GL_FALSE, &PVM[0][0]);
	glDrawArrays(boundingBox->mesh->RenderMode, 0, boundingBox->mesh->GetVertexCount());

	glUseProgram(material->ShaderID);

	glBindTexture(GL_TEXTURE_2D, material->TextureIDs[0]);
	glUniform1i(glGetUniformLocation(material->ShaderID, "tex2"), 0);
	glFrontFace(GL_CW);

	for (GameObject* gameObject : gameObjects) {
		glm::mat4 PVM = projection * view * gameObject->transform->GetWorldTransform();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(material->VertexArrayID);
		glUniformMatrix4fv(material->GetUniformAttribute("clip"), 1, GL_FALSE, &PVM[0][0]);
		glDrawArrays(gameObject->mesh->RenderMode, 0, gameObject->mesh->GetVertexCount());
	}
}

void CollisionResponse::performIntegration(double secondsElapsed)
{
	double t = 0.0;
	double dt = 0.01;

	double accumulator = 0.0;

	double newTime = secondsElapsed;

	double frameTime = newTime - currentTime;
	if (frameTime > 0.25)
	{
		frameTime = 0.25;
	}

	currentTime = newTime;
	accumulator += frameTime;

	while (accumulator >= dt)
	{
		// Previous state = currentState
		// Simulate Phyics
		
		for (GameObject *gameObject : gameObjects) {
			float deltaTime = (float)dt;
			gameObject->transform->Position += gameObject->rigidBody->velocity * deltaTime;

			quat orientation = quat(gameObject->transform->Rotation);
			quat spin = (quat(0.0f, gameObject->rigidBody->angularVelocity.x, gameObject->rigidBody->angularVelocity.y, gameObject->rigidBody->angularVelocity.z) * orientation)* 0.5f;
			orientation += (spin * deltaTime);
			orientation = normalize(orientation);
			gameObject->transform->Rotation = eulerAngles(orientation);
		}

		t += dt;
		accumulator -= dt;
	}

	const double alpha = accumulator / dt;
	// interpolate between previous phyics state and current physics state
}

void CollisionResponse::detectAndResolveBoundingBoxCollisions()
{
	for (int index = 0; index < gameObjectsCount; index++) {
		// Check if the object is outside of the box AND still continuing in the same direction
		if (gameObjects[index]->collider->min.x < boundingBoxMin.x && gameObjects[index]->rigidBody->velocity.x < 0) {
			gameObjects[index]->rigidBody->velocity.x *= -1;
		}
		else if (gameObjects[index]->collider->max.x > boundingBoxMax.x && gameObjects[index]->rigidBody->velocity.x > 0) {
			gameObjects[index]->rigidBody->velocity.x *= -1;
		}

		if (gameObjects[index]->collider->min.y < boundingBoxMin.y && gameObjects[index]->rigidBody->velocity.y < 0) {
			gameObjects[index]->rigidBody->velocity.y *= -1;
		}
		else if (gameObjects[index]->collider->max.y > boundingBoxMax.y && gameObjects[index]->rigidBody->velocity.y > 0) {
			gameObjects[index]->rigidBody->velocity.y *= -1;
		}

		if (gameObjects[index]->collider->min.z < boundingBoxMin.z && gameObjects[index]->rigidBody->velocity.z < 0) {
			gameObjects[index]->rigidBody->velocity.z *= -1;
		}
		else if (gameObjects[index]->collider->max.z > boundingBoxMax.z && gameObjects[index]->rigidBody->velocity.z > 0) {
			gameObjects[index]->rigidBody->velocity.z *= -1;
		}
	}
}

void CollisionResponse::handleEvents(GLFWwindow* window)
{
	
}