#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "physics_system.h"
#include "text_system.h"
#include <list>

struct CollisionObject
{
	CollisionObject(): NumVertices(0), VerticesPtr(NULL),
		Position(NULL), Width(0), Height(0), Depth(0) {}
	
	~CollisionObject() {}

	uint32 NumVertices;
	float* VerticesPtr;
	v3 *Position;
	float Width, Height, Depth;

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

void FillCollisionObject(CollisionObject* Object, uint32 NumVertices,
	float* VerticesPtr, v3 &ObjPosition, float Width, float Height,
	float Depth);

struct SupportPoint
{
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
		if (MinkowskiPoint == B.MinkowskiPoint) //&&
			//(ObjAPoint == B.ObjAPoint) &&
			//(ObjBPoint == B.ObjBPoint))
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
		if (MinkowskiPoint == B.MinkowskiPoint) //&&
			//(ObjAPoint == B.ObjAPoint) &&
			//(ObjBPoint == B.ObjBPoint))
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
	Face()
	{
		OriginDistance = 0;
	}
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

// Global Functions
uint8 GetClickState(); // Does this belong in Collision?
v2 GetOrthoMousePosition(); // Does this belong in Collision?

bool Collision_OrthoMouseToRect(v3 &ObjectPosition, float ObjWidth, float ObjHeight);
v3 Collision_UpdateMousePickRay(m4 *ProjectionMatrix, m4 *ViewMatrix);
bool Collision_RayToObject(v3* Ray, CollisionObject* Object);

// Entity Only Functions
//v3 GetFurthestPoint(ObjectInstance *ObjA, v3 &Direction);
v3 GetFurthestPoint(CollisionObject* Object, v3 &Direction); // Move this to ObjInstance?

//v3 GJKSupport(ObjectInstance *ObjA, ObjectInstance *ObjB, v3 &Direction);
//SupportPoint MinkowskiSupport(ObjectInstance *ObjA, ObjectInstance *ObjB, v3 &Direction);
SupportPoint MinkowskiSupport(CollisionObject* ObjectA,
	CollisionObject* ObjectB, v3 &Direction);

bool LineSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
bool PlaneSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
bool TetrahedronSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount);
//bool GJK(ObjectInstance *ObjA, ObjectInstance *ObjB);
bool Collision_GJK(CollisionObject* ObjectA, CollisionObject* ObjectB);

Face AddFaceEPA(SupportPoint &v0, SupportPoint &v1, SupportPoint &v2);
//Face EPA(ObjectInstance *ObjA, ObjectInstance *ObjB);
Face Collision_EPA(CollisionObject* ObjectA, CollisionObject* ObjectB);

bool Collision_HeightMap(HeightMap* HeightMapPlane, v3 &ObjectPosition);

#endif