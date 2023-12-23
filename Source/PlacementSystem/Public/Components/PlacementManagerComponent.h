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
		the current(last) data used for placement
	*/
	struct FPlacementData CurrentPlacementData;

	// internal functions
	class APlayerController* GetPlayerController()const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadWrite , EditAnywhere , Category = "Placement System | ")

	bool bstartPlacing = false;

public:	
	// Sets default values for this component's properties
	UPlacementManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool GetUnderCursorTrans(FTransform& UnderCursorLoc) const;

	/*
		updates the spawned Placment mesh's location and rotation, this will be called when the placment started but not yet accepted or canceled !
	*/
	void UpdatePreviewMewsh(FTransform PlacementTransform);



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


};
