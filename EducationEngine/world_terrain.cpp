#include "world_terrain.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary
static HeightMap HeightMapPlane;

void InitWorldTerrain()
{
	HeightMapPlane.Init(Asset_GetTexture(15));
	HeightMapPlane.InputTexture(Asset_GetTexture(3));
}

void DrawWorldTerrain(uint32 ShaderVariable)
{
	m4 ModelMatrix = IdentityMatrix();
	void* Player1 = GetPlayer(0);

	Render_UpdateShaderVariable(3, ShaderVariable,
		(float*)&ModelMatrix);
	HeightMapPlane.DrawIndices();

	v3 Position = 
		Entity_GetPosition(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID);

	if ((Position.x < 1.0f) ||
		(Position.x > (float)HeightMapPlane.TerrainMaxWidth - 1) ||
		(Position.z > -1.0f) ||
		(Position.z < -(float)HeightMapPlane.TerrainMaxDepth + 1))
	{
		// outside of terrain map
		if (Position.y < 0)
		{
			Position.y = 0;
			Phys_AddForce(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
				&(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
				&v3(0.0f, 
					-Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID)->Acceleration.y, 0.0f));
			Entity_SetPosition(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID, Position);
		}
	}
	else
	{
		int32 x, z;
		x = (int32)round(Position.x) - 1; // because negative is backwards?
		z = (int32)round(Position.z);

		// Test Code
		uint32 PlayerPosition = ((x * HeightMapPlane.TerrainMaxDepth) +
			(HeightMapPlane.TerrainMaxDepth - z)) * 3 * sizeof(float);

		HeightMapPlane.UpdateColorVertice(HeightMapPlane.ObjectDescription, 
			PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();

		if (Collision_HeightMap(&HeightMapPlane, Position))
		{
			Phys_AddForce(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
				&(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
				&v3(0.0f, 
					-Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID)->Acceleration.y, 0.0f));
			Entity_SetPosition(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID, Position);
		}
	}
}