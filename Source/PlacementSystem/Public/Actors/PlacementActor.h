// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacementActor.generated.h"

UCLASS()
class PLACEMENTSYSTEM_API APlacementActor : public AActor
{
	GENERATED_BODY()

	// draws debug gizmo for the cells this building will require !
	void DrawCellGizmo();

	// called when ever the bp constructed!
	void OnConstruction(const FTransform& Transform) override;

	TArray<struct FGridCell> reservedCells;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Internal
	class UPlacementManagerComponent* PlacementComponent;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Placement System")
	class USceneComponent* root;
	// Components
	UPROPERTY(BlueprintReadWrite , EditAnywhere, Category = "Placement System")
	class UBoxComponent* collision;
	// Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Placement System")
	class UStaticMeshComponent* Mesh;


	FGuid BuildingId;
	
	// is this building hovered
	UPROPERTY(BlueprintReadOnly , VisibleAnywhere, Category = "Placement System | Cpp variables Exposed to BP")
	bool bIsHovered = false;
	// is this building seleceted
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Placement System | Cpp variables Exposed to BP")
	bool bIsSelected = false;


	/*
		whether this actor will automatically play scale-up on hover and scale-down on Unhover ?
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere , Category = "Placement System | Hover Animation")
	bool bShouldScaleOnHover = false;
	
	/*
		min scale, the scale this actor will animate to when unhovered ( normal scale )
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Hover Animation")
	float normalScale = 1.0f;
	/*
		max scale, the scale this actor will be scaled to when Hovered (hovered scale)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Hover Animation")
	float hoveredScale = 1.2f;
	/*
		scaling speed, how fast the scaling  animation will play 
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Hover Animation")
	float scalingSpeed = 3.0f;


	float currentScale = 1.0f;

public:	


	/*
		width in cell , how many grid cells required for this building in X direction, (grid cell)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Size and Grid")
	int32 width_Cell = 1;
	/*
		depth in cell, how many grid cells required for this building in Y direction, (grid cell)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Size and Grid")
	int32 depth_Cell = 1;
	/*
		grid size (visual only ) this value will only be used for gizmoz draw only ( will not effect the system at all)
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Placement System | Size and Grid")
	float cell_size = 100.0;


	bool bFirstTimeToPlace = true;

	
	// Sets default values for this actor's properties
	APlacementActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/*
		will be called when this blueprint was spawned as preview
		this object may not be spawned !
	*/
	void OnPreviewPlacement();

	/*
		hover animation process
	*/

	void AnimateHover(float DeltaTime);


	/*
		requestes a movement action for this placement actor,
		this request can be accepted declined.
		if accpted, allows this placement actor to be moved ( snapped to mouse cursor ) and placed else where
		returns true if accepted and false if declined
		NOTE : the placementActor must be seected in order for this function to work
	*/
	UFUNCTION(BlueprintCallable, Category = "Placement System")
	bool RegisterForReplacement();

	/*
		Removes the building from the world 
		and frees the already reserved cells so new buildings can be placed there !
	*/
	UFUNCTION(BlueprintCallable, Category = "Placement System")
	void RemoveBuilding();


	/*
		will be called when this blueprint was spawned as preview
		this object may not be spawned !
		can be Implemented on Blueprint 
	*/
	UFUNCTION(BlueprintImplementableEvent , META = (DisplayName = "OnPreviewPlacement"), Category = "Placement System")
	void OnPreviewPlacement_BP();
	/*
		called When this actor placed on world
	*/
	void OnPlaced(class UPlacementManagerComponent* PlacementComponentRef);
	/*
		called When this actor placed on world
		can be Implemented on Blueprint 
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnPlaced"), Category = "Placement System")
	void OnPlaced_BP();

	/*
		called when this actor selected by PlacemenManagerComponent
	*/
	void OnSelected();
	/*
		called when this actor selected by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnSelected"), Category = "Placement System")
	void OnSelected_BP();

	/*
		called when this actor Unselected by PlacemenManagerComponent
	*/
	void OnUnSelected();
	/*
		called when this actor Unselected by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnUnSelected"), Category = "Placement System")
	void OnUnSelected_BP();



	/*
		called when  replacement process started by PlacemenManagerComponent
	*/
	void OnReplace_Started();
	/*
		called when  replacement process started by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnReplace Started"), Category = "Placement System")
	void OnReplace_Started_BP();

	/*
		called when this actor replaced by PlacemenManagerComponent
	*/
	void OnReplaced();
	/*
		called when this actor replaced by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnReplaced"), Category = "Placement System")
	void OnReplaced_BP();



	/*
		called when this actor Hovered by PlacemenManagerComponent
	*/
	void OnHovered();
	/*
		called when this actor Hovered by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnHovered"), Category = "Placement System")
	void OnHovered_BP();


	/*
		called when this actor UnHovered by PlacemenManagerComponent
	*/
	void OnUnHovered();
	/*
		called when this actor UnHovered by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnUnHovered"), Category = "Placement System")
	void OnUnHovered_BP();

	/*
		this will be called when ever this building is on placing/replacing process
		and passes a boolean that determines if the current position is valid or not
		in an other word @bCanPlaceInCurrentLocation is true then the building can be placed at that particular location other wise it cant be placed there !
		
		this function will be called every frame !

		@param DeltaTime                   deltatime  of the world
		@param bCanPlaceInCurrentLocation  is it valid to place the building at thelocation
		@param bIsFirstSpwan               whether it is first time placement(first spawn) or replacement

	*/
	void OnPlacementProcessValidation(float DeltaTime, bool bCanPlaceInCurrentLocation , bool bIsFirstSpwan);

	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnPlacementProcessValidation"), Category = "Placement System")
	void OnPlacementProcessValidation_BP(float DeltaTime, bool bCanPlaceInCurrentLocation, bool bIsFirstSpwan);



	UFUNCTION(BlueprintPure, Category = "Placement System")
	FGuid GetBuildingId() const { return BuildingId; }


	TArray<struct FGridCell> GetReservedCells() { return reservedCells; }
	void SetReserverCells(TArray<struct FGridCell> newCells) { reservedCells = newCells; }


};
