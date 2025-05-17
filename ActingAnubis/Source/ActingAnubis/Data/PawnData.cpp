// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnData.h"

#include "ActingAnubis/AbilitySystem/Interfaces/IListenToInputTag.h"

void UListenToTagInputSet::CreatedComponent(const FGameplayTag& Tag, UActorComponent* ActorComponent)
{
	if (ActorComponent && Tag.IsValid())
	{
		TArray<TWeakObjectPtr<UActorComponent>>& FoundArray = ComponentsWaitingForInput.FindOrAdd(Tag);
		FoundArray.Add(ActorComponent);
	}
}

void UListenToTagInputSet::InputProcess(const FGameplayTag& Tag, bool InputTriggered)
{
	const TArray<TWeakObjectPtr<UActorComponent>>& FoundArray = ComponentsWaitingForInput.FindRef(Tag);
	for (const auto& ActorComponent : FoundArray)
	{
		if (!ActorComponent.IsValid())
		{
			continue;
		}
		if (InputTriggered)
		{
			IListenToInputTagInterface::Execute_ReceiveTriggeredInputFromTag(ActorComponent.Get(), Tag);
		}
		else
		{
			IListenToInputTagInterface::Execute_ReceiveReleasedInputFromTag(ActorComponent.Get(), Tag);
		}
	}
}

void UPawnData::ProcessComponentInput(const FGameplayTag& Tag, bool InputTriggered) const
{
	for (const TObjectPtr<UListenToTagInputSet>& ListenToInputComponentInfo : ListenToTagInputSets)
	{
		ListenToInputComponentInfo->InputProcess(Tag, InputTriggered);
	}
}

void UPawnData::InitializeListenToInputTagSets(AActor* OwnerActor) const
{
	for (const TObjectPtr<UListenToTagInputSet>& ListenToInputComponentInfo : ListenToTagInputSets)
	{
		for (const FListenToInputComponentsInfo& Info : ListenToInputComponentInfo->ListenToInputComponentsInfos)
		{
			UActorComponent* ActorComponentDefault = Info.ListenToTagComponent.GetDefaultObject();
			if (!ActorComponentDefault || !ActorComponentDefault->Implements<UListenToInputTagInterface>())
			{
				continue;
			}
			UActorComponent* CreatedActorComponent = OwnerActor->AddComponentByClass(Info.ListenToTagComponent, false, IListenToInputTagInterface::Execute_RelativeTransform(ActorComponentDefault), false);
			ListenToInputComponentInfo->CreatedComponent(Info.InputTag, CreatedActorComponent);
		}
	}
}