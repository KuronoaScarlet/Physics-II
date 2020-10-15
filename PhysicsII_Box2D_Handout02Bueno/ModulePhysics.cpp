#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

void ModulePhysics::CreateCircle(float x, float y, float radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	float rad = PIXEL_TO_METERS(radius);
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape circle;
	circle.m_radius = rad;
	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	

	b->CreateFixture(&fixture);
}

void ModulePhysics::CreateBox(float x, float y)
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

void ModulePhysics::CreateChain(float x, float y)
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

void PhysBody::GetPosition(int&x, int&y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}
