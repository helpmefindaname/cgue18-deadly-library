#include "PhysicsPipeline.h"
#include <stdexcept>


PhysicsPipeline::PhysicsPipeline()
	:gAllocator(),
	gErrorCallback()
{
	this->initPhysics();
}

PxRigidDynamic* PhysicsPipeline::createDynamic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* material, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*this->gPhysicsSDK, t, geometry, *material, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}


void PhysicsPipeline::createStaticCube(const PxVec3 position, const PxVec3 size) {
	PxShape* shape = gPhysicsSDK->createShape(PxBoxGeometry(size.x, size.y, size.z), *gNoReboundMaterial);

	PxTransform localTm(position);
	PxRigidStatic* cube = gPhysicsSDK->createRigidStatic(localTm);
	cube->attachShape(*shape);
	gScene->addActor(*cube);
	shape->release();
}

PxRigidStatic* PhysicsPipeline::createStatic(const PxTransform& t, const PxGeometry& geometry, const PxMaterial* mMaterial)
{
	// Create static actor
	//
	// 1- creating static plane
	PxTransform planePos = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(planePos);
	plane->createShape(PxPlaneGeometry(), *mMaterial);
	gScene->addActor(*plane);
	return plane;
}

void PhysicsPipeline::initPhysics()
{
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	//Creating instance of PhysX SDK
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	if (gPhysicsSDK == nullptr) {
		throw std::runtime_error("gPhysicsSDK initialization failed");
	}

	// Create scene
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysicsSDK->createScene(sceneDesc);
	/* Think about:
		// Triggers = obj which can collide, but do not influence other objects (eg automatic door opener)
		// Exclude shape from simulation and mark it as a trigger shape
		// Setting trigger flag for 'triggerShape':
		//
		PxShape* triggerShape = nullptr;
		triggerShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //bei Collision kein Abprallen
		triggerShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // bei Collision erhalten einer Nachricht
		*/
	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	gNoReboundMaterial = gPhysicsSDK->createMaterial(0.0f, 0.0f, 1.0f);
}

void PhysicsPipeline::update(float time)
{
	gScene->simulate(time);
	gScene->fetchResults(true);
}

PhysicsPipeline::~PhysicsPipeline()
{
	// free resources and destroy all actors
	gScene->release();
	gPhysicsSDK->release();
	gDispatcher->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	gFoundation->release();
}
