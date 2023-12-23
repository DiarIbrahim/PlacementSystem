// Developed by Diar Games    twitter :  https://twitter.com/diar__Ibrahim      github : https://github.com/DiarIbrahim

#include "PlacementSystem.h"

#define LOCTEXT_NAMESPACE "FPlacementSystemModule"

void FPlacementSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPlacementSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPlacementSystemModule, PlacementSystem)