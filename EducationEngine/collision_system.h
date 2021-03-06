#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "render_object.h"
#include <list>

//TODO: GetMousePosition once per frame and store it for use in state?
//TODO: Add collision maximum distance

struct CollisionObject
{
	v3 Position{};
	v3 CollideNormal{};
	float* VerticesPtr;
	float Width, Height, Depth, HalfWidth, HalfHeight,
		HalfDepth;
	uint32 NumVertices;
	int32 CollisionCode;
};

struct AABB
{
	float Width;
	float Height;
	float Depth;
	v3 Position;
};

struct SupportPoint
{
	SupportPoint() : MinkowskiPoint(0.0f, 0.0f, 0.0f),
		ObjAPoint(0.0f, 0.0f, 0.0f),
		ObjBPoint(0.0f, 0.0f, 0.0f) {}

	~SupportPoint() {}

	v3 MinkowskiPoint;
	v3 ObjAPoint;
	v3 ObjBPoint;

	SupportPoint & operator=(SupportPoint& B)
	{
		MinkowskiPoint = B.MinkowskiPoint;
		ObjAPoint = B.ObjAPoint;
		ObjBPoint = B.ObjBPoint;
		return *this;
	}

	bool operator==(SupportPoint& B)
	{
		if (MinkowskiPoint == B.MinkowskiPoint)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator==(const SupportPoint& B)
	{
		if (MinkowskiPoint == B.MinkowskiPoint)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

void
Collision_FillObject(CollisionObject* CollObj,
	float Width, float Height, float Depth,
	v3* Position);
int
Collision_ButtonClick(v2* MousePosition, 
	CollisionObject* CollObj);
void
Collision_CreateScrollBarCollision(uint32 ScrollBarID,
	CollisionObject* UpArrow, CollisionObject* Slider,
	CollisionObject* DownArrow);
uint32
Collision_ScrollBar(uint32 ScrollBar, v2* MousePos);
float
Collision_PointDistanceFromPlane(v3* Point,
	v3* PointOnPlane, v3* Normal);
uint32
Collision_AABB(AABB* CollideA, AABB* CollideB);
// TODO: Change to OBB instead of just a point for other collide obj
// TODO: Take into account rotations.
// TODO: Find a better way to pass the Heightmap normals
bool
Collision_HeightMap(CollisionObject* HeightMapCollObj,
	float* Normals, CollisionObject* OtherCollObj);
SupportPoint
Collision_MinkowskiSupport(CollisionObject* ObjectA,
	CollisionObject* ObjectB, v3 *Direction);
bool
Collision_GJK(CollisionObject* ObjectA,
	CollisionObject* ObjectB);
v3
Collision_EPA(CollisionObject* CollisionObjA,
	CollisionObject* CollisionObjB);
#if 0

struct CollisionObject
{
	CollisionObject(): NumVertices(0), VerticesPtr(0),
		 Width(0), Height(0), Depth(0), HalfWidth(0),
		Position(0), HalfHeight(0), HalfDepth(0),
		CollisionCode(0) {}
	
	~CollisionObject() {}

	m4 *ModelMatrix;
	v3 *Position;
	v3 CollideNormal;
	float* VerticesPtr;
	float Width, Height, Depth, HalfWidth, HalfHeight,
		HalfDepth;
	uint32 NumVertices;
	int32 CollisionCode;

	CollisionObject & operator=(CollisionObject& NewObj)
	{
		NumVertices = NewObj.NumVertices;
		VerticesPtr = NewObj.VerticesPtr;
		Position = NewObj.Position;
		CollideNormal = NewObj.CollideNormal;
		Width = NewObj.Width;
		Height = NewObj.Height;
		Depth = NewObj.Depth;
		return *this;
	}
};

struct SupportPoint
{
	SupportPoint() : MinkowskiPoint(0.0f, 0.0f, 0.0f),
		ObjAPoint(0.0f, 0.0f, 0.0f),
		ObjBPoint(0.0f, 0.0f, 0.0f) {}
	
	~SupportPoint() {}

	v3 MinkowskiPoint;
	v3 ObjAPoint;
	v3 ObjBPoint;

	SupportPoint & operator=(SupportPoint& B)
	{
		MinkowskiPoint = B.MinkowskiPoint;
		ObjAPoint = B.ObjAPoint;
		ObjBPoint = B.ObjBPoint;
		return *this;
	}

	bool operator==(SupportPoint& B)
	{
		if (MinkowskiPoint == B.MinkowskiPoint) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator==(const SupportPoint& B)
	{
		if (MinkowskiPoint == B.MinkowskiPoint) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct Face
{
	Face() : OriginDistance(0) {}

	~Face() {}

	Face(SupportPoint& X, SupportPoint& Y,
		SupportPoint& Z)
	{
		Vertices[0] = X;
		Vertices[1] = Y;
		Vertices[2] = Z;
		OriginDistance = 0;
	}

	SupportPoint Vertices[3];
	v3 Normal;
	float OriginDistance;

	Face & operator=(Face& B)
	{
		Vertices[0] = B.Vertices[0];
		Vertices[1] = B.Vertices[1];
		Vertices[2] = B.Vertices[2];
		Normal = B.Normal;
		OriginDistance = B.OriginDistance;
		return *this;
	}
};

struct Edge
{
	Edge() {}

	~Edge() {}

	Edge(SupportPoint a, SupportPoint b)
	{
		A = a;
		B = b;
	}

	SupportPoint A;
	SupportPoint B;

	Edge & operator=(Edge& b)
	{
		A = b.A;
		B = b.B;
		return *this;
	}
};

int 
Collision_ButtonClick(v2* MousePosition,
	CollisionObject* Object);

bool 
Collision_OrthoMouseToRect(v2* MousePosition, 
	CollisionObject* Object);
v3 
Collision_UpdateMousePickRay(m4 *ProjectionMatrix,
	m4 *ViewMatrix);
bool
Collision_RayToOBB(v3* RayOrigin, v3* RayDirection, 
	CollisionObject* Object, float* Distance);
bool
Collision_PlaneRayIntersect(v3* RayOrigin, 
	v3* RayDirection, CollisionObject* CollideObject, 
	float* Distance);

v3 
GetFurthestPoint(CollisionObject* Object, v3 *Direction); 

SupportPoint
MinkowskiSupport(CollisionObject* ObjectA,
	CollisionObject* ObjectB, v3 *Direction);

bool 
LineSimplex(SupportPoint* PList, v3* Direction,
	uint32* PointCount);
bool
PlaneSimplex(SupportPoint* PList, v3* Direction, 
	uint32* PointCount);
bool 
TetrahedronSimplex(SupportPoint* PList, v3* Direction,
	uint32* PointCount);
bool
Collision_GJK(CollisionObject* ObjectA,
	CollisionObject* ObjectB);

Face 
AddFaceEPA(SupportPoint &v0, SupportPoint &v1, 
	SupportPoint &v2);
//Face EPA(ObjectInstance *ObjA, ObjectInstance *ObjB);
Face 
Collision_EPA(CollisionObject* ObjectA,
	CollisionObject* ObjectB);

bool 
Collision_HeightMap(
	CollisionObject* HeightMapCollisionObj,
	v3 &ObjectPosition);

v3 
Collision_GetNormal(CollisionObject* CollideObj, 
	PhysicsObject* PhysObj);

#endif // DATA_ORIENTED
#endif