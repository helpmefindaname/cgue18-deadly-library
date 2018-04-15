#pragma once

#include "../libimport/glm.h"
#include "../libimport/glew.h"
#include "shader.h"

	class Camera {
	public:
		Camera();
		Camera(Camera&& camera);
		Camera(Camera& camera);
		Camera(float fieldOfView, float near, float far, unsigned int viewWidth, unsigned int viewHeight);
		~Camera();

	private:
		unsigned int viewWidth;
		unsigned int viewHeight;

		float fieldOfView;
		float near;
		float far;

		glm::mat4 inverseViewMatrix;
		glm::mat4 projectionMatrix;

		bool projectionNeedsUpdate;

	public:
		void uploadData(graphics::Shader& shader);

		void uploadViewMatrix(GLint location);
		void uploadProjectionMatrix(GLint location);

		void setViewWidth(unsigned int value);
		void setViewHeight(unsigned int value);

		void setFieldOfView(float value);
		void setFar(float value);
		void setNear(float value);

		void setPosition(glm::vec3 position);
		void translateForward(float value);
		void translateBackward(float value);
		void translateRight(float value);
		void translateLeft(float value);
		void translateUp(float value);
		void translateDown(float value);

		void setRotation(glm::vec3 rotation);
		void roll(float value);
		void pitch(float value);
		void yaw(float value);

		void setInverseViewMatrix(glm::mat4 inverseViewMatrix);

		glm::vec3 getPosition() const;
		glm::mat4 getInverseViewMatrix();
		glm::mat4 getProjectionMatrix();

		void updateViewMatrix(glm::vec3 position, glm::vec3 lookDirection, glm::vec3 upVector);

	private:
		void updateProjectionMatrix();
	};