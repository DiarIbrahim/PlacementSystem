// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim


#include "Actors/GridManager.h"
#include "ProceduralMeshComponent.h"
#include "PlacementSystem.h"

AGridManager::AGridManager()
{
	if (!ProceduralMesh) {
		ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(FName("mesh"));
		ProceduralMesh->SetupAttachment(GetRootComponent());
	}
}

void AGridManager::Init(UPlacementManagerComponent* _Owner)
{
	ensureMsgf(_Owner , TEXT("Owner of AGridManager must be valid !  AGridManager::Init"));
	_placementManager = _Owner;

}

void AGridManager::SetGridSettingsData(FGridSettingsData data)
{
	_gridSettings = data;

}



void AGridManager::ApplyGridSettings(FTransform& transform )
{

	FVector location = transform.GetLocation();
	int32 XCells = location.X / _gridSettings.GridSize;
	int32 YCells = location.Y / _gridSettings.GridSize;

	location.X = XCells * _gridSettings.GridSize;
	location.Y = YCells * _gridSettings.GridSize;

	if (_gridSettings.bSnapToGridCellCenter) {
		location.X += _gridSettings.GridSize / 2.0f;
		location.Y += _gridSettings.GridSize / 2.0f;
	}

	transform.SetLocation(location);
}

TArray<FGridCell> AGridManager::GetCellsNeededForBuilding(APlacementActor* actorToPlace) const
{
	FVector location = actorToPlace->GetActorLocation();
	int32 XCells = location.X / _gridSettings.GridSize;
	int32 YCells = location.Y / _gridSettings.GridSize;

	TArray<FGridCell> cellsNeeded;
	for (size_t i = 0; i < actorToPlace->width_Cell; i++) {
		for (size_t j = 0; j < actorToPlace->depth_Cell; j++) {
			cellsNeeded.Add(FGridCell(XCells + i, YCells + j));
		}
	}

	return cellsNeeded;
}

bool AGridManager::CanAddBuildingToGrid(APlacementActor* actorToPlace) const
{
	if (!actorToPlace) { Print("actor to Place  is null "); return false; };

	TArray<FGridCell> cellsNeeded = GetCellsNeededForBuilding(actorToPlace);

	for (FGridCell c : reservedCells)
	{
		for (FGridCell c2 : cellsNeeded) {
			if ( ( c.IsValid() && c.IsEqual(c2) && !c.IsTheSameReserver(actorToPlace->GetBuildingId())) ) {
				return false;
			}
		}
	}

	return true;
}

FGridCell AGridManager::LocationToCell(FVector Location)
{
	int32 XCells = Location.X / _gridSettings.GridSize;
	int32 YCells = Location.Y / _gridSettings.GridSize;
	return FGridCell(XCells,YCells);
}

bool AGridManager::AddBuildingToGrid(APlacementActor* ToPlace)
{
	if (!ToPlace) return false;


	if (CanAddBuildingToGrid(ToPlace)) {
		TArray<FGridCell> neededcells = GetCellsNeededForBuilding(ToPlace);
		ToPlace->SetReserverCells(neededcells);
		for (FGridCell& c : neededcells) {
			c.Reserve(ToPlace->GetBuildingId());
			reservedCells.Add(c);
		}
	}

	return true;
}

void AGridManager::ReplaceBuilding(APlacementActor* ToPlace)
{
	if (!ToPlace) return;

	for (FGridCell c : ToPlace->GetReservedCells()) {
		for (FGridCell c2 : reservedCells) {
			if (c.IsEqual(c2)) {
				reservedCells.Remove(c2);
				break;
			}
		}
	}

	if (CanAddBuildingToGrid(ToPlace)) {
		TArray<FGridCell> cells = GetCellsNeededForBuilding(ToPlace);
		ToPlace->SetReserverCells(cells);
		for (FGridCell c : cells) {
			c.Reserve(ToPlace->GetBuildingId());
			reservedCells.Add(c);
		}
	}

}

void AGridManager::OnBuildingRemoved(APlacementActor* ToRemove)
{
	// free reserved cells
	for (size_t i = 0; i < ToRemove->GetReservedCells().Num(); i++) {
		for (int j = 0; j < reservedCells.Num(); j++) {
			if (reservedCells[j].IsEqual(ToRemove->GetReservedCells()[i])) {
				reservedCells.RemoveAt(j);
				break;
			}
		}
	}
}


FGridCell LastdrawnLocation;
void AGridManager::RedrawPlacementCells(APlacementActor* toPlace)
{

	// check if this building in this particular location is already drawn preview cells !
	if (LocationToCell(toPlace->GetActorLocation()).IsEqual(LastdrawnLocation) && toPlace->GetBuildingId() == LastdrawnLocation.reserverBuildingId) return;
	LastdrawnLocation = LocationToCell(toPlace->GetActorLocation());
	LastdrawnLocation.reserverBuildingId = toPlace->GetBuildingId();
	
	ClearCellDrawing(0);
	DrawCells(GetCellsNeededForBuilding(toPlace),0, grid_preview_padding ,Grid_placment_Previrew_material, FVector(0,0,toPlace->GetActorLocation().Z + 2));
}

/*
	data for drawing a cell
*/
struct DrawCellData {
	DrawCellData() {}
	DrawCellData(TArray<FVector> _vectors , TArray<int> _tringles) {
		tringles = _tringles ;
		vectors = _vectors;
	}
	TArray<FVector> vectors;
	TArray<int> tringles;

	/*
	returns vectors and trignles for each cell !
	*/
	static DrawCellData MakeCellDrawData(FGridCell cell, float cell_size , FVector offset = FVector() , float padding = 10.0)
	{
		TArray<FVector> vectors = {
			FVector(cell_size * cell.x+ padding ,cell_size * cell.y + padding,0)+ offset ,
			FVector(cell_size * cell.x + cell_size  - padding,cell_size * cell.y +padding,0)+ offset ,
			FVector(cell_size * cell.x + cell_size -padding,cell_size * cell.y + cell_size - padding,0)+ offset ,
			FVector(cell_size * cell.x +padding ,cell_size * cell.y + cell_size - padding,0) + offset };
		TArray<int> tringles = { 2,1,0   ,0,3,2 };

		return DrawCellData(vectors,tringles);
	}
};
// data for drawing multiple cells / mesh 
struct DrawData {
	DrawData() {

	}
	TArray<FVector> vectors;
	TArray<int> tringles;

	static DrawData MakeSetOfCells(TArray<FGridCell> cells, float cell_size, FVector offset = FVector() , float padding = 10.0) {
		TArray<DrawCellData> CellsData;
		for (FGridCell c : cells) {
			CellsData.Add(DrawCellData::MakeCellDrawData(c, cell_size, offset , padding));
		}
		DrawData drawData = DrawData();

		for (int i = 0; i < CellsData.Num(); i++) {
			for (int j = 0; j < CellsData[i].vectors.Num(); j++) {
				drawData.vectors.Add(CellsData[i].vectors[j]);
			}
			for (int j = 0; j < CellsData[i].tringles.Num(); j++) {
				drawData.tringles.Add(CellsData[i].tringles[j] + 4 * i);
			}
		}
		return drawData;
	}
};

void AGridManager::DrawCells(TArray<FGridCell> cells , int meshIndex , float Padding , UMaterialInterface* CustomCellDrawMaterial , FVector offset)
{
	DrawData d = DrawData::MakeSetOfCells(cells, _gridSettings.GridSize, offset, Padding);

	if (ProceduralMesh) {
		ProceduralMesh->CreateMeshSection(meshIndex, d.vectors, d.tringles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), 0);
		ProceduralMesh->SetMaterial(meshIndex, CustomCellDrawMaterial);

	}
}

void AGridManager::ClearCellDrawing(int Meshindex , bool bAllSections)
{
	if (!ProceduralMesh) return;
	if (bAllSections) {
		ProceduralMesh->ClearAllMeshSections();
	}
	else {
		ProceduralMesh->ClearMeshSection(Meshindex);
	}

}

