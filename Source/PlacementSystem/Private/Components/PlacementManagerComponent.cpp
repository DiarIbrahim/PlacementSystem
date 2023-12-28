// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#include "Components/PlacementManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Actors/PlacementActor.h"
#include "Actors/GridManager.h"
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

		if (CustomGridClass) {
			_GridManager = GetWorld()->SpawnActor<AGridManager>(CustomGridClass);
			_GridManager->Init(this);
		}
		else {
			_GridManager = GetWorld()->SpawnActor<AGridManager>();
			_GridManager->Init(this);
			_GridManager->SetGridSettingsData(GridSettings);
		}

}


// Called every frame
void UPlacementManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bstartPlacing) {
		FTransform transform;
		if (GetUnderCursorTrans(transform)) {
			UpdatePreviewMewsh(transform);
			_GridManager->RedrawPlacementCells(_previewPlacement);
		}
	}
	else if (bIsReplacing) {
		FTransform transform;
		if (GetUnderCursorTrans(transform)) {
			UpdateSelectedForReplacement(transform);
			_GridManager->RedrawPlacementCells(_SelectedPlacement);
		}

	}
	else {
		// track under the mouse 
		CheckUnderCursorForSelection();
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


bool UPlacementManagerComponent::GetUnderCursorTrans(FTransform& UnderCursorTrans) 
{
	if (GetPlayerController()) {
		FHitResult h;
		if (GetPlayerController()->GetHitResultUnderCursor(ECC_Camera, 0, h)) {
			UnderCursorTrans.SetLocation(h.ImpactPoint);

			if (_GridManager) {
				 _GridManager->ApplyGridSettings(UnderCursorTrans);
			}

			if (CurrentPlacementData.bAlignToSurfaceNormal) {
				UnderCursorTrans.SetRotation(UKismetMathLibrary::MakeRotFromZX(h.ImpactNormal , FVector::ForwardVector).Quaternion());
			}
			else {
				UnderCursorTrans.SetRotation(FQuat::Identity);
			}

			return 1;
		}
	}

	return false;
}

void UPlacementManagerComponent::UpdatePreviewMewsh( FTransform PlacementTransform)
{
	// already spawned  just re possition it
	if (_previewPlacement && bstartPlacing) {

		// apply offset befor updating location and rotation
		PlacementTransform.SetLocation(PlacementTransform.GetLocation() + CurrentPlacementData.LocationOffset);
		FRotator rot = PlacementTransform.GetRotation().Rotator();
		rot.Yaw += CurrentPlacementData.Additional_Placement_time_Yaw;
		 PlacementTransform.SetRotation(rot.Quaternion());
		_previewPlacement->SetActorTransform(PlacementTransform);



		// update placement State
		if (_GridManager && _previewPlacement) {
			if (_GridManager->CanAddBuildingToGrid(_previewPlacement)) {
				_previewPlacement->OnPlacementProcessValidation(GetWorld()->GetDeltaSeconds() ,true , 1);
			}
			else {
				_previewPlacement->OnPlacementProcessValidation(GetWorld()->GetDeltaSeconds(), false, 1);
			}
		}

	}
}

void UPlacementManagerComponent::UpdateSelectedForReplacement(FTransform PlacementTransform)
{
	if (_SelectedPlacement && bIsReplacing) {
		_SelectedPlacement->SetActorTransform(PlacementTransform);
		if (_GridManager->CanAddBuildingToGrid(_SelectedPlacement)) {
			_SelectedPlacement->OnPlacementProcessValidation(GetWorld()->GetDeltaSeconds(), true, 0);
		}
		else {
			_SelectedPlacement->OnPlacementProcessValidation(GetWorld()->GetDeltaSeconds(), false, 0);
		}
	}
}
void UPlacementManagerComponent::Placement_Start(FPlacementData data)
{
	if (bstartPlacing) return;

	// unselect already selected placment
	if (_SelectedPlacement) {
		_SelectedPlacement->OnUnSelected();
		_SelectedPlacement = nullptr;
	}

	CurrentPlacementData = data;
	if (!_previewPlacement) {
		if (_previewPlacement = GetWorld()->SpawnActor<APlacementActor>(data.ActorToPlace)) {
			_previewPlacement->OnPreviewPlacement();
			bstartPlacing = true;
		}
	}
}

void UPlacementManagerComponent::Placement_Accept()
{
	if (!bstartPlacing || !_GridManager || !_GridManager->CanAddBuildingToGrid(_previewPlacement)) {
		Placement_Cancel();
		return;
	}

	bstartPlacing = false;
	if (_previewPlacement) {
		_previewPlacement->OnPlaced(this);
		_GridManager->AddBuildingToGrid(_previewPlacement);
		_GridManager->ClearCellDrawing(0);
		_previewPlacement = nullptr;
	}

}

void UPlacementManagerComponent::Placement_Cancel()
{
	if (!bstartPlacing) return;
	bstartPlacing = false;

	if (_previewPlacement) {
		_previewPlacement->Destroy(); 
		_GridManager->ClearCellDrawing(0);
		_previewPlacement = nullptr;
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

void UPlacementManagerComponent::SelectUnderCursor()
{               
	//   placing new  OR   replacing  OR                clicking already slected building              
	if (bstartPlacing || bIsReplacing || (_HoveredPlacement && _SelectedPlacement == _HoveredPlacement)) return;


	if (_HoveredPlacement ) {
		
		// if already seleced, just unselect
		if (_SelectedPlacement) {
			_SelectedPlacement->OnUnSelected();
			_SelectedPlacement = nullptr;
		}
		// selct newOne 
		_SelectedPlacement = _HoveredPlacement;
		_SelectedPlacement->OnSelected();

	}
	else if (_SelectedPlacement){
		_SelectedPlacement->OnUnSelected();
		_SelectedPlacement = nullptr;
	}
	
}


bool UPlacementManagerComponent::Replacement_Start(APlacementActor* building)
{
	if (bIsReplacing || bstartPlacing || !building) return false;

	_Pre_Replace_Selected_Transform = building->GetActorTransform();
	bIsReplacing = true;
	_SelectedPlacement = building;
	_SelectedPlacement->OnReplace_Started();
	return true;
}

void UPlacementManagerComponent::Replacement_Accept()
{
	if (!bIsReplacing || !_GridManager || !_GridManager->CanAddBuildingToGrid(_SelectedPlacement)) {
		Replacement_Cancel();
		return;
	}

	bIsReplacing = false;
	_SelectedPlacement->OnReplaced();
	_GridManager->ReplaceBuilding(_SelectedPlacement);
	_GridManager->ClearCellDrawing(0);
	_SelectedPlacement = nullptr;

}

void UPlacementManagerComponent::Replacement_Cancel()
{
	if (!bIsReplacing) return;

	bIsReplacing = false;
	_SelectedPlacement->SetActorTransform(_Pre_Replace_Selected_Transform);
	_SelectedPlacement->OnReplaced();
	_GridManager->ClearCellDrawing(0);
	_SelectedPlacement = nullptr;

}

void UPlacementManagerComponent::RemoveBuilding(APlacementActor* toRemove)
{
	if (_SelectedPlacement == toRemove) {
		_SelectedPlacement = nullptr;
	}
	if (_HoveredPlacement == toRemove) {
		_HoveredPlacement = nullptr;
	}

	if (_GridManager) {
		_GridManager->OnBuildingRemoved(toRemove);
	}


	toRemove->Destroy();
}

