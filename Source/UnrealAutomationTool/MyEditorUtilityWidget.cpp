// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEditorUtilityWidget.h"
#include "EditorAssetLibrary.h"
#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"

#pragma region Organize World Outliner
void UMyEditorUtilityWidget::OrganizeWorldOutliner()
{
	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
	
	for (AActor* Actor: AllActors)
	{
		if (ensure(Actor))
		{
			const FName* FolderName = FolderMap.Find(Actor->GetClass());
			if (ensure(FolderName))
			{
				Actor->SetFolderPath(*FolderName);
			}
		}
	}
}
#pragma endregion

#pragma region Delete Null SMActors
void UMyEditorUtilityWidget::DeleteNullSMActors()
{
	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
	
	for (AActor* Actor : AllActors)
	{
		if (ensure(Actor))
		{
			AStaticMeshActor* SaticMeshActor = dynamic_cast<AStaticMeshActor*>(Actor);
			if (ensure(SaticMeshActor))
			{
				UStaticMeshComponent* StaticMeshComponent = SaticMeshActor->GetStaticMeshComponent();
				if (ensure(StaticMeshComponent) && nullptr == StaticMeshComponent->GetStaticMesh())
				{
					Actor->Destroy();
				}
			}
		}
	}
}
#pragma endregion

#pragma region	Set Texture Parameter
void UMyEditorUtilityWidget::SetTextureParameter()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, false);
	for (FString AssetPath : AssetsInPath)
	{
		for (FString Pattern : LinearColorPattern)
		{
			if (AssetPath.Contains(Pattern))
			{
				UObject* Object = UEditorAssetLibrary::LoadAsset(AssetPath);
				if (!ensure(Object))
					return;

				UTexture2D* Texture = dynamic_cast<UTexture2D*>(Object);
				if (ensure(Texture))
				{
					Texture->SRGB = bSetRGB;
					Texture->CompressionSettings = Compression;
				}
			}
		}
	}
}

#pragma endregion

#pragma region Search And Replace
void UMyEditorUtilityWidget::SearchAndReplace()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, false);

	for (FString AssetPath: AssetsInPath)
	{
		//FString FileName = FPaths::Combine(SearchPattern, )
		FString FileName = FPaths::GetCleanFilename(AssetPath);
		if (FileName.Contains(SearchPattern, SearchCase))
		{
			FString NewAssetName = FileName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
			FString NewAssetPath = FPaths::Combine(FPaths::GetPath(AssetPath), NewAssetName);
			UEditorAssetLibrary::RenameAsset(AssetPath, NewAssetPath);
		}
	}

}

#pragma endregion

#pragma region Remove Empty Folder
void UMyEditorUtilityWidget::RemoveEmptyFolder()
{
	TArray<FString> AssetsInPath = UEditorAssetLibrary::ListAssets(FolderPath, bRecursive, true);

	for (FString Asset : AssetsInPath)
	{
		if (!UEditorAssetLibrary::DoesDirectoryHaveAssets(Asset, bRecursive))
		{
			UEditorAssetLibrary::DeleteDirectory(Asset);
		}
	}
}
#pragma endregion

