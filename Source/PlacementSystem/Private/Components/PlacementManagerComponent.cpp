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
	else {
		// track under the mouse 
		CheckUnderCursorForSelection();
	}
	

}

void UPlacementManagerComponent::SetGridSettings(FGridSettingsData newdata)
{
	GridSettings = newdata;
}

APlayerController* UPlacementManagerComponent::GetPlayerController() const
{

	if (Cast<APawn>(GetOwner())) {
		return Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
	}

	Print_Error("The Owner Is Not a Pawn !   UPlacementManagerComponent");
	return nullptr;

}

void UPlacementManagerComponent::ApplyGridSettings(FTransform& transform)
{
	if (!GridSettings.bApplyGrid || !CurrentPlacementData.bApplyGridRules) return;

	FVector location = transform.GetLocation();

	int32 XCells = location.X / GridSettings.GridSize;
	int32 YCells = location.Y / GridSettings.GridSize;

	location.X = XCells * GridSettings.GridSize;
	location.Y = YCells * GridSettings.GridSize;

	if (GridSettings.bSnapToGridCellCenter) {
		location.X += GridSettings.GridSize / 2.0f;
		location.Y += GridSettings.GridSize / 2.0f;
	}

	transform.SetLocation(location);

}

bool UPlacementManagerComponent::GetUnderCursorTrans(FTransform& UnderCursorTrans) 
{
	if (GetPlayerController()) {
		FHitResult h;
		if (GetPlayerController()->GetHitResultUnderCursor(ECC_Visibility, 0, h)) {
			UnderCursorTrans.SetLocation(h.ImpactPoint);

			ApplyGridSettings(UnderCursorTrans);

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
	if (_previewPlacement && bstartPlacing) {

		// apply offset befor updating location and rotation
		PlacementTransform.SetLocation(PlacementTransform.GetLocation() + CurrentPlacementData.LocationOffset);
		FRotator rot = PlacementTransform.GetRotation().Rotator();
		rot.Yaw += CurrentPlacementData.Additional_Placement_time_Yaw;
		PlacementTransform.SetRotation(rot.Quaternion());
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
		_previewPlacement->OnPlaced(this);
		_previewPlacement = nullptr;
	}

}


void UPlacementManagerComponent::Placement_Start(FPlacementData data)
{
	if (bstartPlacing) return;


	CurrentPlacementData = data;
	if (!_previewPlacement) {
		if (_previewPlacement = GetWorld()->SpawnActor<APlacementActor>(data.ActorToPlace)) {
			_previewPlacement->OnPreviewPlacement();
			bstartPlacing = true;
		}
	}
}



void UPlacementManagerComponent::ApplyYawRotationToPlacement(float Yaw)
{
	if (!bstartPlacing) return;

	CurrentPlacementData.Additional_Placement_time_Yaw += Yaw;
}

bool UPlacementManagerComponent::CheckUnderCursorForSelection()
{
	if (!GetPlayerController()) return false;

	FHitResult h;
	if (GetPlayerController()->GetHitResultUnderCursor(ECC_WorldStatic, 0, h)) {
		
		if (APlacementActor* hitted = Cast<APlacementActor>(h.GetActor())) {
		
			// hover when there is no hovered building
			if (!_HoveredPlacement) {
				_HoveredPlacement = hitted;
				_HoveredPlacement->OnHovered();
			
			}
			// when there is already hovered building and hovered a new one 
			else if (_HoveredPlacement && _HoveredPlacement != hitted) {
				_HoveredPlacement->OnUnHovered();
				_HoveredPlacement = hitted;
				_HoveredPlacement->OnHovered();
			}

		}
		else {
			// unhover the already hovered placement buildings
			if (_HoveredPlacement) {
				_HoveredPlacement->OnUnHovered();
				_HoveredPlacement = nullptr;
			}

		}

	}


	return false;
}

bool UPlacementManagerComponent::RegisterBuildingForReplacement(APlacementActor* building)
{
	if (bstartPlacing) return false;

	return false;
}
