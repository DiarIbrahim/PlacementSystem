// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actors/placementActor.h"
#include "PlacmentSystemTypes.generated.h"

/*
	is datas needed to start placment process
*/
USTRUCT(BlueprintType)
struct FPlacementData {
	GENERATED_BODY()



	// cpp only

	/*
		rotates the mesh after spawn around Z axis by this amount
	*/
	float Additional_Placement_time_Yaw = 0;


	// exposed to BP

	/*
		the item needed to be placed in the world 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APlacementActor> ActorToPlace;
	/*
		whether to align th ebuilding to the normal of the surface or keep it facing Up Axis (Z)
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAlignToSurfaceNormal = true;

	/*
		the positional offset to be applyed to the placement actor
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LocationOffset;

	/*
		whether to apply grid rule to location, the building can not be placed freely, it will snap to certain points based on grid settings
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bApplyGridRules = true;


};


/*
	Settings used in grid proccessing
*/
USTRUCT(BlueprintType)
struct FGridSettingsData {
	GENERATED_BODY()

	FGridSettingsData() {}

	/*
		size (x,y) of the grids ( in cm )
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double GridSize = 100;
	
	/*
		whether to snap the location to the center of the cell rather than corner of the grid cell
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSnapToGridCellCenter = false;

	/*
		whether to apply the grid or not 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bApplyGrid = true;


};

UCLASS()
class PLACEMENTSYSTEM_API UPlacmentSystemTypes : public UObject
{
	GENERATED_BODY()
	
};
