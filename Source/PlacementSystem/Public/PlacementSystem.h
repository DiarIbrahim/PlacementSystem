// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"



void Print(FString message , int32 key = -1) {
	GEngine->AddOnScreenDebugMessage(key , 5 , FColor::Blue , message);
	UE_LOG(LogTemp, Warning, TEXT("s%"), *message);
}
void Print_Error(FString message) {
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, message);
	UE_LOG(LogTemp , Error , TEXT("s%"), *message);
}


class FPlacementSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
