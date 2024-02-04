// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialFunctionInstance.h"
#include "Particles/ParticleSystem.h"
#include "Blueprint/UserWidget.h"
#include "MyAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class UNREALAUTOMATIONTOOL_API UMyAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()
public:
	/**
	 *  Rename the selected assets
	 *  @param SeachPattern - the pattern to search for
	 *  @param ReplacePattern - the pattern to replace with
	 *  @param SearchCase - should the case be ignored?
	 */
	UFUNCTION(CallInEditor)
	void RenameSelectedAsset(FString SeachPattern, FString ReplacePattern, ESearchCase::Type SearchCase);
	/**
	 * Check if a Texture is power of two texture
	 */
	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();
	
	/**
	 * Add Prefix on selected assets
	 */
	UFUNCTION(CallInEditor)
	void AddPrefix();

	/**
	 * Clean up assets by moving them to parent folder
	 * @param ParentFolder - the parent folder of selected assets.
	 */
	UFUNCTION(CallInEditor)
	void CleanupFolder(FString ParentFolder = FString("/Game"));

	/**
	 * Duplicates the selected assets
	 * @param NumberOfDuplicates - The amount of duplicates to create
	 * @param bSave - Should the duplicates be saved immediately?
	 */
	UFUNCTION(CallInEditor)
	void DuplicateAsset(uint32 NumberOfDuplicates = 1, bool bSave = true);

	/**
	 * Deletes unused Assets
	 * bDeleteImmediately - Delete them?
	 */
	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets(bool bDeleteImmediately);
private:

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UMaterialFunctionInstance::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UUserWidget::StaticClass(), TEXT("UW_")},
		{UMorphTarget::StaticClass(), TEXT("MT_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
	};

	/**
	 * Prints a message to the screen
	 * @param Message - the message to print
	 * @praam Color - the Color to use for the Message
	 */
	void PrintToScreen(FString Message, FColor Color);
	
	/**
	 * Give Feedback to the user
	 * @param Method - the Method that was used
	 * @param Counter - how many objects were affected
	 */
	void GiveFeedback(FString Method, uint32 Counter);

	/**
	 * Give Feedback to the user
	 * @param NumberToCheck - the Number to check if is power of two
	 */
	bool IsPowerOfTwo(int32 NumberToCheck);
};
