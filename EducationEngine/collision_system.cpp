#include "collision_system.h"

void
Collision_FillObject(CollisionObject* CollObj, float Width,
	float Height, float Depth,  v3* Position)
{
	CollObj->Width = Width;
	CollObj->Height = Height;
	CollObj->Depth = Depth;
	CollObj->HalfWidth = Width * 0.5f;
	CollObj->HalfHeight = Height * 0.5f;
	CollObj->HalfDepth = Depth * 0.5f;
	CollObj->Position = *Position;
	CollObj->CollisionCode = 0;
}

int
Collision_ButtonClick(v2* MousePosition, 
	CollisionObject* CollObj)
{
	// TODO: Remove mouse position calculations from here,
	// it should be pre-calculated because it can only change
	// once per frame.
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	v2 NewMousePosition;
	NewMousePosition.x =
		MousePosition->x - 
		((float)WindowDimensions.Width * 0.5f);
	NewMousePosition.y =
		((float)WindowDimensions.Height * 0.5f) - 
		MousePosition->y;

	if ((NewMousePosition.x > CollObj->Position.x) &&
		(NewMousePosition.x < (CollObj->Position.x + 
			CollObj->Width)))
	{
		if ((NewMousePosition.y > CollObj->Position.y) &&
			(NewMousePosition.y < (CollObj->Position.y +
				CollObj->Height)))
		{
			if (Platform_GetMouseState())
			{
				return CollObj->CollisionCode + 2;
			}
			return CollObj->CollisionCode + 1;
		}
	}
	return CollObj->CollisionCode;
}

// NOTE: Finds the smallest x,z vertice point in a quad that the
// colliding object is in, determines which of the two triangles
// of the quad that the object lies using (x1 - x0)(y2 - y0) - 
// (x2 - x0)(y1 - y0) adjusted to the origin. Sets the normal
// value and confirms collision.
bool 
Collision_HeightMap(CollisionObject* HeightMapCollObj,
	float* Normals, CollisionObject* OtherCollObj)
{
	v3 RelativePos = OtherCollObj->Position -
		HeightMapCollObj->Position;

	if ((RelativePos.x >= 0) &&
		(RelativePos.z <= 0) &&
		(RelativePos.x < HeightMapCollObj->Width - 1) &&
		(RelativePos.z > -HeightMapCollObj->Depth + 1))
	{
		float XFloor = (float)((int32)RelativePos.x);
		float ZFloor = (float)((int32)RelativePos.z);

		float BottomLeft =
			(XFloor * (HeightMapCollObj->Depth - 1)) - 
			ZFloor;

		bool FlipX = (uint32)XFloor % 2;
		bool FlipZ = (uint32)ZFloor % 2;

		float XPosOrigin = Math_AbsVal(RelativePos.x) -
			(float)Math_AbsVal(XFloor);
		float ZPosOrigin = Math_AbsVal(RelativePos.z) - 
			(float)Math_AbsVal(ZFloor);
		
		uint32 Position = (uint32)BottomLeft * 18;

		v3 RightTopVertice(
			HeightMapCollObj->VerticesPtr[Position + 9],
			HeightMapCollObj->VerticesPtr[Position + 10],
			HeightMapCollObj->VerticesPtr[Position + 11]);

		v3 RightBottomVertice(
			HeightMapCollObj->VerticesPtr[Position + 12],
			HeightMapCollObj->VerticesPtr[Position + 13],
			HeightMapCollObj->VerticesPtr[Position + 14]);

		v3 LeftTopVertice(
			HeightMapCollObj->VerticesPtr[Position + 3],
			HeightMapCollObj->VerticesPtr[Position + 4],
			HeightMapCollObj->VerticesPtr[Position + 5]);

		v3 LeftBottomVertice(
			HeightMapCollObj->VerticesPtr[Position + 0],
			HeightMapCollObj->VerticesPtr[Position + 1],
			HeightMapCollObj->VerticesPtr[Position + 2]);

		if ((FlipX && !FlipZ) || (!FlipX && FlipZ))
		{
			float Side = ZPosOrigin - 1.0f + XPosOrigin;
			if (Side < 0)
			{
				// NOTE: Left Side
				HeightMapCollObj->CollideNormal.x =
					Normals[Position + 0];
				HeightMapCollObj->CollideNormal.y =
					Normals[Position + 1];
				HeightMapCollObj->CollideNormal.z = 
					Normals[Position + 2];
				return 1;
			}
			else
			{
				// NOTE: Right Side
				HeightMapCollObj->CollideNormal.x =
					Normals[Position + 9];
				HeightMapCollObj->CollideNormal.y = 
					Normals[Position + 10];
				HeightMapCollObj->CollideNormal.z =
					Normals[Position + 11];
				return 1;
			}
		}
		else
		{ 
			float Side = ZPosOrigin - XPosOrigin;
			if (Side > 0)
			{
				// NOTE: Left Side
				HeightMapCollObj->CollideNormal.x =
					Normals[Position + 3];
				HeightMapCollObj->CollideNormal.y =
					Normals[Position + 4];
				HeightMapCollObj->CollideNormal.z = 
					Normals[Position + 5];
				return 1;
			}
			else
			{
				// NOTE: Right Side
				HeightMapCollObj->CollideNormal.x = 
					Normals[Position + 12];
				HeightMapCollObj->CollideNormal.y =
					Normals[Position + 13];
				HeightMapCollObj->CollideNormal.z =
					Normals[Position + 14];
				return 1;
			}
		}
	}
	return 0;
}


v3
GetFurthestPoint(CollisionObject* Object, v3 *Direction)
{
	v3 FurthestPoint = { 0.0f, 0.0f, 0.0f };
	float MaxDot = -100000.0f;

	// NOTE: This is checking all points twice, fix?
	for (uint32 i = 0; i < Object->NumVertices; i++)
	{
		v3 Vertex = { Object->VerticesPtr[i * 3],
			Object->VerticesPtr[i * 3 + 1],
			Object->VerticesPtr[i * 3 + 2] };
		Vertex += Object->Position;
		float Dot = Math_InnerProduct(&Vertex, Direction);

		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			FurthestPoint = Vertex;
		}
	}

	return FurthestPoint;
}

SupportPoint
Collision_MinkowskiSupport(CollisionObject* ObjectA,
	CollisionObject* ObjectB, v3 *Direction)
{
	SupportPoint Result;
	Result.ObjAPoint = GetFurthestPoint(ObjectA, Direction);
	Result.ObjBPoint = GetFurthestPoint(ObjectB, &(-*Direction));
	Result.MinkowskiPoint = Result.ObjAPoint - Result.ObjBPoint;

	// NOTE: Max point in Direction.
	return Result;
}

bool
GJK_LineSimplex(SupportPoint* PList, v3* Direction, 
	uint32* PointCount)
{
	SupportPoint A = PList[1];
	SupportPoint B = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;

	if (Math_InnerProduct(&AB, &AO) > 0)
	{
		*Direction = 
			Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
		*PointCount = 2;
		return true;
	}
	else
	{
		*Direction = AO;
		PList[0] = A;
		*PointCount = 1;
		return false;
	}
}

bool
GJK_PlaneSimplex(SupportPoint* PList, v3* Direction,
	uint32* PointCount)
{
	SupportPoint A = PList[2];
	SupportPoint B = PList[1];
	SupportPoint C = PList[0];
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = Math_CrossProduct(AB, AC);

	if (Math_InnerProduct(&Math_CrossProduct(ABC, AC), &AO) > 0)
	{
		if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction = 
				Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			if (Math_InnerProduct(&AB, &AO) > 0)
			{
				*Direction =
					Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
				PList[0] = B;
				PList[1] = A;
				*PointCount = 2;
				return true;
			}
			else
			{
				*Direction = AO;
				PList[0] = A;
				*PointCount = 1;
				return false;
			}
		}
	}
	else
	{
		if (Math_InnerProduct(&Math_CrossProduct(AB, ABC), &AO) > 0)
		{
			if (Math_InnerProduct(&AB, &AO) > 0)
			{
				*Direction = 
					Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
				PList[0] = B;
				PList[1] = A;
				*PointCount = 2;
				return true;
			}
			else
			{
				*Direction = AO;
				PList[0] = A;
				*PointCount = 1;
				return false;
			}
		}
		else
		{
			if (Math_InnerProduct(&ABC, &AO) > 0)
			{
				*Direction = ABC;
				*PointCount = 3;
				return true;
			}
			else
			{
				*Direction = -ABC;
				PList[0] = B;
				PList[1] = C;
				*PointCount = 3;
				return true;
			}
		}
	}
}

bool
GJK_TetrahedronSimplex(SupportPoint* PList, v3 *Direction,
	uint32* PointCount)
{
	SupportPoint A = PList[3];
	SupportPoint B = PList[2];
	SupportPoint C = PList[1];
	SupportPoint D = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AD = D.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = Math_CrossProduct(AB, AC);
	v3 ACD = Math_CrossProduct(AC, AD);
	v3 ADB = Math_CrossProduct(AD, AB);

	if (Math_InnerProduct(&ABC, &AO) > 0)
	{
		if (Math_InnerProduct(&AB, &AO) > 0)
		{
			*Direction =
				Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction = 
				Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ABC;
			PList[0] = C;
			PList[1] = B;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else if (Math_InnerProduct(&ACD, &AO) > 0)
	{
		if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction =
				Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AD, &AO) > 0)
		{
			*Direction = 
				Math_CrossProduct(Math_CrossProduct(AD, AO), AD);
			PList[0] = D;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ACD;
			PList[0] = D;
			PList[1] = C;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else if (Math_InnerProduct(&ADB, &AO) > 0)
	{
		if (Math_InnerProduct(&AB, &AO) > 0)
		{
			*Direction = 
				Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AD, &AO) > 0)
		{
			*Direction =
				Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ADB;
			PList[0] = B;
			PList[1] = D;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else
	{
		return true;
	}
}

// TODO: Temporary variable
SupportPoint PointList[4];

bool
Collision_GJK(CollisionObject* ObjectA, CollisionObject* ObjectB)
{
	SupportPoint A;
	uint32 PointCount = 1;

	v3 RandomDirection = { 1.0f, 0.0f, 0.0f };
	SupportPoint StartingPoint = 
		Collision_MinkowskiSupport(ObjectA, ObjectB,
		&RandomDirection);

	PointList[0] = StartingPoint;
	// NOTE: Origin - StartPoint.
	v3 Direction = -StartingPoint.MinkowskiPoint; 

	uint32 EmergencyExit = 0;

	while (1)
	{
		A = Collision_MinkowskiSupport(ObjectA, ObjectB, 
			&Direction);

		// NOTE: No Intersection
		if (Math_InnerProduct(&A.MinkowskiPoint, 
			&Direction) < 0) 
		{
			return 0;
		}

		PointList[PointCount] = A;
		PointCount++;

		// NOTE: InterSection
		switch (PointCount) 
		{
		case 2:
		{
			if (GJK_LineSimplex(PointList, &Direction, 
				&PointCount))
			{
				//PCount++;
			}
		} break;
		case 3:
		{
			if (GJK_PlaneSimplex(PointList, &Direction, 
				&PointCount))
			{
				//PCount++;
			}
		} break;
		case 4:
		{
			if (GJK_TetrahedronSimplex(PointList, &Direction,
				&PointCount))
			{
				return true;
			}
		} break;
		default: break;
		}

		EmergencyExit++; // Infinit Loop break
		if (EmergencyExit > 100)
		{
			return true;
		}
	}
	return true;
}

float
Collision_PointDistanceFromPlane(v3* Point,
	v3* PointOnPlane, v3* Normal)
{
	// NOTE: If we gaurantee Normal is a unit vector, 
	//	the division can be removed.
	return Math_InnerProduct(Normal,
		&(*Point - *PointOnPlane));// /
		//Math_InnerProduct(Normal, Normal);
}

struct Face
{
	v3 Point[3];
	v3 Normal;
//	uint8 Status;
};

#include <vector>

v3
Collision_EPA(CollisionObject* CollisionObjA,
	CollisionObject* CollisionObjB)
{
	std::vector<Face> Rebuild;
	std::vector<Face> GoodFaces;
	std::vector<v3> NewPointList;

	Face Faces[4]{};
	uint32 FaceCount = 4;
	uint32 ChosenFace = 0;

	Faces[0].Point[0] = PointList[0].MinkowskiPoint;
	Faces[0].Point[1] = PointList[1].MinkowskiPoint;
	Faces[0].Point[2] = PointList[2].MinkowskiPoint;
		
	Faces[1].Point[0] = PointList[0].MinkowskiPoint;
	Faces[1].Point[1] = PointList[2].MinkowskiPoint;
	Faces[1].Point[2] = PointList[3].MinkowskiPoint;

	Faces[2].Point[0] = PointList[0].MinkowskiPoint;
	Faces[2].Point[1] = PointList[3].MinkowskiPoint;
	Faces[2].Point[2] = PointList[1].MinkowskiPoint;

	Faces[3].Point[0] = PointList[1].MinkowskiPoint;
	Faces[3].Point[1] = PointList[3].MinkowskiPoint;
	Faces[3].Point[2] = PointList[2].MinkowskiPoint;

	for (uint32 Index = 0;
		Index < 4;
		Index++)
	{
		Faces[Index].Normal = Math_CrossProduct(
			Faces[Index].Point[1] - Faces[Index].Point[0],
			Faces[Index].Point[2] - Faces[Index].Point[0]);
		Faces[Index].Normal = Math_Normalize(Faces[Index].Normal);
	}

	GoodFaces.push_back(Faces[0]);
	GoodFaces.push_back(Faces[1]);
	GoodFaces.push_back(Faces[2]);
	GoodFaces.push_back(Faces[3]);

	uint32 EmergencyCount = 0;

	float SmallestDistance = 100000.0f;
	float PrevSmallestDistance = SmallestDistance;
	while (1)
	{
		SmallestDistance = 100000.0f;
 		for (uint32 Index = 0;
			Index < GoodFaces.size();
			Index++)
		{
			float Distance =
				-Collision_PointDistanceFromPlane(&v3(0.0f, 0.0f, 0.0f),
					&GoodFaces[Index].Point[0], &GoodFaces[Index].Normal);
		//	Distance = Math_AbsVal(Distance);

			if (Distance < SmallestDistance)
			{
				SmallestDistance = Distance;
				ChosenFace = Index;
			}
		}

		if (SmallestDistance >= (PrevSmallestDistance + 0.000001f))
		{
			return GoodFaces[ChosenFace].Normal;
		}
		else
		{
			SupportPoint NewPoint =
				Collision_MinkowskiSupport(CollisionObjB,
					CollisionObjB, &GoodFaces[ChosenFace].Normal);

			for (uint32 Index = 0;
				Index < GoodFaces.size(); 
				Index++)
			{
				if (Math_InnerProduct(&GoodFaces[Index].Normal,
					&(NewPoint.MinkowskiPoint - 
						GoodFaces[Index].Point[0])) > 0)
				{
						//Rebuild.push_back(Faces[Index]);

						for (uint32 j = 0;
							j < 3;
							j++)
						{
							bool Add = true;
							for (uint32 k = 0;
								k < NewPointList.size();
								k++)
							{
								if (Faces[Index].Point[j] == NewPointList[k])
								{
									Add = false;
								}
							}
							if (Add == true)
							{
								NewPointList.push_back(Faces[Index].Point[j]);
							}
						}
						GoodFaces.erase(GoodFaces.begin() + Index);
					// push face to rebuild
					// remove face from list
				}
			}

			for (uint32 Index = 0;
				Index < NewPointList.size();
				Index++)
			{
				//Face NewFace;
				//NewFace.Point[0] = NewPointList[Index];
				//NewFace.Point[1] = NewPoint.MinkowskiPoint;
				//
				//if (Index == NewPointList.size() - 1)
				//{
				//	NewFace.Point[2] = NewPointList[0];
				//	NewFace.Normal = -Math_CrossProduct(
				//		NewFace.Point[0] - NewFace.Point[1],
				//		NewFace.Point[2] - NewFace.Point[1]);
				//	NewFace.Normal = Math_Normalize(NewFace.Normal);
			//	//	NewFace.Status = 1;
				//}
				//else
				//{
				//	NewFace.Point[2] = NewPointList[Index + 1];
				//	NewFace.Normal = Math_CrossProduct(
				//		NewFace.Point[0] - NewFace.Point[1],
				//		NewFace.Point[2] - NewFace.Point[1]);
				//	NewFace.Normal = Math_Normalize(NewFace.Normal);
				////	NewFace.Status = 1;
				//}
				//
				////NewFace.Normal = Math_CrossProduct(
				////	NewFace.Point[0] - NewFace.Point[1],
				////	NewFace.Point[2] - NewFace.Point[1]);
				////NewFace.Normal = Math_Normalize(NewFace.Normal);
				////NewFace.Status = 1;

				Face NewFace;
				NewFace.Point[0] = NewPoint.MinkowskiPoint;
				NewFace.Point[1] = NewPointList[Index];
				if (Index == NewPointList.size() - 1)
				{
					NewFace.Point[2] = NewPointList[0];
				}
				else
				{
					NewFace.Point[2] = NewPointList[Index + 1];
				}
				NewFace.Normal = Math_CrossProduct(
					NewFace.Point[1] - NewFace.Point[0],
					NewFace.Point[2] - NewFace.Point[0]);
				NewFace.Normal = Math_Normalize(NewFace.Normal);

				GoodFaces.push_back(NewFace);
			}

#if 0
			// TODO: Fix the issue of double faces
			for (uint32 Index = 0;
				Index < Rebuild.size();
				Index++)
			{
				Face NewFaceA;
				NewFaceA.Point[0] = Rebuild[Index].Point[0];
				NewFaceA.Point[1] = NewPoint.MinkowskiPoint;
				NewFaceA.Point[2] = Rebuild[Index].Point[1];

				NewFaceA.Normal = Math_CrossProduct(
					Rebuild[Index].Point[0] - Rebuild[Index].Point[1],
					Rebuild[Index].Point[2] - Rebuild[Index].Point[1]);
				NewFaceA.Status = 1;

				GoodFaces.push_back(NewFaceA);

				Face NewFaceB;
				NewFaceB.Point[0] = Rebuild[Index].Point[1];
				NewFaceB.Point[1] = NewPoint.MinkowskiPoint;
				NewFaceB.Point[2] = Rebuild[Index].Point[2];

				NewFaceB.Normal = Math_CrossProduct(
					Rebuild[Index].Point[0] - Rebuild[Index].Point[1],
					Rebuild[Index].Point[2] - Rebuild[Index].Point[1]);
				NewFaceB.Status = 1;

				GoodFaces.push_back(NewFaceB);

				Face NewFaceC;
				NewFaceC.Point[0] = Rebuild[Index].Point[2];
				NewFaceC.Point[1] = NewPoint.MinkowskiPoint;
				NewFaceC.Point[2] = Rebuild[Index].Point[0];

				NewFaceC.Normal = Math_CrossProduct(
					Rebuild[Index].Point[0] - Rebuild[Index].Point[1],
					Rebuild[Index].Point[2] - Rebuild[Index].Point[1]);
				NewFaceC.Status = 1;

				GoodFaces.push_back(NewFaceC);
			}
#endif
		}
		EmergencyCount++;
		if (EmergencyCount > 10)
		{
			return Faces[ChosenFace].Normal;
		}
		PrevSmallestDistance = SmallestDistance;
	}
}

/*
Take over the simplex from GJK when GJK terminated, and “blow up” the simplex
to a tetrahedron if it contains less than 4 vertices.

Use the 4 faces (triangles) of the tetrahedron to construct an initial polytope.

Pick the closest face of the polytope to the origin.
- requires, 3 vertices of the face, normal of the face,

If the closest face is no closer (by a certain threshold) to the origin than
the previously picked one, go to 8.

Remove the closest face, use the face normal (outward pointing) as the search
direction to find a support point on the Minkowski Difference.

Remove all faces from the polytope that can be “seen” by this new support point,
and add new faces to cover up the “hole” on the polytope, where all new faces
share the new support point as a common vertex (this is the expanding part of
the algorithm).

Go to 3.

Project the origin onto the closest triangle. This is our closest point to the
origin on the CSO’s boundary. Compute the barycentric coordinates of this closest
point with respect to the vertices from the closest triangle. The barycentric
coordinates are coefficients of linear combination of vertices from the closest
triangle. Linearly combining the individual support points (original results
from individual colliders) corresponding to the vertices from the closest triangle,
with the same barycentric coordinates as coefficients, gives us contact
points on both colliders in their own local space. We can then convert these
contact points to world space.

End EPA.
*/

#if 0

int 
Collision_ButtonClick(v2* MousePosition, CollisionObject* CollObj)
{
	window_properties WindowDimensions = Render_GetWindowProperties();
	v2 NewMousePosition;
	NewMousePosition.x =
		MousePosition->x - ((float)WindowDimensions.Width * 0.5f);
	NewMousePosition.y =
		((float)WindowDimensions.Height * 0.5f) - MousePosition->y;

	if ((NewMousePosition.x > (CollObj->Position->x - (CollObj->HalfWidth))) &&
		(NewMousePosition.x < (CollObj->Position->x + (CollObj->HalfWidth))))
	{
		if ((NewMousePosition.y >(CollObj->Position->y - (CollObj->HalfHeight))) &&
			(NewMousePosition.y < (CollObj->Position->y + (CollObj->HalfHeight))))
		{
			if (Platform_GetMouseState())
			{
				return CollObj->CollisionCode + 2;
			}
			return CollObj->CollisionCode + 1;
		}
	}
	return CollObj->CollisionCode;
}

bool 
Collision_OrthoMouseToRect(v2* MousePosition, CollisionObject* Object)
{
	window_properties WindowDimensions = Render_GetWindowProperties();
	v2 NewMousePosition;
	NewMousePosition.x =
		MousePosition->x - ((float)WindowDimensions.Width * 0.5f);
	NewMousePosition.y =
		((float)WindowDimensions.Height * 0.5f) - MousePosition->y;

	if ((NewMousePosition.x > (Object->Position->x - (Object->HalfWidth))) &&
		(NewMousePosition.x < (Object->Position->x + (Object->HalfWidth))))
	{
		if ((NewMousePosition.y >(Object->Position->y - (Object->HalfHeight))) &&
			(NewMousePosition.y < (Object->Position->y + (Object->HalfHeight))))
		{
			return true;
		}
	}
	return false;

}

v3 Collision_UpdateMousePickRay(m4 *ProjectionMatrix, m4 *ViewMatrix)
{
	window_properties WindowDimensions = Render_GetWindowProperties();
	v2 MousePosition = {};
	Platform_GetCursorPosition(&MousePosition.x, &MousePosition.y);

	// TODO: Fix mouse coords, X wrong direction? somewhere in view matrix?.
	MousePosition.x = 1.0f - ((2.0f * MousePosition.x) / WindowDimensions.Width);
	MousePosition.y = 1.0f - ((2.0f * MousePosition.y) / WindowDimensions.Height);

	// Convert to Clip Space 
	v4 ClipSpaceVector = v4(MousePosition.x, MousePosition.y, -1.0f, 0.0f);

	// Convert to Eye Space
	v4 EyeSpaceVector = Math_InvertMatrix(ProjectionMatrix) * ClipSpaceVector;
	EyeSpaceVector = v4(EyeSpaceVector.x, EyeSpaceVector.y, -1.0f, 1.0f);

	// Convert to View Space
	v4 ViewSpaceVector = Math_InvertMatrix(ViewMatrix) * EyeSpaceVector;
	v3 WorldMouseRay = v3(-ViewSpaceVector.x, ViewSpaceVector.y, ViewSpaceVector.z);
	WorldMouseRay = Math_Normalize(WorldMouseRay);

	return WorldMouseRay;
}

bool 
Collision_PlaneRayIntersect(v3* RayOrigin, v3* RayDirection, 
	CollisionObject* CollideObject, float* Distance)
{

	return false;
}

bool 
Collision_RayToOBB(v3* RayOrigin, v3* RayDirection,
	CollisionObject* Object, float* Distance)
{
	return false;
}

v3 
GetFurthestPoint(CollisionObject* Object, v3 *Direction)
{
	v3 FurthestPoint = { 0.0f, 0.0f, 0.0f };
	float MaxDot = -100000.0f;

	// this is checking all points twice...
	for (uint32 i = 0; i < Object->NumVertices; i++)
	{
		v3 Vertex = { Object->VerticesPtr[i * 3],
			Object->VerticesPtr[i * 3 + 1],
			Object->VerticesPtr[i * 3 + 2] };
		Vertex += *Object->Position;
		float Dot = Math_InnerProduct(&Vertex, Direction);

		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			FurthestPoint = Vertex;
		}
	}

	return FurthestPoint;
}

// Finds the furtherst point within a shape in a given direction
SupportPoint 
MinkowskiSupport(CollisionObject* ObjectA, 
	CollisionObject* ObjectB, v3 *Direction) 
{
	SupportPoint Result;
	Result.ObjAPoint = GetFurthestPoint(ObjectA, Direction);
	Result.ObjBPoint = GetFurthestPoint(ObjectB, &(-*Direction));
	Result.MinkowskiPoint = Result.ObjAPoint - Result.ObjBPoint;

	return Result;// max point in direction
}

bool 
LineSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount)
{
	SupportPoint A = PList[1];
	SupportPoint B = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;

	if (Math_InnerProduct(&AB, &AO) > 0)
	{
		*Direction = Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
		*PointCount = 2;
		return true;
	}
	else
	{
		*Direction = AO;
		PList[0] = A;
		*PointCount = 1;
		return false;
	}
}

bool
PlaneSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount)
{	
	SupportPoint A = PList[2];
	SupportPoint B = PList[1];
	SupportPoint C = PList[0];
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = Math_CrossProduct(AB, AC);
	
	if (Math_InnerProduct(&Math_CrossProduct(ABC, AC), &AO) > 0 )
	{
		if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			if (Math_InnerProduct(&AB, &AO) > 0)
			{
				*Direction = Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
				PList[0] = B;
				PList[1] = A;
				*PointCount = 2;
				return true;
			}
			else
			{
				*Direction = AO;
				PList[0] = A;
				*PointCount = 1;
				return false;
			}
		}
	}
	else
	{
		if (Math_InnerProduct(&Math_CrossProduct(AB, ABC), &AO) > 0)
		{
			if (Math_InnerProduct(&AB, &AO) > 0)
			{
				*Direction = Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
				PList[0] = B;
				PList[1] = A;
				*PointCount = 2;
				return true;
			}
			else
			{
				*Direction = AO;
				PList[0] = A;
				*PointCount = 1;
				return false;
			}
		}
		else
		{
			if (Math_InnerProduct(&ABC, &AO) > 0)
			{
				*Direction = ABC; 
				*PointCount = 3;
				return true;
			}
			else
			{
				*Direction = -ABC; 
				PList[0] = B;
				PList[1] = C;
				*PointCount = 3;
				return true;
			}
		}
	}
}

bool 
TetrahedronSimplex(SupportPoint* PList, v3 *Direction, uint32* PointCount)
{
	SupportPoint A = PList[3];
	SupportPoint B = PList[2];
	SupportPoint C = PList[1];
	SupportPoint D = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AD = D.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = Math_CrossProduct(AB, AC);
	v3 ACD = Math_CrossProduct(AC, AD);
	v3 ADB = Math_CrossProduct(AD, AB);

	if (Math_InnerProduct(&ABC, &AO) > 0)
	{
		if (Math_InnerProduct(&AB, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ABC;
			PList[0] = C;
			PList[1] = B;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else if (Math_InnerProduct(&ACD, &AO) > 0)
	{
		if (Math_InnerProduct(&AC, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AD, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AD, AO), AD);
			PList[0] = D;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ACD;
			PList[0] = D;
			PList[1] = C;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else if (Math_InnerProduct(&ADB, &AO) > 0)
	{
		if (Math_InnerProduct(&AB, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else if (Math_InnerProduct(&AD, &AO) > 0)
		{
			*Direction = Math_CrossProduct(Math_CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			*Direction = ADB;
			PList[0] = B;
			PList[1] = D;
			PList[2] = A;
			*PointCount = 3;
			return false;
		}
	}
	else
	{
		return true;
	}
}

// TODO: Temporary variable
SupportPoint PointList[4];

bool 
Collision_GJK(CollisionObject* ObjectA, CollisionObject* ObjectB)
{
	SupportPoint A;
	uint32 PointCount = 1;

	v3 RandomDirection = { 0.0f, 1.0f, 0.0f };
	SupportPoint StartingPoint = MinkowskiSupport(ObjectA, ObjectB, 
		&RandomDirection);

	PointList[0] = StartingPoint;
	v3 Direction = -StartingPoint.MinkowskiPoint; // Origin - StartPoint.

	uint32 EmergencyExit = 0;

	while (1)
	{
		A = MinkowskiSupport(ObjectA, ObjectB, &Direction);

		if (Math_InnerProduct(&A.MinkowskiPoint, &Direction) < 0) // No Intersection
		{
			return 0;
		}

		PointList[PointCount] = A;
		PointCount++;

		switch (PointCount) // InterSection
		{
		case 2:
		{
			if (LineSimplex(PointList, &Direction, &PointCount))
			{
				//PCount++;
			}
		} break;
		case 3:
		{
			if (PlaneSimplex(PointList, &Direction, &PointCount))
			{
				//PCount++;
			}
		} break;
		case 4:
		{
			if (TetrahedronSimplex(PointList, &Direction, &PointCount))
			{
				return true;
			}
		} break;
		default: break;
		}

		EmergencyExit++; // Infinit Loop break
		if (EmergencyExit > 100)
		{
			return true;
		}
	}

	return true;

}

Face 
AddFaceEPA(SupportPoint &v0, SupportPoint &v1, SupportPoint &v2)
{
	Face Result;
	Result.Vertices[0] = v0;
	Result.Vertices[1] = v1;
	Result.Vertices[2] = v2;
	Result.Normal = Math_CrossProduct(Result.Vertices[1].MinkowskiPoint - Result.Vertices[0].MinkowskiPoint,
		Result.Vertices[2].MinkowskiPoint - Result.Vertices[1].MinkowskiPoint);
	Result.OriginDistance = Math_InnerProduct(&Result.Normal,
		&(-Result.Vertices[0].MinkowskiPoint)) / Math_Magnitude(Result.Normal);

	// NOTE: To fix errors in wrapping order
	if (Result.OriginDistance < 0) 
	{							 
		Result.OriginDistance = -Result.OriginDistance;
		Result.Normal = -Result.Normal;
	}

	return Result;
}

void 
AddEdge(SupportPoint *A, SupportPoint *B, std::list<Edge> *Edges)
{
	for (auto it = Edges->begin(); it != Edges->end(); it++) 
	{
		if (it->A == *B && it->B == *A)
		{
			// NOTE: opposite edge found, remove it and do not add new one
			Edges->erase(it);
			return;
		}
	}
	Edges->push_back(Edge(*A, *B));
}

Face 
Collision_EPA(CollisionObject* ObjectA, CollisionObject* ObjectB)
{
	double Threshold = 0.000001;
	uint32 IterationLimit = 50;
	uint32 CurrentIteration = 0;

	std::list<Face> Triangles;
	std::list<Edge> Edges;

	Triangles.push_back(AddFaceEPA(PointList[0], PointList[1], PointList[2]));
	Triangles.push_back(AddFaceEPA(PointList[0], PointList[2], PointList[3]));
	Triangles.push_back(AddFaceEPA(PointList[0], PointList[3], PointList[1]));
	Triangles.push_back(AddFaceEPA(PointList[1], PointList[3], PointList[2]));

	while (1)
	{
		std::list<Face>::iterator entry_cur_triangle_it = Triangles.begin();
		float FurthestDistance = FLT_MAX;
		for (std::list<Face>::iterator it = Triangles.begin();
			it != Triangles.end(); it++) 
		{
			float dst = fabs(Math_InnerProduct(&it->Normal,
				&it->Vertices[0].MinkowskiPoint));
			if (dst < FurthestDistance) 
			{
				FurthestDistance = dst;
				entry_cur_triangle_it = it;
			}
		}

		if (CurrentIteration++ >= IterationLimit)
		{
			return *entry_cur_triangle_it;
		}

 		SupportPoint entry_cur_support = MinkowskiSupport(ObjectA, ObjectB, &(-entry_cur_triangle_it->Normal));

		if ((Math_InnerProduct(&entry_cur_triangle_it->Normal,
			&entry_cur_support.MinkowskiPoint) - FurthestDistance < Threshold))
		{
			return *entry_cur_triangle_it;
			break;
		}

		for (std::list<Face>::iterator it = Triangles.begin();
			it != Triangles.end(); it++)
		{
			if (Math_InnerProduct(&it->Normal,
				&(entry_cur_support.MinkowskiPoint - it->Vertices[0].MinkowskiPoint)) > 0) 
			{
				AddEdge(&it->Vertices[0], &it->Vertices[1], &Edges);
				AddEdge(&it->Vertices[1], &it->Vertices[2], &Edges);
				AddEdge(&it->Vertices[2], &it->Vertices[0], &Edges);
				it = Triangles.erase(it);
			}
		}

		for (std::list<Edge>::iterator it = Edges.begin();
			it != Edges.end(); it++) 
		{
			Triangles.push_back(AddFaceEPA(entry_cur_support, it->A, it->B));
		}

		Edges.clear();
	}
}

//face C = Collision_EPA_FindClosestFace()
//get a new support in the direction
//find closest face to origin

/*
Take over the simplex from GJK when GJK terminated, and “blow up” the simplex 
to a tetrahedron if it contains less than 4 vertices.

Use the 4 faces (triangles) of the tetrahedron to construct an initial polytope.

Pick the closest face of the polytope to the origin.
- requires, 3 vertices of the face, normal of the face, 

If the closest face is no closer (by a certain threshold) to the origin than 
the previously picked one, go to 8.

Remove the closest face, use the face normal (outward pointing) as the search 
direction to find a support point on the Minkowski Difference.

Remove all faces from the polytope that can be “seen” by this new support point,
and add new faces to cover up the “hole” on the polytope, where all new faces 
share the new support point as a common vertex (this is the expanding part of 
the algorithm).

Go to 3.

Project the origin onto the closest triangle. This is our closest point to the 
origin on the CSO’s boundary. Compute the barycentric coordinates of this closest 
point with respect to the vertices from the closest triangle. The barycentric 
coordinates are coefficients of linear combination of vertices from the closest 
triangle. Linearly combining the individual support points (original results 
from individual colliders) corresponding to the vertices from the closest triangle, 
with the same barycentric coordinates as coefficients, gives us contact 
points on both colliders in their own local space. We can then convert these 
contact points to world space.

End EPA.
*/

/* EPA algorithm
Simplex s = termination simplex from GJK
//loop to find the collision information
while (true) {
// obtain the feature (edge for 2D) closest to the
// origin on the Minkowski Difference
Edge e = findClosestEdge(s);
// obtain a new support point in the direction of the edge normal
Vector p = support(A, B, e.normal);
// check the distance from the origin to the edge against the
// distance p is along e.normal
double d = p.dot(e.normal);
if (d - e.distance < TOLERANCE) {
// the tolerance should be something positive close to zero (ex. 0.00001)

// if the difference is less than the tolerance then we can
// assume that we cannot expand the simplex any further and
// we have our solution
normal = e.normal;
depth = d;
} else {
// we haven't reached the edge of the Minkowski Difference
// so continue expanding by adding the new point to the simplex
// in between the points that made the closest edge
simplex.insert(p, e.index);
}
}


*/

bool Collision_HeightMap(CollisionObject* CollisionObj, v3 &ObjectPosition)
{
	int32 x, z;
	// TODO: Add bounds for overflow.
	x = (int32)round(ObjectPosition.x);// -1; 
	z = (int32)round(ObjectPosition.z);
	// x and z contain the objects poisition (player?)

	v3 TerrainVertice;

	float* VerticeAttrib = 0;

	int Test3 = ((x * (uint32)CollisionObj->Depth) + // column
		((uint32)CollisionObj->Depth - z)) * 3; // row and 3  for floats to vertices
	VerticeAttrib = &CollisionObj->VerticesPtr[Test3];
	TerrainVertice.x = *VerticeAttrib++;
	TerrainVertice.y = *VerticeAttrib++;
	TerrainVertice.z = *VerticeAttrib;

	// NOTE: If the object is below the terrain
	if ((ObjectPosition.y) < TerrainVertice.y + 1.0f)
	{

		v3 TerrainVertice2;
		v3 TerrainVertice3;

		if ((ObjectPosition.x - x) < 0)//1 - x) < 0)
		{
			if ((ObjectPosition.z - z) < 0)
			{
				if (z < (int32)CollisionObj->Depth)
				{
					int Test = ((x * (uint32)CollisionObj->Depth) + ((uint32)CollisionObj->Depth - (z - 1))) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice2.x = *VerticeAttrib++;
					TerrainVertice2.y = *VerticeAttrib++;
					TerrainVertice2.z = *VerticeAttrib;

					int x = 0;
				}
				if (x > 0)
				{
					int Test = (((x - 1) * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - z)) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice3.x = *VerticeAttrib++;
					TerrainVertice3.y = *VerticeAttrib++;
					TerrainVertice3.z = *VerticeAttrib;
				}
			}
			else
			{
				int Test2 = (((x - 1) * (uint32)CollisionObj->Depth) +
					((uint32)CollisionObj->Depth - (z + 1))) * 3;
				VerticeAttrib = &CollisionObj->VerticesPtr[Test2];
				TerrainVertice2.x = *VerticeAttrib++;
				TerrainVertice2.y = *VerticeAttrib++;
				TerrainVertice2.z = *VerticeAttrib;

				if ((((TerrainVertice.x - TerrainVertice2.x) * 
					((-ObjectPosition.z) - (-TerrainVertice2.z))) -
					(((-TerrainVertice.z) - (-TerrainVertice2.z)) *
					(ObjectPosition.x - TerrainVertice2.x))) < 0)
				{
					int Test = ((x * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - (z + 1))) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice3.x = *VerticeAttrib++;
					TerrainVertice3.y = *VerticeAttrib++;
					TerrainVertice3.z = *VerticeAttrib;
				}
				else
				{
					int Test = (((x - 1) * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - z)) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice3.x = *VerticeAttrib++;
					TerrainVertice3.y = *VerticeAttrib++;
					TerrainVertice3.z = *VerticeAttrib;
				}
			}
		}
		else
		{
			if ((ObjectPosition.z - z) < 0) // red
			{
				if ((z < 0) &&
					(x < (int32)CollisionObj->Width))
				{
					int Test2 = (((x + 1) * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - (z - 1))) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test2];
					TerrainVertice2.x = *VerticeAttrib++;
					TerrainVertice2.y = *VerticeAttrib++;
					TerrainVertice2.z = *VerticeAttrib;

					if ((((TerrainVertice2.x - TerrainVertice.x) *
						((-ObjectPosition.z) - (-TerrainVertice.z))) -
						(((-TerrainVertice2.z) - (-TerrainVertice.z)) *
						(ObjectPosition.x - TerrainVertice.x))) < 0)
					{
						int Test = (((x + 1) *
							(uint32)CollisionObj->Depth) +
							((uint32)CollisionObj->Depth - z)) * 3;
						VerticeAttrib = &CollisionObj->VerticesPtr[Test];
						TerrainVertice3.x = *VerticeAttrib++;
						TerrainVertice3.y = *VerticeAttrib++;
						TerrainVertice3.z = *VerticeAttrib;
					}
					else
					{
						int Test = ((x * (uint32)CollisionObj->Depth) +
							((uint32)CollisionObj->Depth - (z - 1))) * 3;
						VerticeAttrib = &CollisionObj->VerticesPtr[Test];
						TerrainVertice3.x = *VerticeAttrib++;
						TerrainVertice3.y = *VerticeAttrib++;
						TerrainVertice3.z = *VerticeAttrib;
					}
				}
			}
			else
			{
				if (z < 0)
				{
					int Test = ((x * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - (z + 1))) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice2.x = *VerticeAttrib++;
					TerrainVertice2.y = *VerticeAttrib++;
					TerrainVertice2.z = *VerticeAttrib;
				}
				if (x < (int32)CollisionObj->Width) // conversation errors?
				{
					int Test = (((x + 1) * (uint32)CollisionObj->Depth) +
						((uint32)CollisionObj->Depth - z)) * 3;
					VerticeAttrib = &CollisionObj->VerticesPtr[Test];
					TerrainVertice3.x = *VerticeAttrib++;
					TerrainVertice3.y = *VerticeAttrib++;
					TerrainVertice3.z = *VerticeAttrib;
				}
			}
		}

		//Line Test;
		//Test.Init(TerrainVertice.Arr,
		//	 (TerrainVertice + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test.Draw();
		//Test.Delete();
		//Test.~Line();
		//
		//Line Test2;
		//Test2.Init(TerrainVertice2.Arr, 
		//	(TerrainVertice2 + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test2.Draw();
		//Test2.Delete();
		//Test2.~Line();
		//
		//Line Test3;
		//Test3.Init(TerrainVertice3.Arr, 
		//	(TerrainVertice3 + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test3.Draw();
		//Test3.Delete();
		//Test3.~Line();

		v3 Line1 = (TerrainVertice2 - TerrainVertice);
		v3 Line2 = (TerrainVertice3 - TerrainVertice);
		v3 Norm = Math_Normalize(Math_CrossProduct(Line2, Line1));
		CollisionObj->CollideNormal = Norm;
		v3 PointLine = ObjectPosition - TerrainVertice;
		float test = -Math_InnerProduct(&PointLine, &Norm);
		v3 point = test * Norm;

		ObjectPosition.y = ObjectPosition.y + point.y + 0.7f;
		return true;
	}
	else
	{
		return false;
	}
}

v3 Collision_GetNormal(CollisionObject* CollideObj, PhysicsObject* PhysObj)
{
	v3 Normal;
	
	v3 LBR = { CollideObj->VerticesPtr[0], CollideObj->VerticesPtr[1] , CollideObj->VerticesPtr[2] };
	v3 RBR = { CollideObj->VerticesPtr[3], CollideObj->VerticesPtr[4] , CollideObj->VerticesPtr[5] };
	v3 RTR = { CollideObj->VerticesPtr[6], CollideObj->VerticesPtr[7] , CollideObj->VerticesPtr[8] };
	v3 LTR = { CollideObj->VerticesPtr[12], CollideObj->VerticesPtr[13] , CollideObj->VerticesPtr[14] };
	v3 LBF = { CollideObj->VerticesPtr[18], CollideObj->VerticesPtr[19] , CollideObj->VerticesPtr[20] };
	v3 RBF = { CollideObj->VerticesPtr[21], CollideObj->VerticesPtr[22] , CollideObj->VerticesPtr[23] };
	v3 RTF = { CollideObj->VerticesPtr[24], CollideObj->VerticesPtr[25] , CollideObj->VerticesPtr[26] };
	v3 LTF = { CollideObj->VerticesPtr[30], CollideObj->VerticesPtr[31] , CollideObj->VerticesPtr[32] };
	LBR = LBR * *CollideObj->Position;
	RBR = RBR + *CollideObj->Position;
	RTR = RTR + *CollideObj->Position;
	LTR = LTR + *CollideObj->Position;
	LBF = LBF + *CollideObj->Position;
	RBF = RBF + *CollideObj->Position;
	RTF = RTF + *CollideObj->Position;
	LTF = LTF + *CollideObj->Position;

	v4 CurrentPoint = { PhysObj->PrevPosition.x, PhysObj->PrevPosition.y,
		PhysObj->PrevPosition.z, 1 };

	// back
	v3 LineBackBottom = (RBR - LBR);
	v3 LineBackRight = (RBR - RTR);
	v3 BackNormal = Math_CrossProduct(LineBackBottom, LineBackRight);

	float w = Math_InnerProduct(&LineBackBottom, &BackNormal);

	v4 BackPlane = { BackNormal.x, BackNormal.y, BackNormal.z, w};

	// top
	v3 LineTopBack = (RTR - LTR);
	v3 LineTopRight = (RTR - RTF);
	v3 TopNormal = Math_CrossProduct(LineTopBack, LineTopRight);

	float t = Math_InnerProduct(&LineTopBack, &TopNormal);

	v4 TopPlane = { TopNormal.x, TopNormal.y, TopNormal.z, t };


	// right
	v3 LineRightBottom = (RTR - RTF);
	v3 LineRightRight = (RTR - RBR);
	v3 RightNormal = Math_CrossProduct(LineRightBottom, LineRightRight);

	float p = Math_InnerProduct(&LineRightBottom, &RightNormal);

	v4 RightPlane = { RightNormal.x, RightNormal.y, RightNormal.z, p };


	if (Math_InnerProduct(&BackPlane, &CurrentPoint) < 0)
	{
		Normal = BackNormal;
	}
	else if (Math_InnerProduct(&TopPlane, &CurrentPoint) < 0)
	{
		Normal = TopNormal;
	}
	else if (Math_InnerProduct(&RightPlane, &CurrentPoint) < 0)
	{
		Normal = RightNormal;
	}

	Line Test;
	Test.Init(CollideObj->Position->Arr, Normal.Arr, 10.0f);
	Test.Draw();
	Test.Delete();
	Test.~Test();

	return Normal;
}
#endif // DATA_ORIENTED