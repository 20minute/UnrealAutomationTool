// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorActionUtility.h"
#include "Engine/Selection.h"
#include "Engine/StaticMeshActor.h"
#include "UnrealEd.h"

void UMyActorActionUtility::ChangeMaterial(UMaterialInterface* NewMaterial)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	if (SelectedActors == nullptr)
		return;

	TArray<UObject*> SelectedObjects;
	SelectedActors->GetSelectedObjects(AStaticMeshActor::StaticClass(), SelectedObjects);

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (SelectedObject == nullptr)
			continue;

		AStaticMeshActor* StaticMeshActor = dynamic_cast<AStaticMeshActor*>(SelectedObject);

		if (StaticMeshActor != nullptr)
		{
			UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();
			if (StaticMeshComponent != nullptr)
			{
				int32 NumMaterials = StaticMeshComponent->GetNumMaterials();
				for (int i =0; i< NumMaterials; ++i)
				{
					StaticMeshComponent->SetMaterial(i, NewMaterial);
				}
			}
		}
	}
}
