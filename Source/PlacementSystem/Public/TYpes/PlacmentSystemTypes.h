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

	/*
		the item needed to be placed
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APlacementActor> ActorToPlace;
	/*
		whether to align th ebuilding to the normal of the surface or keep it facing Up Axis (Z)
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAlignToSurfaceNormal = true;

	/*
		the rotational offset to be applyed to the placement actor
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator RotationOffset;
	
	/*
		the positional offset to be applyed to the placement actor
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LocationOffset;



};



UCLASS()
class PLACEMENTSYSTEM_API UPlacmentSystemTypes : public UObject
{
	GENERATED_BODY()
	
};
