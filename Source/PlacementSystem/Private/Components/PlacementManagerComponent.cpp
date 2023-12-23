// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#include "Components/PlacementManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Actors/PlacementActor.h"
#include "PlacementSystem.h"





// Sets default values for this component's properties
UPlacementManagerComponent::UPlacementManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlacementManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdatePreviewMewsh(FTransform());
}


// Called every frame
void UPlacementManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bstartPlacing) {
		FTransform transform;
		if (GetUnderCursorTrans(transform)) {
			UpdatePreviewMewsh(transform);
		}
	}
	

}

APlayerController* UPlacementManagerComponent::GetPlayerController() const
{

	if (Cast<APawn>(GetOwner())) {
		return Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
	}

	Print_Error("The Owner Is Not a Pawn !   UPlacementManagerComponent");
	return nullptr;

}

bool UPlacementManagerComponent::GetUnderCursorTrans(FTransform& UnderCursorTrans) const
{
	if (GetPlayerController()) {
		FHitResult h;
		if (GetPlayerController()->GetHitResultUnderCursor(ECC_Visibility, 0, h)) {
			UnderCursorTrans.SetLocation(h.ImpactPoint);
			
			if (CurrentPlacementData.bAlignToSurfaceNormal) {
				UnderCursorTrans.SetRotation(UKismetMathLibrary::MakeRotFromZ(h.ImpactNormal).Quaternion());
			}
			else {
				UnderCursorTrans.SetRotation(FQuat::Identity);
			}

			return 1;
		}
	}

	return false;
}

void UPlacementManagerComponent::UpdatePreviewMewsh(FTransform PlacementTransform)
{
	// already spawned  just re possition it
	if (_previewPlacement) {

		// apply offset befor updating location and rotation
		PlacementTransform.SetLocation(PlacementTransform.GetLocation() + CurrentPlacementData.LocationOffset);
		//PlacementTransform.SetRotation(PlacementTransform.GetRotation() + CurrentPlacementData.RotationOffset.Quaternion());
		
		
		_previewPlacement->SetActorTransform(PlacementTransform);
	}
}

void UPlacementManagerComponent::Placement_Cancel()
{
	bstartPlacing = false;

	if (_previewPlacement) {
		_previewPlacement->Destroy();
	}
}

void UPlacementManagerComponent::Placement_Accept()
{
	bstartPlacing = false;
	if (_previewPlacement) {
		_previewPlacement = nullptr;
	}

}

void UPlacementManagerComponent::Placement_Start(FPlacementData data)
{
	CurrentPlacementData = data;
	if (!_previewPlacement) {
		if (_previewPlacement = GetWorld()->SpawnActor<APlacementActor>(data.ActorToPlace)) {
			bstartPlacing = true;
		}
	}
}


