#include "PhysicsPipeline.h"


PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysicsSDK = nullptr;
PxScene* gScene = nullptr;

PhysicsPipeline::PhysicsPipeline()
{
	
}

void PhysicsPipeline::init(bool interactive)
{	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	
	// Creating foundation for PhysX
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	
	//Creating instance of PhysX SDK
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
	if (gPhysicsSDK == nullptr) {
	//ERROR
	}
	
	// Create scene
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysicsSDK->createScene(sceneDesc);

	// Creating material
	PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.5, 0.5, 0.5); // static friction, dynamic friction, restitution

	// Create static actor
	//
	// 1- creating static plane
	PxTransform planePos = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(planePos);
	plane->createShape(PxPlaneGeometry(), *mMaterial);
	gScene->addActor(*plane);

	// Triggers = obj which can collide, but do not influence other objects (eg automatic door opener)
	// Exclude shape from simulation and mark it as a trigger shape
	// Setting trigger flag for 'triggerShape':
	PxShape* triggerShape = nullptr;
	triggerShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	triggerShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	/*physx::PxPvd* gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	for (PxU32 i = 0; i<5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
*/
	
	// Character controller - gravity has to be implemented manually!


}
// Important: always simulate with a timestep of 1/60!
//– Accumulate dt until it is >= 1 / 60 ? update with 1 / 60
//– That requires a framerate of at least 60 fps
//(hard requirement!!!)
void PhysicsPipeline::update()
{
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

PhysicsPipeline::~PhysicsPipeline()
{
	// free resources and destroy all actors
	gScene->release();
	gPhysicsSDK->release();
	gFoundation->release();
}
