// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacementActor.generated.h"

UCLASS()
class PLACEMENTSYSTEM_API APlacementActor : public AActor
{
	GENERATED_BODY()

	




protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Internal
	class UPlacementManagerComponent* PlacementComponent;


	// Components
	UPROPERTY(BlueprintReadWrite , EditAnywhere)
	class USphereComponent* collision;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneComponent* root;

	
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
	*/
	UFUNCTION(BlueprintCallable)
	bool RegisterForReplacement();

	/*
		will be called when this blueprint was spawned as preview
		this object may not be spawned !
		can be Implemented on Blueprint 
	*/
	UFUNCTION(BlueprintImplementableEvent , META = (DisplayName = "OnPreviewPlacement"))
	void OnPreviewPlacement_BP();
	/*
		called When this actor placed on world
	*/
	void OnPlaced(class UPlacementManagerComponent* PlacementComponentRef);
	/*
		called When this actor placed on world
		can be Implemented on Blueprint 
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnPlaced"))
	void OnPlaced_BP();

	/*
		called when this actor selected by PlacemenManagerComponent
	*/
	void OnSelected();
	/*
		called when this actor selected by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnSelected"))
	void OnSelected_BP();


	/*
		called when this actor replaced by PlacemenManagerComponent
	*/
	void OnReplaced();
	/*
		called when this actor replaced by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnReplaced"))
	void OnReplaced_BP();



	/*
		called when this actor Hovered by PlacemenManagerComponent
	*/
	void OnHovered();
	/*
		called when this actor Hovered by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnHovered"))
	void OnHovered_BP();


	/*
		called when this actor UnHovered by PlacemenManagerComponent
	*/
	void OnUnHovered();
	/*
		called when this actor UnHovered by PlacemenManagerComponent
		can be Implemented on Blueprint
	*/
	UFUNCTION(BlueprintImplementableEvent, META = (DisplayName = "OnUnHovered"))
	void OnUnHovered_BP();


};
