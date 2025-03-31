////////////////////////////////
/*     This file defines      */
/* legacy icosphere functions */
/*     in case I want them    */
////////////////////////////////

static int AppendMidPoint(int a, int b, FDynamicMesh3* EditMesh) {
	auto v1 = EditMesh->GetVertex(a);
	auto v2 = EditMesh->GetVertex(b);
	FVector3d mid = (v1 + v2) / 2;

	mid.Normalize();
	//mid *= (1.0 + FMath::Sqrt(5.0)) / 2.0;
	return EditMesh->AppendVertex(mid);
}
static void AppendIcoSphere(FDynamicMesh3* EditMesh)
{
	int32 iterations = 4;
	double t = (1.0 + FMath::Sqrt(5.0)) / 2.0;
	//inital vertices
	EditMesh->AppendVertex(FVector3d(-1, t, 0));
	EditMesh->AppendVertex(FVector3d(1, t, 0));
	EditMesh->AppendVertex(FVector3d(-1, -t, 0));
	EditMesh->AppendVertex(FVector3d(1, -t, 0));

	EditMesh->AppendVertex(FVector3d(0, -1, t));
	EditMesh->AppendVertex(FVector3d(0, 1, t));
	EditMesh->AppendVertex(FVector3d(0, -1, -t));
	EditMesh->AppendVertex(FVector3d(0, 1, -t));

	EditMesh->AppendVertex(FVector3d(t, 0, -1));
	EditMesh->AppendVertex(FVector3d(t, 0, 1));
	EditMesh->AppendVertex(FVector3d(-t, 0, -1));
	EditMesh->AppendVertex(FVector3d(-t, 0, 1));


	//initial faces
	EditMesh->AppendTriangle(0, 11, 5);
	EditMesh->AppendTriangle(0, 5, 1);
	EditMesh->AppendTriangle(0, 1, 7);
	EditMesh->AppendTriangle(0, 7, 10);
	EditMesh->AppendTriangle(0, 10, 11);

	EditMesh->AppendTriangle(1, 5, 9);
	EditMesh->AppendTriangle(5, 11, 4);
	EditMesh->AppendTriangle(11, 10, 2);
	EditMesh->AppendTriangle(10, 7, 6);
	EditMesh->AppendTriangle(7, 1, 8);

	EditMesh->AppendTriangle(3, 9, 4);
	EditMesh->AppendTriangle(3, 4, 2);
	EditMesh->AppendTriangle(3, 2, 6);
	EditMesh->AppendTriangle(3, 6, 8);
	EditMesh->AppendTriangle(3, 8, 9);

	EditMesh->AppendTriangle(4, 9, 5);
	EditMesh->AppendTriangle(2, 4, 11);
	EditMesh->AppendTriangle(6, 2, 10);
	EditMesh->AppendTriangle(8, 6, 7);
	EditMesh->AppendTriangle(9, 8, 1);

	//Refine sphere
	for (int j = 0; j < iterations; j++)
	{
		TArray<FIntVector3> trisToAdd;
		TArray<int> trisToRemove;
		//calculate new faces
		for (int i = 0; i < EditMesh->TriangleCount(); i++)
		{
			auto tri = EditMesh->GetTriangle(i);
			//tri indices for new points between old ones
			int a = tri.A;
			int b = tri.B;
			int c = tri.C;
			int d = AppendMidPoint(a, b, EditMesh);
			int e = AppendMidPoint(b, c, EditMesh);
			int f = AppendMidPoint(c, a, EditMesh);
			trisToRemove.Add(i);
			//add new tris
			trisToAdd.Add(FIntVector3(a, d, f));
			trisToAdd.Add(FIntVector3(b, e, d));
			trisToAdd.Add(FIntVector3(c, f, e));
			trisToAdd.Add(FIntVector3(d, e, f));
		}
		//remove old faces
		for (int tri : trisToRemove) {
			if (EditMesh->IsTriangle(tri)) {
				EditMesh->RemoveTriangle(tri, false, false);
			}
			else {
				int asda = 2;
			}
		}
		//re-add new faces
		for (auto tri : trisToAdd) {
			EditMesh->AppendTriangle(tri.X, tri.Y, tri.Z);
		}
	}


	//normalise all vertices
	for (int i = 0; i < EditMesh->VertexCount(); i++) {
		auto v = EditMesh->GetVertex(i);
		v.Normalize();
		v *= 1000;
		EditMesh->SetVertex(i, v);
	}
	//UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(

	EditMesh->ReverseOrientation(true);

	if (EditMesh->HasAttributes() && EditMesh->Attributes()->PrimaryNormals() != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MESH HAS ATTRIBS YOUR FLIPPING MIGHT BE FUCKED LOL"));
	}



}
