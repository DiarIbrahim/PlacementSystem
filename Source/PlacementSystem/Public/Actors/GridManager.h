// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "Gameframework/actor.h"
#include "TYpes/PlacmentSystemTypes.h"
#include "GridManager.generated.h"

class UPlacementManagerComponent;

USTRUCT(BlueprintType)
struct FGridCell {
	GENERATED_BODY()

private :
	bool bIsValid = false;
	bool bIsReserved = false;

public:

	FGridCell(){}
	FGridCell(int32 _x , int32 _y){
		x = _x;
		y = _y;
		bIsValid = true;
	}

	// x and y index of cells
	UPROPERTY(BlueprintReadWrite , EditAnywhere, Category = "Placement System");
	int32 x;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Placement System");
	int32 y;

	FGuid reserverBuildingId;

	//static bool operator== (const FGridCell& A , const FGridCell& B) {
	//	return (B.x == A.x && B.y == A.y);
	//}

	// is the cell initalized
	bool IsValid()const { return bIsValid; }
	// is this cell already used
	bool IsFree()const { return !bIsReserved; }


	void Reserve(FGuid _reserverBuildingId){
		if (bIsReserved || !_reserverBuildingId.IsValid()) return;
		
		reserverBuildingId = _reserverBuildingId;
		bIsReserved = true;
	}

	bool IsEqual(const FGridCell& cell) {
		return (cell.x == x && cell.y == y);
	}

	bool IsTheSameReserver(FGuid NewreserverId) const {
		return reserverBuildingId == NewreserverId;
	}

	void Release() {	
		if (!bIsReserved) return;

		reserverBuildingId = FGuid();;
		bIsReserved = false;
	}

	bool operator==(const FGridCell& other) const {
		return  other.x == x && other.y == y;

	}

};

UCLASS()
class PLACEMENTSYSTEM_API AGridManager : public AActor
{
	GENERATED_BODY()

	AGridManager();
protected:



	// Componwents
	 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Placement System")
	// for generating custom meshes in runtime
	class UProceduralMeshComponent* ProceduralMesh;

	//  a reference to placementManagerComponent
	UPlacementManagerComponent* _placementManager;
	
	// settings for this grid 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "grid Settings ")
	FGridSettingsData _gridSettings;

	// all cells reserved by buildings
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category = "grid Settings | Cells")
	TArray<FGridCell> reservedCells;

	/*
		material used for preview cells
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere , category = "grid Settings ")
	UMaterialInterface* Grid_placment_Previrew_material; 

	// the padding for grid preview (0 the preview will fill the entir grid, otherwise adds padding)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "grid Settings ")
	float grid_preview_padding = 10;




public:

	// called after spawn to init
	void Init(UPlacementManagerComponent* _Owner);
	// update grid settings data
	void SetGridSettingsData(FGridSettingsData data);

	// snaps the location acording to the grid settings
	void ApplyGridSettings(FTransform& transform);

	/*
		returns a list of all needed cells to be reserved for this building to be placed !
	*/
	TArray<struct FGridCell> GetCellsNeededForBuilding(APlacementActor* actorToPlace) const;
	
	/*
		checks if this building can be placed on its current location
	*/
	bool CanAddBuildingToGrid(APlacementActor* actorToPlace)const;

	/*
		converts location to cell
	*/
	UFUNCTION(BlueprintCallable, Category = "Placement System")
	FGridCell LocationToCell(FVector Location);
	/*
		adds building to the grid for tyhe first time and reserves the reqyuired cells
	*/
	bool AddBuildingToGrid(class APlacementActor* ToPlace);
	/*
		called when a building replaced, so the already reserved cells will be freed, and new ones will be reserved
	*/
	void ReplaceBuilding(class APlacementActor* ToPlace);

	/*
		called when a building replaced, so the already reserved cells will be freed, and new ones will be reserved
	*/
	void OnBuildingRemoved(class APlacementActor* ToRemove);


	/*
		draws cells as mesh, on the grid in their proper position
		can be used for any kind of drawing 
		for example used it to indicate the used/reserved cells in the grid !

		@param cells					list of cells to draw
		@param meshIndex				works as an id for sectoins you are drawing (cells you are drawing)
		@param Padding					is the padding for drawing cells (bigger the padding, smaller cell will be drawed) (please make a quick search to see how padding works)
		@param CustomCellDrawMaterial   material used while drawing the cells (mesh)
		@param offset					offset to be applyed to the drawing mesh	 

	*/
	UFUNCTION(BlueprintCallable, Category = "Placement System")
	void DrawCells(TArray<FGridCell> cells , int meshIndex = 1 , float Padding = 10.0f, UMaterialInterface* CustomCellDrawMaterial = nullptr , FVector offset = FVector(0,0,60));
	/*
		re-draws cells for a praticular building
	*/
	void RedrawPlacementCells(class APlacementActor* ToPlace);
	UFUNCTION(BlueprintCallable, Category = "Placement System")
	// clears already drawn cells
	void ClearCellDrawing(int meshIndex, bool bAllSections = false);

};
