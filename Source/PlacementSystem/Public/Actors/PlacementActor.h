// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacementActor.generated.h"

UCLASS()
class PLACEMENTSYSTEM_API APlacementActor : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	APlacementActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
