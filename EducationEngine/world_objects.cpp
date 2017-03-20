#include "world_objects.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary
static uint32 ObjectInstanceIDs[4];

void InitWorldObjects()
{
	uint32 ObjectID = Object_Load(new Box, 0.25f, 0.25f, 0.25f);
	Entity_Create(EntityBlockNum, 1, ObjectID, v3(3.0f, 0.0f, 15.0f));

	ObjectID = Object_Load(new Box, 0.1f, 0.1f, 0.1f);
	Entity_Create(EntityBlockNum, 2, ObjectID, v3(2.0f, 1.0f, 17.0f)); // light
	
	ObjectID = Object_Load(new Plane2D, 10, 10);
	Entity_Create(EntityBlockNum, 3, ObjectID, v3(0.0f, -0.5f, 20.0f));
	Entity_AddTexture(EntityBlockNum, 3, Asset_GetTexture(11));
}

void DrawWorldObjects(uint32 ShaderVariableID, uint32 ShaderTextureVar)
{
	//for (uint32 i = 0; i < 4; i++)
	//{
	//	//if (GJK(GetObjectMapPtr(GetPlayer(0)->PlayerMapID), GetObjectMapPtr(ObjectMaps[i])))
	//	//{
	//	//	GetPlayer(100)->RemoveForce();
	//	//	GetPlayer(100)->AddForce(&ReflectVector(GetPlayer(0)->GetDirectionVector(), v3(1.0f, 0.0f, 0.0f)));
	//	//	GetPlayer(100)->ApplyForces();
	//	//}
	//	DrawObjectMap(ObjectMaps[i], ShaderVariableID);
	//}
	float Choice = 0.0f;
	for (uint32 Index = 1; Index < 3; Index++) // not textured
	{
		Render_UpdateShaderVariable(4, ShaderTextureVar, &Choice);
		Entity_Draw(EntityBlockNum, Index, ShaderVariableID);
	}
	Choice = 1.0f;
	for (uint32 Index = 3; Index < 4; Index++) // textured
	{
		Render_UpdateShaderVariable(4, ShaderTextureVar, &Choice);
		Entity_Draw(EntityBlockNum, Index, ShaderVariableID);
	}
}