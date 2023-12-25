// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim


#include "Actors/PlacementActor.h"
#include "Components/SphereComponent.h"
#include "Components/PlacementManagerComponent.h"
#include "placementSystem.h"

// Sets default values
APlacementActor::APlacementActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!root) {
		root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
		SetRootComponent(root);
	}
	
	if (!collision) {
		collision = CreateDefaultSubobject<USphereComponent>(FName("Collsion"));
		collision->SetCollisionObjectType(ECC_WorldStatic);
		collision->SetupAttachment(root);
	}

}

// Called when the game starts or when spawned
void APlacementActor::BeginPlay()
{
	Super::BeginPlay();
	
	currentScale = normalScale;

}

// Called every frame
void APlacementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// whether to animate hover
	if (bShouldScaleOnHover)
		AnimateHover(DeltaTime);

}

void APlacementActor::OnPreviewPlacement()
{
	OnPreviewPlacement_BP();
}

void APlacementActor::AnimateHover(float DeltaTime)
{
	if( (bIsHovered &&  !FMath::IsNearlyEqual(currentScale , hoveredScale,0.005))
		||
		(!bIsHovered &&   !FMath::IsNearlyEqual(currentScale, normalScale,0.005)) )
	{
		currentScale += scalingSpeed * DeltaTime * (bIsHovered ? 1 : -1);

		// how to clamp ?

		if (hoveredScale > normalScale) {
			currentScale = FMath::Clamp(currentScale , normalScale , hoveredScale);
		}
		else {
			currentScale = FMath::Clamp(currentScale, hoveredScale , normalScale);
		}
		
		SetActorScale3D(FVector(currentScale));
	}

}

bool APlacementActor::RegisterForReplacement()
{
	if (PlacementComponent)
		return PlacementComponent->RegisterBuildingForRePlacement(this);
	else return false;
}

void APlacementActor::OnPlaced(UPlacementManagerComponent* PlacementComponentRef)
{
	ensure(PlacementComponentRef, "Error APlacementActor::OnPlaced PlacementSystem Component reference is Invalid !");
	PlacementComponent = PlacementComponentRef;
	OnPlaced_BP();
}

void APlacementActor::OnSelected()
{
	OnSelected_BP();
}

void APlacementActor::OnHovered()
{
	if (bIsHovered) return;

	bIsHovered = true;
	OnHovered_BP();
}

void APlacementActor::OnUnHovered()
{
	if (!bIsHovered) return;

	bIsHovered = false;
	OnUnHovered_BP();
}

