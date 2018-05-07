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

	PxPvd*                  gPvd = NULL;

	PxControllerManager*    gControllerManager = NULL;

	PxMaterial*             gNoReboundMaterial;

public:

	void update(float time);
	void PhysicsPipeline::createStaticCube(const PxVec3 position, const PxVec3 = PxVec3(0.5f));
	PxController* PhysicsPipeline::createController(const PxVec3 position, const PxVec3 size);


	PxRigidDynamic* PhysicsPipeline::createDynamic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* material, const PxVec3& velocity = PxVec3(0));
	PxRigidStatic* PhysicsPipeline::createStatic(const PxTransform& t, const PxGeometry& geometry, const PxMaterial* mMaterial);

private:
	void PhysicsPipeline::initPhysics();
};



