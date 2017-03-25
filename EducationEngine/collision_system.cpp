#include "collision_system.h"

void FillCollisionObject(CollisionObject* Object, uint32 NumVertices,
	float* VerticesPtr, v3 &ObjPosition, float Width, float Height,
	float Depth)
{
	Object->NumVertices = NumVertices;
	Object->VerticesPtr = VerticesPtr;
	*Object->Position = ObjPosition; //TODO: remove?
	Object->Width = Width;
	Object->Height = Height;
	Object->Depth = Depth;
}

uint8 GetClickState()
{
	uint8 Result = 0;

	if (Platform_GetStateOfKey(0x01))
	{
		Result = 1;
	}

	return Result;
}

v2 GetOrthoMousePosition() // TODO: RENAME
{
	v2 Result = {};

	Platform_GetCursorPosition(&Result.x, &Result.y);

	int Width = Render_GetWindowProperties().Width;
	int Height = Render_GetWindowProperties().Height;

	Result.x = Result.x - ((float)Width / 2);
	Result.y = ((float)Height / 2) - Result.y;

	return Result;
}

bool Collision_OrthoMouseToRect(v3 &ObjectPosition,
	float ObjWidth, float ObjHeight)
{
	v2 MousePosition = GetOrthoMousePosition();

	if ((MousePosition.x > (ObjectPosition.x - (ObjWidth / 2))) &&
		(MousePosition.x < (ObjectPosition.x + (ObjWidth / 2))))
	{
		if ((MousePosition.y >(ObjectPosition.y - (ObjHeight / 2))) &&
			(MousePosition.y < (ObjectPosition.y + (ObjHeight / 2))))
		{
			return true;
		}
	}
	return false;

}

v3 Collision_UpdateMousePickRay(m4 *ProjectionMatrix, m4 *ViewMatrix)
{
	window_properties Window = Render_GetWindowProperties();
	v2 MousePosition = GetOrthoMousePosition();
	MousePosition.x = (2.0f * MousePosition.x) / Window.Width - 1.0f;
	MousePosition.y = 1.0f - (2.0f * MousePosition.y) / Window.Height;
	v2 ray_nds = MousePosition;

	v4 ray_clip = v4(ray_nds.x, ray_nds.y, -1.0f, 0.0f);
	v4 ray_eye = GetMatrixInverse(ProjectionMatrix) * ray_clip;
	ray_eye = v4(ray_eye.x, ray_eye.y, -1.0f, 1.0f);

	v4 Result = MatrixInverse(*ViewMatrix) * ray_eye;
	v3 ray_world = v3(Result.x, Result.y, Result.z);

	ray_world = Normalize(ray_world);

	return ray_world;
}

v3 GetFurthestPoint(CollisionObject* Object, v3 &Direction)
{
	v3 FurthestPoint = { 0.0f, 0.0f, 0.0f };
	float MaxDot = -100000.0f;

	//for (uint32 i = 0; i < CollisionObjects[EntityID].NumVertices; i++) // this is checking all points twice...
	for (uint32 i = 0; i < Object->NumVertices; i++)
	{
		v3 Vertex = { Object->VerticesPtr[i * 3],
			Object->VerticesPtr[i * 3 + 1],
			Object->VerticesPtr[i * 3 + 2] };
		Vertex += *Object->Position;
		float Dot = InnerProduct(Vertex, Direction);

		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			FurthestPoint = Vertex;
		}
	}

	return FurthestPoint;
}

//v3 GetFurthestPoint(ObjectInstance *ObjA, v3 &Direction)
//{
//	v3 FurthestPoint = {0.0f, 0.0f, 0.0f};
//	float MaxDot = -100000.0f;
//
//	for (uint32 i = 0; i < ObjA->ObjectPtr->NumberOfVertices; i++) // this is checking all points twice...
//	{
//		v3 Vertex = { ObjA->ObjectPtr->VerticeFloatArrayPtr[i * 3],
//					  ObjA->ObjectPtr->VerticeFloatArrayPtr[i * 3 + 1],
//					  ObjA->ObjectPtr->VerticeFloatArrayPtr[i * 3 + 2] };
//		Vertex.x += ObjA->ModelMatrix.Rc[3][0];
//		Vertex.y += ObjA->ModelMatrix.Rc[3][1];
//		Vertex.z += ObjA->ModelMatrix.Rc[3][2];
//		float Dot = InnerProduct(Vertex, Direction);
//
//		if (Dot > MaxDot)
//		{
//			MaxDot = Dot;
//			FurthestPoint = Vertex;
//		}
//	}
//
//	return FurthestPoint;
//}

//v3 GJKSupport(ObjectInstance *ObjA, ObjectInstance *ObjB, v3 &Direction) // Finds the furtherst point within a shape in a given direction
//{
//	v3 Result = GetFurthestPoint(ObjA, Direction) -
//		GetFurthestPoint(ObjB, -Direction);
//
//	return Result;// max point in direction
//}

SupportPoint MinkowskiSupport(CollisionObject* ObjectA, CollisionObject* ObjectB, v3 &Direction) // Finds the furtherst point within a shape in a given direction
{
	SupportPoint Result;
	//Result.ObjAPoint = GetFurthestPoint(EntityAID, Direction);
	//Result.ObjBPoint = GetFurthestPoint(EntityBID, -Direction);
	Result.ObjAPoint = GetFurthestPoint(ObjectA, Direction);
	Result.ObjBPoint = GetFurthestPoint(ObjectB, -Direction);
	Result.MinkowskiPoint = Result.ObjAPoint - Result.ObjBPoint;

	return Result;// max point in direction
}

//SupportPoint MinkowskiSupport(ObjectInstance *ObjA, ObjectInstance *ObjB, v3 &Direction) // Finds the furtherst point within a shape in a given direction
//{
//	SupportPoint Result;
//	Result.ObjAPoint = GetFurthestPoint(ObjA, Direction);
//	Result.ObjBPoint = GetFurthestPoint(ObjB, -Direction);
//	Result.MinkowskiPoint = Result.ObjAPoint - Result.ObjBPoint;
//
//	return Result;// max point in direction
//}


bool LineSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount)
{
	SupportPoint A = PList[1];
	SupportPoint B = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;

	if (InnerProduct(AB, AO) > 0)
	{
		*Direction = CrossProduct(CrossProduct(AB, AO), AB);
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

bool PlaneSimplex(SupportPoint* PList, v3* Direction, uint32* PointCount)
{	
	SupportPoint A = PList[2];
	SupportPoint B = PList[1];
	SupportPoint C = PList[0];
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = CrossProduct(AB, AC);
	
	if (InnerProduct(CrossProduct(ABC, AC), AO) > 0 )
	{
		if (InnerProduct(AC, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			return false;
		}
		else
		{
			if (InnerProduct(AB, AO) > 0)
			{
				*Direction = CrossProduct(CrossProduct(AB, AO), AB);
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
		if (InnerProduct(CrossProduct(AB, ABC), AO) > 0)
		{
			if (InnerProduct(AB, AO) > 0)
			{
				*Direction = CrossProduct(CrossProduct(AB, AO), AB);
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
			if (InnerProduct(ABC, AO) > 0)
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

bool TetrahedronSimplex(SupportPoint* PList, v3 *Direction, uint32* PointCount)
{
	SupportPoint A = PList[3];
	SupportPoint B = PList[2];
	SupportPoint C = PList[1];
	SupportPoint D = PList[0];
	v3 AB = B.MinkowskiPoint - A.MinkowskiPoint;
	v3 AC = C.MinkowskiPoint - A.MinkowskiPoint;
	v3 AD = D.MinkowskiPoint - A.MinkowskiPoint;
	v3 AO = -A.MinkowskiPoint;
	v3 ABC = CrossProduct(AB, AC);
	v3 ACD = CrossProduct(AC, AD);
	v3 ADB = CrossProduct(AD, AB);


	if (InnerProduct(ABC, AO) > 0)
	{
		if (InnerProduct(AB, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
		//	PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else if (InnerProduct(AC, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
		//	PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else
		{
			*Direction = ABC;
			PList[0] = C;
			PList[1] = B;
			PList[2] = A;
			*PointCount = 3;
		//	TetrahedronSimplex(PList, Direction, PointCount);
			return false;
		}
	}
	else if (InnerProduct(ACD, AO) > 0)
	{
		if (InnerProduct(AC, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			//PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else if (InnerProduct(AD, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AD, AO), AD);
			PList[0] = D;
			PList[1] = A;
			*PointCount = 2;
			//PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else
		{
			*Direction = ACD;
			PList[0] = D;
			PList[1] = C;
			PList[2] = A;
			*PointCount = 3;
			//TetrahedronSimplex(PList, Direction, PointCount);
			return false;
		}
	}
	else if (InnerProduct(ADB, AO) > 0)
	{
		if (InnerProduct(AB, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AB, AO), AB);
			PList[0] = B;
			PList[1] = A;
			*PointCount = 2;
		//	PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else if (InnerProduct(AD, AO) > 0)
		{
			*Direction = CrossProduct(CrossProduct(AC, AO), AC);
			PList[0] = C;
			PList[1] = A;
			*PointCount = 2;
			//PlaneSimplex(PList, Direction, PointCount);
			return false;
		}
		else
		{
			*Direction = ADB;
			PList[0] = B;
			PList[1] = D;
			PList[2] = A;
			*PointCount = 3;
			//TetrahedronSimplex(PList, Direction, PointCount);
			return false;
		}
	}
	else
	{
		return true;
	}
}

SupportPoint PointList[4];

bool Collision_GJK(CollisionObject* ObjectA, CollisionObject* ObjectB)
{
	//v3 A;
	SupportPoint A;
	uint32 PointCount = 1;

	v3 RandomDirection = { 0.0f, 1.0f, 0.0f };
	//v3 StartingPoint = GJKSupport(ObjA, ObjB, RandomDirection);
	//SupportPoint StartingPoint = MinkowskiSupport(EntityAID, EntityBID, RandomDirection);
	SupportPoint StartingPoint = MinkowskiSupport(ObjectA, ObjectB, RandomDirection);

	PointList[0] = StartingPoint;
	v3 Direction = -StartingPoint.MinkowskiPoint; // Origin - StartPoint.

	uint32 EmergencyExit = 0;

	while (1)
	{
		//A = GJKSupport(ObjA, ObjB, Direction);
		//A = MinkowskiSupport(EntityAID, EntityBID, Direction);
		A = MinkowskiSupport(ObjectA, ObjectB, Direction);

		if (InnerProduct(A.MinkowskiPoint, Direction) < 0) // No Intersection
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
//bool GJK(ObjectInstance *ObjA, ObjectInstance *ObjB)
//{
//	//v3 A;
//	SupportPoint A;
//	uint32 PointCount = 1;
//
//	v3 RandomDirection = {0.0f, 1.0f, 0.0f};
//	//v3 StartingPoint = GJKSupport(ObjA, ObjB, RandomDirection);
//	SupportPoint StartingPoint = MinkowskiSupport(ObjA, ObjB, RandomDirection);
//
//	PointList[0] = StartingPoint;
//	v3 Direction = -StartingPoint.MinkowskiPoint; // Origin - StartPoint.
//
//	uint32 EmergencyExit = 0;
//
//	while (1)
//	{
//		//A = GJKSupport(ObjA, ObjB, Direction);
//		A = MinkowskiSupport(ObjA, ObjB, Direction);
//
//		if (InnerProduct(A.MinkowskiPoint, Direction) < 0) // No Intersection
//		{
//			return 0;
//		}
//		
//		PointList[PointCount] = A;
//		PointCount++;
//
//		switch (PointCount) // InterSection
//		{
//		case 2: 
//			{
//				if (LineSimplex(PointList, &Direction, &PointCount))
//				{
//					//PCount++;
//				}
//			} break;
//		case 3:
//			{
//				if (PlaneSimplex(PointList, &Direction, &PointCount))
//				{
//					//PCount++;
//				}
//			} break;
//		case 4:
//			{
//				if (TetrahedronSimplex(PointList, &Direction, &PointCount))
//				{
//					return true;
//				}
//			} break;
//		default: break;
//		}
//
//		EmergencyExit++; // Infinit Loop break
//		if (EmergencyExit > 100)
//		{
//			return true;
//		}
//	}
//
//	return true;
//}

Face AddFaceEPA(SupportPoint &v0, SupportPoint &v1, SupportPoint &v2)
{
	Face Result;
	Result.Vertices[0] = v0;
	Result.Vertices[1] = v1;
	Result.Vertices[2] = v2;
	Result.Normal = CrossProduct(Result.Vertices[1].MinkowskiPoint - Result.Vertices[0].MinkowskiPoint,
		Result.Vertices[2].MinkowskiPoint - Result.Vertices[1].MinkowskiPoint);
	Result.OriginDistance = InnerProduct(Result.Normal,
		-Result.Vertices[0].MinkowskiPoint) / Magnitude(Result.Normal);

	if (Result.OriginDistance < 0) // To fix errors in wrapping order
	{							 
		Result.OriginDistance = -Result.OriginDistance;
		Result.Normal = -Result.Normal;
	}

	return Result;
}

/*Face EPA(ObjectMap *ObjA, ObjectMap *ObjB)
{
	Face Faces[12]; //Temporary (The objects are boxes that will have no more than 12 faces in this test)
	uint32 FaceCount = 4; // Temporary, SimplexPoints always comes in with 4 vertices
	uint32 FinalFace = 0; // Temporary, Store the final closest face position in the array

	Faces[0] = AddFaceEPA(PointList[0], PointList[1], PointList[2]);
	Faces[1] = AddFaceEPA(PointList[1], PointList[2], PointList[0]);
	Faces[2] = AddFaceEPA(PointList[2], PointList[0], PointList[1]);
	Faces[3] = AddFaceEPA(PointList[0], PointList[1], PointList[2]);

	float FinalDistance = Faces[0].OriginDistance; // A random choice start value

	for (uint32 i = 0; i < FaceCount; i++) // Find the closest face to the origin
	{
		if (Faces[i].OriginDistance < FinalDistance)
		{
			FinalDistance = Faces[i].OriginDistance;
			FinalFace = i;
		}
	}

	while (1)
	{
		SupportPoint NewPoint = MinkowskiSupport(ObjA, ObjB, Faces[FinalFace].Normal);

		for (unsigned int i = 0; i < FaceCount; i++)
		{
			if ((Faces[i].Vertices[0] == NewPoint) || // If the new point is already a part of the 
				(Faces[i].Vertices[1] == NewPoint) || // simplex then return the current closest face.
				(Faces[i].Vertices[2] == NewPoint))
			{
				return Faces[FinalFace];
			}
		}

		float D = Inner(NewPoint.MinkowskiPoint, Faces[FinalFace].Normal);

		if ((D - Faces[FinalFace].OriginDistance) < 0.00001) // 0.00001 is the tolerance
		{
			return Faces[FinalFace];
		}
		else
		{
			// Add 3 new faces using the new point, the last one replacing the old face slot in the array.
			Faces[FaceCount] = AddFaceEPA(NewPoint, Faces[FinalFace].Vertices[1], Faces[FinalFace].Vertices[2]);
			FaceCount++;
			Faces[FaceCount] = AddFaceEPA(Faces[FinalFace].Vertices[0], NewPoint, Faces[FinalFace].Vertices[2]);
			FaceCount++;
			Faces[FinalFace] = AddFaceEPA(Faces[FinalFace].Vertices[0], Faces[FinalFace].Vertices[1], NewPoint);
	
			for (unsigned int i = 0; i < FaceCount; i++)
			{
				if (Faces[i].OriginDistance < FinalDistance) // Find the closest face to the origin
				{
					FinalDistance = Faces[i].OriginDistance;
					FinalFace = i;
				}
			}
		}
	}
}*/
/*
bool extrapolateContactInformation(const Face* triangle) 
{
	const m4& colliderA_toWorld = contactData->colliders[0]->collision_detection.mtxColliderToWorld;
	const m4& colliderB_toWorld = contactData->colliders[1]->collision_detection.mtxColliderToWorld;

	const float distanceFromOrigin = Inner(triangle->Normal, triangle->Vertices[0].MinkowskiPoint);

	// calculate the barycentric coordinates of the closest triangle with respect to
	// the projection of the origin onto the triangle
	float bary_u, bary_v, bary_w;
	barycentric(triangle->Normal * distanceFromOrigin,
		triangle->Vertices[0].MinkowskiPoint,
		triangle->Vertices[1].MinkowskiPoint,
		triangle->Vertices[2].MinkowskiPoint,
		&bary_u,
		&bary_v,
		&bary_w);

	// barycentric can fail and generate invalid coordinates, if this happens return false
	if (!is_valid(bary_u) || !is_valid(bary_v) || !is_valid(bary_w)) return false;

	// if any of the barycentric coefficients have a magnitude greater than 1, then the origin is not within the triangular prism described by 'triangle'
	// thus, there is no collision here, return false
	if (fabs(bary_u)>1.0f || fabs(bary_v)>1.0f || fabs(bary_w)>1.0f) return false;

	// collision point on object a in world space
	const v3 wcolpoint(
		(bary_u * (triangle->points[0].localSupports[0] * colliderA_toWorld)) +
		(bary_v * (triangle->points[1].localSupports[0] * colliderA_toWorld)) +
		(bary_w * (triangle->points[2].localSupports[0] * colliderA_toWorld)));

	// collision normal
	const v3 wcolnormal = -triangle->Normal;

	// penetration depth
	const float wpendepth = distanceFromOrigin;

	contactData->point = wcolpoint;
	contactData->normal = wcolnormal;
	contactData->penetration = wpendepth;

	for (uint8 i = 0; i < 2; i++)
		for (uint8 j = 0; j < 3; j++)
			contactData->triangleSupports_local[i][j] = triangle->points[j].localSupports[i];

	return true;
}*/

void AddEdge(SupportPoint *A, SupportPoint *B, std::list<Edge> *Edges)
{
	for (auto it = Edges->begin(); it != Edges->end(); it++) 
	{
		if (it->A == *B && it->B == *A)
		{
			//opposite edge found, remove it and do not add new one
			Edges->erase(it);
			return;
		}
	}
	Edges->push_back(Edge(*A, *B));//(*A, *B);
}

Face Collision_EPA(CollisionObject* ObjectA, CollisionObject* ObjectB)
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
			float dst = fabs(InnerProduct(it->Normal, 
				it->Vertices[0].MinkowskiPoint));
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

 		SupportPoint entry_cur_support = MinkowskiSupport(ObjectA, ObjectB, -entry_cur_triangle_it->Normal);

		if ((InnerProduct(entry_cur_triangle_it->Normal,
			entry_cur_support.MinkowskiPoint) - FurthestDistance < Threshold))
		{
			// GENERATE CONTACT INFO AND RETURN
			return *entry_cur_triangle_it;
			//return extrapolateContactInformation(&*entry_cur_triangle_it, contactData);
			break;
		}

		for (std::list<Face>::iterator it = Triangles.begin();
			it != Triangles.end(); it++)
		{
			// can this face be 'seen' by entry_cur_support?
			if (InnerProduct(it->Normal, 
				(entry_cur_support.MinkowskiPoint - it->Vertices[0].MinkowskiPoint)) > 0) 
			{
				AddEdge(&it->Vertices[0], &it->Vertices[1], &Edges);
				AddEdge(&it->Vertices[1], &it->Vertices[2], &Edges);
				AddEdge(&it->Vertices[2], &it->Vertices[0], &Edges);
				it = Triangles.erase(it);
			}
		}

		// create new triangles from the edges in the edge list
		for (std::list<Edge>::iterator it = Edges.begin();
			it != Edges.end(); it++) 
		{
			Triangles.push_back(AddFaceEPA(entry_cur_support, it->A, it->B));
		}

		Edges.clear();
	}
}

//Face EPA(ObjectInstance *ObjA, ObjectInstance *ObjB)
//{
//	double Threshold = 0.000001;
//	uint32 IterationLimit = 50;
//	uint32 CurrentIteration = 0;
//
//	std::list<Face> Triangles;
//	std::list<Edge> Edges;
//
//	Triangles.push_back(AddFaceEPA(PointList[0], PointList[1], PointList[2]));
//	Triangles.push_back(AddFaceEPA(PointList[0], PointList[2], PointList[3]));
//	Triangles.push_back(AddFaceEPA(PointList[0], PointList[3], PointList[1]));
//	Triangles.push_back(AddFaceEPA(PointList[1], PointList[3], PointList[2]));
//
//	while (1)
//	{
//		std::list<Face>::iterator entry_cur_triangle_it = Triangles.begin();
//		float FurthestDistance = FLT_MAX;
//		for (std::list<Face>::iterator it = Triangles.begin(); it != Triangles.end(); it++) {
//			float dst = fabs(InnerProduct(it->Normal, it->Vertices[0].MinkowskiPoint));
//			if (dst < FurthestDistance) {
//				FurthestDistance = dst;
//				entry_cur_triangle_it = it;
//			}
//		}
//
//		if (CurrentIteration++ >= IterationLimit)
//		{
//			return *entry_cur_triangle_it;
//		}
//
//		SupportPoint entry_cur_support = MinkowskiSupport(ObjA, ObjB, -entry_cur_triangle_it->Normal);
//
//		if ((InnerProduct(entry_cur_triangle_it->Normal, entry_cur_support.MinkowskiPoint) - FurthestDistance < Threshold)) {
//			// GENERATE CONTACT INFO AND RETURN
//			return *entry_cur_triangle_it;
//			//return extrapolateContactInformation(&*entry_cur_triangle_it, contactData);
//			break;
//		}
//
//		for (std::list<Face>::iterator it = Triangles.begin(); it != Triangles.end(); it++) {
//			// can this face be 'seen' by entry_cur_support?
//			if (InnerProduct(it->Normal, (entry_cur_support.MinkowskiPoint - it->Vertices[0].MinkowskiPoint)) > 0) {
//				AddEdge(&it->Vertices[0], &it->Vertices[1], &Edges);
//				AddEdge(&it->Vertices[1], &it->Vertices[2], &Edges);
//				AddEdge(&it->Vertices[2], &it->Vertices[0], &Edges);
//				it = Triangles.erase(it);
//			}
//		}
//
//		// create new triangles from the edges in the edge list
//		for (std::list<Edge>::iterator it = Edges.begin(); it != Edges.end(); it++) {
//			Triangles.push_back(AddFaceEPA(entry_cur_support, it->A, it->B));
//		}
//
//		Edges.clear();
//	}
//}

/*auto lam_addEdge = [&](const SupportPoint &a, const SupportPoint &b)->void {
for (auto it = Edges.begin(); it != Edges.end(); it++) {
if (it->A == b && it->B == a) {
//opposite edge found, remove it and do not add new one
Edges.erase(it);
return;
}
}
Edges.emplace_back(a, b);
};*/


bool Collision_HeightMap(HeightMap* HeightMapPlane, v3 &ObjectPosition)
{
	int32 x, z;
	x = (int32)round(ObjectPosition.x) - 1; // because negative is backwards?
	z = (int32)round(ObjectPosition.z); //TODO: Write round to get rid of cmath

	v3 TerrainVertice;
	
	TerrainVertice.x =
		HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
		((uint32)HeightMapPlane->Depth - z)) * 3];
	TerrainVertice.y =
		HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
		((uint32)HeightMapPlane->Depth - z)) * 3 + 1];
	TerrainVertice.z =
		HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
		((uint32)HeightMapPlane->Depth - z)) * 3 + 2];

	if ((ObjectPosition.y) < TerrainVertice.y + 1.0f)
	{

		v3 TerrainVertice3;
		v3 TerrainVertice2;

		if ((ObjectPosition.x - 1 - x) < 0) // allignment issues?
		{
			if ((ObjectPosition.z - z) < 0)
			{
				if (z < (int32)HeightMapPlane->Depth)
				{
					TerrainVertice2.x =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3];
					TerrainVertice2.y =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 1];
					TerrainVertice2.z =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 2];
				}
				if (x > 0)
				{
					TerrainVertice3.x =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3];
					TerrainVertice3.y =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 1];
					TerrainVertice3.z =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 2];
				}
			}
			else
			{
				TerrainVertice2.x =
					HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
					((uint32)HeightMapPlane->Depth - (z + 1))) * 3];
				TerrainVertice2.y =
					HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
					((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 1];
				TerrainVertice2.z =
					HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
					((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 2];

				if ((((TerrainVertice.x - TerrainVertice2.x) * // Determinant 
					((-ObjectPosition.z) - (-TerrainVertice2.z))) -
					(((-TerrainVertice.z) - (-TerrainVertice2.z)) *
					(ObjectPosition.x - TerrainVertice2.x))) < 0)
				{
					TerrainVertice3.x =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3];
					TerrainVertice3.y =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 1];
					TerrainVertice3.z =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 2];
				}
				else
				{
					TerrainVertice3.x =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3];
					TerrainVertice3.y =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 1];
					TerrainVertice3.z =
						HeightMapPlane->VerticeFloatArrayPtr[(((x - 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 2];
				}
			}
		}
		else
		{
			if ((ObjectPosition.z - z) < 0) // red
			{
				if ((z < 0) &&
					(x < (int32)HeightMapPlane->Width))
				{
					TerrainVertice2.x =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3];
					TerrainVertice2.y =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 1];
					TerrainVertice2.z =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 2];

					if ((((TerrainVertice2.x - TerrainVertice.x) *
						((-ObjectPosition.z) - (-TerrainVertice.z))) -
						(((-TerrainVertice2.z) - (-TerrainVertice.z)) *
						(ObjectPosition.x - TerrainVertice.x))) < 0)
					{
						TerrainVertice3.x =
							HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - z)) * 3];
						TerrainVertice3.y =
							HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - z)) * 3 + 1];
						TerrainVertice3.z =
							HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - z)) * 3 + 2];
					}
					else
					{
						TerrainVertice3.x =
							HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - (z - 1))) * 3];
						TerrainVertice3.y =
							HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 1];
						TerrainVertice3.z =
							HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
							((uint32)HeightMapPlane->Depth - (z - 1))) * 3 + 2];
					}
				}
			}
			else
			{
				if (z < 0)
				{
					TerrainVertice2.x =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3];
					TerrainVertice2.y =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 1];
					TerrainVertice2.z =
						HeightMapPlane->VerticeFloatArrayPtr[((x * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - (z + 1))) * 3 + 2];
				}
				if (x < (int32)HeightMapPlane->Width) // conversation errors?
				{
					TerrainVertice3.x =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3];
					TerrainVertice3.y =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 1];
					TerrainVertice3.z =
						HeightMapPlane->VerticeFloatArrayPtr[(((x + 1) * (uint32)HeightMapPlane->Depth) +
						((uint32)HeightMapPlane->Depth - z)) * 3 + 2];
				}
			}
		}

		//Line Test;
		//Test.Init(TerrainVertice.Arr, (TerrainVertice + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test.Draw();
		//
		//Line Test2;
		//Test2.Init(TerrainVertice2.Arr, (TerrainVertice2 + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test2.Draw();
		//
		//Line Test3;
		//Test3.Init(TerrainVertice3.Arr, (TerrainVertice3 + v3(0.0f, 1.0f, 0.0f)).Arr, 10.0f);
		//Test3.Draw();

		v3 Line1 = (TerrainVertice2 - TerrainVertice);
		v3 Line2 = (TerrainVertice3 - TerrainVertice);
		v3 Norm = Normalize(CrossProduct(Line2, Line1));
		v3 PointLine = ObjectPosition - TerrainVertice;
		float test = -InnerProduct(PointLine, Norm);
		v3 point = test * Norm;

		ObjectPosition.y = ObjectPosition.y + point.y + 0.7f;
		return true;
	}
	else
	{
		return false;
	}
}