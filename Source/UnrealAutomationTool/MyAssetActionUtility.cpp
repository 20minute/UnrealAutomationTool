// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetActionUtility.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"

#pragma region Rename Selected Asset
void UMyAssetActionUtility::RenameSelectedAsset(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase)
{
	// Empty check
	if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern))
	{
		return;
	}

	// Get the selected assets
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			FString AssetName = SelectedObject->GetName();
			if (AssetName.Contains(SearchPattern, SearchCase))
			{
				FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				++Counter;
			}
		}
	}

	GiveFeedback(TEXT("Renamed"), Counter);
}

#pragma endregion


#pragma  region Check power of two
void UMyAssetActionUtility::CheckPowerOfTwo()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			UTexture* Texture = dynamic_cast<UTexture*>(SelectedObject);
			if (ensure(Texture))
			{
				int32 Height = static_cast<int32>(Texture->GetSurfaceHeight());
				int32 Width = static_cast<int32>(Texture->GetSurfaceWidth());
				if (!IsPowerOfTwo(Height) || !IsPowerOfTwo(Width))
				{
					PrintToScreen(SelectedObject->GetPathName() + " is not a power of 2 texture", FColor::Red);
				}
				else
				{
					++Counter;
				}
			}
			else
			{
				PrintToScreen(SelectedObject->GetPathName() + " is not texture", FColor::Red);
			}
		}
	}

	GiveFeedback("Power of two", Counter);
}
void UMyAssetActionUtility::AddPrefix()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;
	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			const FString* Prefix = PrefixMap.Find(SelectedObject->GetClass());
			if (ensure(Prefix) && !Prefix->IsEmpty())
			{
				FString OldName = SelectedObject->GetName();
				if (OldName.StartsWith(*Prefix))
					continue;

				FString NewName = *Prefix + OldName;
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				++Counter;
			}
			else
			{
				PrintToScreen("Couldn't find prefix for class " + SelectedObject->GetClass()->GetName(), FColor::Red);
			}
		}
	}

	GiveFeedback("Added Prefix to", Counter);
}

#pragma  endregion

#pragma region Cleanup Folder
void UMyAssetActionUtility::CleanupFolder(FString ParentFolder)
{
	// Check if parent folder is in the content folder
	if (!ParentFolder.StartsWith(TEXT("/Game")))
	{
		ParentFolder = FPaths::Combine(TEXT("/Game"), ParentFolder);
	}

	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectObject : SelectedObjects)
	{
		if (ensure(SelectObject))
		{
			FString NewPath = FPaths::Combine(ParentFolder, SelectObject->GetClass()->GetName(), SelectObject->GetName());
			if (UEditorAssetLibrary::RenameLoadedAsset(SelectObject, NewPath))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Couldn't move " + SelectObject->GetPathName(), FColor::Red);
			}
		}
	}

	GiveFeedback("Moved", Counter);
}

#pragma endregion

#pragma region Duplicate Asset
void UMyAssetActionUtility::DuplicateAsset(uint32 NumberOfDuplicates, bool bSave)
{
	TArray<FAssetData> AssetDataArray = UEditorUtilityLibrary::GetSelectedAssetData();

	uint32 Counter = 0;

	for (FAssetData AssetData : AssetDataArray)
	{
		for (uint32 i=0; i < NumberOfDuplicates; ++i)
		{
			FString NewFileName = AssetData.AssetName.ToString().AppendChar('_').Append(FString::FromInt(i));
			FString NewPath = FPaths::Combine(AssetData.PackagePath.ToString(), NewFileName);
			if (ensure(UEditorAssetLibrary::DuplicateAsset(AssetData.GetObjectPathString(), NewPath)))
			{
				++Counter;

				if (bSave)
				{
					UEditorAssetLibrary::SaveAsset(NewPath, false);
				}
			}
		}
	}

	GiveFeedback("Duplicated", Counter);
}

#pragma endregion

#pragma region Remove Unused Assets
void UMyAssetActionUtility::RemoveUnusedAssets(bool bDeleteImmediately)
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<UObject*> UnusedObjects = TArray<UObject*>();
	uint32 Counter = 0;

	for (UObject* SelectedObject: SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			// FindPackageReferencersForAsset returns an array -> check size
			if (UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedObject->GetPathName(), true).Num() <= 0)
			{
				UnusedObjects.Add(SelectedObject);
			}
		}
	}

	for (UObject* SelectedObject : UnusedObjects)
	{
		if (bDeleteImmediately)
		{
			if (UEditorAssetLibrary::DeleteLoadedAsset(SelectedObject))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Error deleting " + SelectedObject->GetPathName(), FColor::Red);
			}
		}
		else
		{
			// Rename asset to avoid duplicated name in bin folder
			uint32 assetsAmount = UEditorAssetLibrary::ListAssets(TEXT("/Game/Bin"), false, false).Num();
			FString NewName = SelectedObject->GetName().AppendChar('_').Append(FString::FromInt(assetsAmount));
			FString NewPath = FPaths::Combine(TEXT("/Game/Bin"), NewName);
			if (UEditorAssetLibrary::RenameLoadedAsset(SelectedObject, NewPath))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Error moving " + SelectedObject->GetPathName(), FColor::Red);
			}
		}
	}

	GiveFeedback(bDeleteImmediately ? "Deleted" : "Moved to bin:", Counter);
}
#pragma endregion
#pragma region Helper
void UMyAssetActionUtility::PrintToScreen(FString Message, FColor Color)
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, Message);
	}
}

void UMyAssetActionUtility::GiveFeedback(FString Method, uint32 Counter)
{
	FString Message = FString("No matching files found");
	FColor Color = Counter > 0 ? FColor::Green : FColor::Red;

	if (Counter > 0)
	{
		Message = Method.AppendChar(' ');
		Message.AppendInt(Counter);
		Message.Append(Counter == 1 ? TEXT(" file") : TEXT(" files"));
	}

	PrintToScreen(Message, Color);
}

bool UMyAssetActionUtility::IsPowerOfTwo(int32 NumberToCheck)
{
	if (NumberToCheck <= 0)
		return false;

	return (NumberToCheck & (NumberToCheck - 1)) == 0;
}

#pragma endregion