// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnData.h"

#include "ActingAnubis/AbilitySystem/Interfaces/IListenToInputTag.h"

void UListenToTagInputSet::CreatedComponent(const FGameplayTag& Tag,const FGameplayTagContainer& Tags, UActorComponent* ActorComponent)
{
	if (!ActorComponent || (!Tag.IsValid() && Tags.IsEmpty()))
	{
		return;
	}
	if (Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DOMINIK, YOU THERE, Don't use Tag anymore inside ListenToTagInputSet, you placed tag with name %s on this component %s"), *Tag.GetTagName().ToString(), *ActorComponent->GetName())
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5000000, FColor::Red, "DOMINIK, YOU THERE, Don't use Tag anymore inside ListenToTagInputSet, Look into Logs");
		}
		TArray<TWeakObjectPtr<UActorComponent>>& FoundArray = ComponentsWaitingForInput.FindOrAdd(Tag);
		FoundArray.Add(ActorComponent);
	}
	for (auto TagIt = Tags.CreateConstIterator(); TagIt; ++TagIt)
	{
		const FGameplayTag& InputTagFromTags = *TagIt;
		if (!InputTagFromTags.IsValid() || Tag.MatchesTagExact(InputTagFromTags))
		{
			continue;
		}
		TArray<TWeakObjectPtr<UActorComponent>>& FoundArray = ComponentsWaitingForInput.FindOrAdd(InputTagFromTags);
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
			ListenToInputComponentInfo->CreatedComponent(Info.InputTag, Info.InputTags, CreatedActorComponent);
		}
	}
}