#pragma once
#include "../libimport/physX.h"
class PhysicsPipeline
{
public:
	PhysicsPipeline();
	~PhysicsPipeline();

private:
	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysicsSDK = NULL;

	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;

	PxMaterial*				gMaterial = NULL;

	PxPvd*                  gPvd = NULL;

	PxReal stackZ = 10.0f;

public:

	void update();
	PxRigidDynamic* PhysicsPipeline::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));
	PxRigidStatic* PhysicsPipeline::createStatic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* mMaterial);

private:
	void PhysicsPipeline::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	void PhysicsPipeline::initPhysics(bool interactive);
	void PhysicsPipeline::stepPhysics(bool interactive);
	//void cleanupPhysics(bool interactive);
	//void keyPress(unsigned char key, const PxTransform& camera);

};



