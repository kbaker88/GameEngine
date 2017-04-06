#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "physics_system.h"
#include "text_system.h"
#include <list>

//TODO: GetMousePosition once per frame and store it for use in state?

struct CollisionObject
{
	CollisionObject(): NumVertices(0), VerticesPtr(0),
		Position(0), Width(0), Height(0), Depth(0),
		HalfWidth(0), HalfHeight(0), HalfDepth(0) {}
	
	~CollisionObject() {}

	uint32 NumVertices;
	float* VerticesPtr;
	v3 *Position;
	float Width, Height, Depth, HalfWidth, HalfHeight, HalfDepth;

	CollisionObject & operator=(CollisionObject& NewObj)
	{
		NumVertices = NewObj.NumVertices;
		VerticesPtr = NewObj.VerticesPtr;
		Position = NewObj.Position;
		Width = NewObj.Width;
		Height = NewObj.Height;
		Depth = NewObj.Depth;
		return *this;
	}
};

struct SupportPoint
{
	SupportPoint() : MinkowskiPoint(0.0f, 0.0f, 0.0f),
		ObjAPoint(0.0f, 0.0f, 0.0f), ObjBPoint(0.0f, 0.0f, 0.0f) {}
	
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

	Face(SupportPoint& X, SupportPoint& Y, SupportPoint& Z)
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
		B= b.B;
		return *this;
	}
};

bool Collision_OrthoMouseToRect(v2* MousePosition, CollisionObject* Object);
v3 Collision_UpdateMousePickRay(m4 *ProjectionMatrix, m4 *ViewMatrix);
bool Collision_RayToObject(v3* RayOrigin, v3* RayDirection, CollisionObject* Object);
bool Collision_PlaneRayIntersect(v3* PlaneNormal, v3* PlaneOrigin,
	v3* RayOrigin, v3* RayDirection, float* Distance);

v3 GetFurthestPoint(CollisionObject* Object, v3 *Direction); 

SupportPoint MinkowskiSupport(CollisionObject* ObjectA,
	CollisionObject* ObjectB, v3 *Direction);

bool LineSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
bool PlaneSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
bool TetrahedronSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
bool Collision_GJK(CollisionObject* ObjectA, CollisionObject* ObjectB);

Face AddFaceEPA(SupportPoint &v0, SupportPoint &v1, SupportPoint &v2);
//Face EPA(ObjectInstance *ObjA, ObjectInstance *ObjB);
Face Collision_EPA(CollisionObject* ObjectA, CollisionObject* ObjectB);

bool Collision_HeightMap(HeightMap* HeightMapPlane, v3 &ObjectPosition);

#endif