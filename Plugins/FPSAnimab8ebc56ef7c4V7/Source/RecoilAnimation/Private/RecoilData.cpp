// Designed by KINEMATION, 2025. All rights reserved.

#include "RecoilData.h"
#include "PluginSettings.h"
#include "RecoilAnimationComponent.h"
#include "Misc/PackageName.h"
#include "UObject/SavePackage.h"

void URecoilData::SaveData()
{
	/** Getting the full path to the file */
	const FString FilePath = FPackageName::LongPackageNameToFilename(GetPackage()->GetPathName(),
																	 FPackageName::GetAssetPackageExtension());

	/** Finally saving just created package */
	FSavePackageArgs SavePackageArgs;

	SavePackageArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	SavePackageArgs.Error = GError;
	SavePackageArgs.bForceByteSwapping = true;
	SavePackageArgs.bWarnOfLongFilename = true;
	SavePackageArgs.SaveFlags = SAVE_NoError;

	GetPackage()->SetDirtyFlag(true);
	const bool bSuccess = UPackage::SavePackage(GetPackage(), this, *FilePath, SavePackageArgs);
	
	UE_LOG(LogRecoilAnimation, Warning, TEXT("Saved Package: %s, location: %s"),
	       bSuccess ? TEXT("SUCCESS") : TEXT("ERROR"), *FilePath)
}