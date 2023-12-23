// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim


#include "Actors/PlacementActor.h"
#include "placementSystem.h"

// Sets default values
APlacementActor::APlacementActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlacementActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlacementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

