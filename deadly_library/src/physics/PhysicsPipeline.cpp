#include "PhysicsPipeline.h"
#include <stdexcept>


PhysicsPipeline::PhysicsPipeline()
{

}

/*RigidDynamic* PhysicsPipeline::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*this->gPhysicsSDK, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}*/
PxRigidStatic* PhysicsPipeline::createStatic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* mMaterial)
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

void PhysicsPipeline::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysicsSDK->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i<size; i++)
	{
		for (PxU32 j = 0; j<size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysicsSDK->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

void PhysicsPipeline::initPhysics(bool interactive)
{
	// Creating foundation for PhysX
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

	// Creating material
	gMaterial = gPhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);	// static friction, dynamic friction, restitution
	
	// Static ground plane
	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysicsSDK, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);
/*
	for (PxU32 i = 0; i<5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
	*/
	// Character controller - gravity has to be implemented manually!
}

void PhysicsPipeline::stepPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

/* PX_UNUSED ? wichtig?
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->release();
	gDispatcher->release();
	gPhysicsSDK->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	printf("SnippetHelloWorld done.\n");
}*/
/* wo anders hin
void keyPress(unsigned char key, const PxTransform& camera)
{
	switch (toupper(key))
	{
	case 'B':	createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);						break;
	case ' ':	createDynamic(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0, 0, -1)) * 200);	break;
	}
}

int snippetMain(int, const char*const*)
{
#ifdef RENDER_SNIPPET
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
*/
	

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
	gDispatcher->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	gFoundation->release();
}
