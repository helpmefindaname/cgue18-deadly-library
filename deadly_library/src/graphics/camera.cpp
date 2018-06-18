#include "camera.h"
#include "../config.h"
#include "../libimport/glew.h"
#include "../Globals.h"

Camera::Camera() :
	viewWidth(1280),
	viewHeight(720),
	fieldOfView(60),
	near(0.1f),
	far(1000.0f),
	projectionNeedsUpdate(true)
{
	this->viewWidth = Config::getInt("WindowWidth");
	this->viewHeight = Config::getInt("WindowHeight");
	this->fieldOfView = Config::getFloat("FieldOfView");
	this->near = Config::getFloat("NearPlane");
	this->far = Config::getFloat("FarPlane");

	this->updateProjectionMatrix();
}

Camera::Camera(Camera&& camera) :
	viewWidth(camera.viewWidth),
	viewHeight(camera.viewHeight),
	fieldOfView(camera.fieldOfView),
	near(camera.near),
	far(camera.far),
	inverseViewMatrix(std::move(camera.inverseViewMatrix)),
	projectionMatrix(std::move(camera.projectionMatrix)),
	projectionNeedsUpdate(camera.projectionNeedsUpdate)
{ }

Camera::Camera(Camera& camera) :
	viewWidth(camera.viewWidth),
	viewHeight(camera.viewHeight),
	fieldOfView(camera.fieldOfView),
	near(camera.near),
	far(camera.far),
	inverseViewMatrix(camera.inverseViewMatrix),
	projectionMatrix(camera.projectionMatrix),
	projectionNeedsUpdate(camera.projectionNeedsUpdate)
{}

Camera::~Camera() {}

Camera::Camera(float fieldOfView, float near, float far, unsigned int viewWidth, unsigned int viewHeight) :
	viewWidth(viewWidth),
	viewHeight(viewHeight),
	fieldOfView(fieldOfView),
	near(near),
	far(far),
	projectionNeedsUpdate(true)
{
	this->updateProjectionMatrix();
}

void Camera::uploadData(Shader& shader) {
	if (this->projectionNeedsUpdate) {
		this->updateProjectionMatrix();
	}

	glm::mat4 viewProjectionMatrix = this->projectionMatrix * this->inverseViewMatrix;
	shader.setUniform("viewProjectionMatrix", viewProjectionMatrix);
	shader.setUniform("camera_world", getPosition());
}

void Camera::uploadViewMatrix(GLint location) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &this->inverseViewMatrix[0][0]);
}

void Camera::uploadProjectionMatrix(GLint location) {
	if (this->projectionNeedsUpdate) {
		this->updateProjectionMatrix();
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, &this->projectionMatrix[0][0]);
}

void Camera::setViewWidth(unsigned int value) {
	this->viewWidth = value;

	this->projectionNeedsUpdate = true;
}

void Camera::setViewHeight(unsigned int value) {
	this->viewHeight = value;

	this->projectionNeedsUpdate = true;
}

void Camera::setFieldOfView(float value) {
	this->fieldOfView = value;

	this->projectionNeedsUpdate = true;
}

void Camera::setFar(float value) {
	this->far = value;

	this->projectionNeedsUpdate = true;
}

void Camera::setNear(float value) {
	this->near = value;

	this->projectionNeedsUpdate = true;
}

void Camera::setPosition(glm::vec3 position) {
	inverseViewMatrix = glm::translate(glm::mat4(), position - getPosition()) * inverseViewMatrix;
}

void Camera::translateForward(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, value)) * inverseViewMatrix;
}

void Camera::translateBackward(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -value)) * inverseViewMatrix;
}

void Camera::translateRight(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(-value, 0.0f, 0.0f)) * inverseViewMatrix;
}

void Camera::translateLeft(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(value, 0.0f, 0.0f)) * inverseViewMatrix;
}

void Camera::translateUp(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -value, 0.0f)) * inverseViewMatrix;
}

void Camera::translateDown(float value) {
	inverseViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, value, 0.0f)) * inverseViewMatrix;
}

void Camera::setFrustumPlanes()
{
	std::vector<glm::vec4> frustumPlanes(6);

	glm::mat4 viewProjection = this->getProjectionMatrix() * this->getInverseViewMatrix();

	// right
	frustumPlanes[0].x = viewProjection[0][3] + viewProjection[0][0];
	frustumPlanes[0].y = viewProjection[1][3] + viewProjection[1][0];
	frustumPlanes[0].z = viewProjection[2][3] + viewProjection[2][0];
	frustumPlanes[0].w = viewProjection[3][3] + viewProjection[3][0];

	// left
	frustumPlanes[1].x = viewProjection[0][3] - viewProjection[0][0];
	frustumPlanes[1].y = viewProjection[1][3] - viewProjection[1][0];
	frustumPlanes[1].z = viewProjection[2][3] - viewProjection[2][0];
	frustumPlanes[1].w = viewProjection[3][3] - viewProjection[3][0];

	// top
	frustumPlanes[2].x = viewProjection[0][3] - viewProjection[0][1];
	frustumPlanes[2].y = viewProjection[1][3] - viewProjection[1][1];
	frustumPlanes[2].z = viewProjection[2][3] - viewProjection[2][1];
	frustumPlanes[2].w = viewProjection[3][3] - viewProjection[3][1];

	// bottom
	frustumPlanes[3].x = viewProjection[0][3] + viewProjection[0][1];
	frustumPlanes[3].y = viewProjection[1][3] + viewProjection[1][1];
	frustumPlanes[3].z = viewProjection[2][3] + viewProjection[2][1];
	frustumPlanes[3].w = viewProjection[3][3] + viewProjection[3][1];

	// far
	frustumPlanes[4].x = viewProjection[0][2];
	frustumPlanes[4].y = viewProjection[1][2];
	frustumPlanes[4].z = viewProjection[2][2];
	frustumPlanes[4].w = viewProjection[3][2];

	// near
	frustumPlanes[5].x = viewProjection[0][3] - viewProjection[0][2];
	frustumPlanes[5].y = viewProjection[1][3] - viewProjection[1][2];
	frustumPlanes[5].z = viewProjection[2][3] - viewProjection[2][2];
	frustumPlanes[5].w = viewProjection[3][3] - viewProjection[3][2];

	Globals::frustumPlanes = frustumPlanes;
}

void Camera::roll(float value) {
	inverseViewMatrix = glm::rotate(glm::mat4(), -value, glm::vec3(0.0f, 0.0f, 1.0f)) * inverseViewMatrix;
}

void Camera::pitch(float value) {
	inverseViewMatrix = glm::rotate(glm::mat4(), -value, glm::vec3(1.0f, 0.0f, 0.0f)) * inverseViewMatrix;
}

void Camera::yaw(float value) {
	inverseViewMatrix = glm::rotate(glm::mat4(), -value, glm::vec3(0.0f, 1.0f, 0.0f)) * inverseViewMatrix;
}

glm::vec3 Camera::getPosition() const {
	glm::vec4 zero(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 result = inverse(this->inverseViewMatrix) * zero;
	return result;
}

void Camera::setInverseViewMatrix(glm::mat4 inverseViewMatrix) {
	this->inverseViewMatrix = inverseViewMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() {
	return this->inverseViewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
	return this->projectionMatrix;
}

void Camera::updateViewMatrix(glm::vec3 position, glm::vec3 lookDirection, glm::vec3 upVector) {

	this->inverseViewMatrix = Glm::lookAt(
		position,
		position + lookDirection,
		upVector
	);
}

void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(
		glm::radians<float>(float(this->fieldOfView)),
		float(this->viewWidth) / float(this->viewHeight),
		this->near,
		this->far
	);

	this->projectionNeedsUpdate = false;
}