#pragma once
#include "Module.h"
#include "Globals.h"

#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class b2World;
class b2Body;

// TODO 6: Create a small class that keeps a pointer to the b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	void CreateCircle(float x, float y, float radius)
	{
		b2BodyDef body;
		body.type = b2_dynamicBody;
		float rad = PIXEL_TO_METERS(radius);
		body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

		b2Body* b = world->CreateBody(&body);

		b2CircleShape shape;
		shape.m_radius = rad;
		b2FixtureDef fixture;
		fixture.shape = &shape;

		b->CreateFixture(&fixture);
	}

	void CreateBox(float x, float y)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 vertices[4];
		vertices[0].Set(0.0f, 0.0f);
		vertices[1].Set(2.0f, 0.0f);
		vertices[2].Set(0.0f, 1.0f);
		vertices[3].Set(2.0f, 1.0f);

		int32 count = 4;

		body.position.Set(PIXEL_TO_METERS(x) - 1.0f, PIXEL_TO_METERS(y) - 0.5f);

		b2Body* b = world->CreateBody(&body);

		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		b2PolygonShape polygon;
		polygon.Set(vertices, count);
		b2FixtureDef fixture;
		fixture.density = 1.0f;
		fixture.shape = &polygon;

		b->CreateFixture(&fixture);
	}

	void CreateChain(float x, float y)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!

		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 vs[12];
		vs[0].Set(PIXEL_TO_METERS(-38.0f), PIXEL_TO_METERS(80.0f));
		vs[1].Set(PIXEL_TO_METERS(-44.0f), PIXEL_TO_METERS(-54.0f));
		vs[2].Set(PIXEL_TO_METERS(-16.0f), PIXEL_TO_METERS(-60.0f));
		vs[3].Set(PIXEL_TO_METERS(-16.0f), PIXEL_TO_METERS(-17.0f));
		vs[4].Set(PIXEL_TO_METERS(19.0f), PIXEL_TO_METERS(-19.0f));
		vs[5].Set(PIXEL_TO_METERS(19.0f), PIXEL_TO_METERS(-79.0f));
		vs[6].Set(PIXEL_TO_METERS(61.0f), PIXEL_TO_METERS(-77.0f));
		vs[7].Set(PIXEL_TO_METERS(57.0f), PIXEL_TO_METERS(73.0f));
		vs[8].Set(PIXEL_TO_METERS(17.0f), PIXEL_TO_METERS(78.0f));
		vs[9].Set(PIXEL_TO_METERS(20.0f), PIXEL_TO_METERS(16.0f));
		vs[10].Set(PIXEL_TO_METERS(-25.0f), PIXEL_TO_METERS(13.0f));
		vs[11].Set(PIXEL_TO_METERS(-9.0f), PIXEL_TO_METERS(72.0f));

		body.position.Set(PIXEL_TO_METERS(x) - 1.0f, PIXEL_TO_METERS(y) - 0.5f);

		b2Body* b = world->CreateBody(&body);

		b2ChainShape chain;
		chain.CreateLoop(vs, 12);

		b2FixtureDef fixture;
		fixture.shape = &chain;

		b->CreateFixture(&fixture);
	}

private:

	bool debug;
	b2World* world;
};