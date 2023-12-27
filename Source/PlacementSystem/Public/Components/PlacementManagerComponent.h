// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/PlacmentSystemTypes.h"
#include "PlacementManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLACEMENTSYSTEM_API UPlacementManagerComponent : public UActorComponent
{
	GENERATED_BODY()


	// Internal Variables
	class UCameraComponent* _camera;
	/*
		the placement actor spawned to the world as preview and its placement not yet been accepted
	*/
	class APlacementActor* _previewPlacement;

	/*
		the placement actor spawned to the world as preview and its placement not yet been accepted	
	*/
	class APlacementActor* _HoveredPlacement;

	/*
		the placement actor spawned to the world as preview and its placement not yet been accepted
	*/
	class APlacementActor* _SelectedPlacement;

	/*
		the current(last) data used for placement
	*/
	struct FPlacementData CurrentPlacementData;

protected:
	/*
		the placement actor spawned to the world as preview and its placement not yet been accepted
	*/
	class AGridManager* _GridManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Placement Component | Grid")
	TSubclassOf<class AGridManager>CustomGridClass;

	
	/*
		settings for grid processing
	*/
	UPROPERTY(BlueprintReadWrite , EditAnywhere , Category = "Placement Component | Grid")
	struct FGridSettingsData GridSettings;

	// internal functions
	
	// returns player controller of this player
	class APlayerController* GetPlayerController()const;

	// Called when the game starts
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadWrite , EditAnywhere , Category = "Placement System | ")

	// is started placing buildings
	bool bstartPlacing = false;
	// is has a selected building to replace
	bool bIsReplacing = false;
	// the selected builing for replacing, the transform befor starting to treplace
	FTransform _Pre_Replace_Selected_Transform;


public:	
	// Sets default values for this component's properties
	UPlacementManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
		returns transform under the cursor 
		applys grid rules acording to @GridSettings	
	*/
	UFUNCTION(BlueprintCallable)
	bool GetUnderCursorTrans(FTransform& UnderCursorLoc);


	/*
		updates the spawned Placment mesh's location and rotation, this will be called when the placment started but not yet accepted or canceled !
	*/
	void UpdatePreviewMewsh(FTransform PlacementTransform);


	/*
		updates the a selected buildings location and rotation,!
	*/
	void UpdateSelectedForReplacement(FTransform PlacementTransform);


	/*
		can be called to start placing the passed static mesh ( @mesh ) to the world !
	*/
	UFUNCTION(BlueprintCallable)
	void Placement_Start(struct FPlacementData Data);

	/*
		this can be called to cancel placement process (if already started)
	*/
	UFUNCTION(BlueprintCallable)
	void Placement_Cancel();

	/*
		this can be called to verify and accept placement process (if already started)
		this request will be ignored if the the placment conditions does not match !
	*/
	UFUNCTION(BlueprintCallable)
	void Placement_Accept();

	/*
		applys rotation around Z axis to the placement process, this will be applyed only if called after calling  Placement_Start() function
		so please call it when you see the preview of the placement mesh

	*/
	UFUNCTION(BlueprintCallable)
	void ApplyYawRotationToPlacement(float Yaw);

	/*
		check the location under the cursor for classes of type APlacementActor
		and and selectes it !
	*/
	bool CheckUnderCursorForSelection();

	/*
		selects currently hovered item (Of Type PlacementActor)
	*/
	UFUNCTION(BlueprintCallable)
	void SelectUnderCursor();

		
	/*
		to start replacing an already placed/spawned building
	*/
	UFUNCTION(BlueprintCallable)
	bool Replacement_Start(class APlacementActor* building);

	/*
		to accept the replacement process and place the currently selected building in proper position
	*/
	UFUNCTION(BlueprintCallable)
	void Replacement_Accept();

	/*
		to reject the replacement process and place the currently selected building to its original transform
	*/
	UFUNCTION(BlueprintCallable)
	void Replacement_Cancel();


};

