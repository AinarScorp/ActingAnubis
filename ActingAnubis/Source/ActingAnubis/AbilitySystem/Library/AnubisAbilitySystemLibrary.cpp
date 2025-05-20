// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ActingAnubis/AbilitySystem/AnubisAbilitySystemComponent.h"
#include "ActingAnubis/AbilitySystem/Data/AbilitySet.h"
#include "ActingAnubis/Data/PawnData.h"

void UAnubisAbilitySystemLibrary::InitializeAbilitySystem(UAbilitySystemComponent* AbilitySystemComp, const UPawnData* PawnData, AActor* OwnerActor, AActor* AvatarActor)
{
	if (PawnData)
	{
		InitializeAbilitySystemWithAbilitySets(AbilitySystemComp, PawnData->AbilitySets, OwnerActor, AvatarActor);
	}
}

void UAnubisAbilitySystemLibrary::InitializeAbilitySystemWithAbilitySets(UAbilitySystemComponent* AbilitySystemComp, const TArray<UAbilitySet*> AbilitySets, AActor* OwnerActor, AActor* AvatarActor)
{
	if (!AbilitySystemComp)
	{
		return;
	}
	for (const auto& AbilitySet : AbilitySets)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComp, nullptr, OwnerActor);
	}
	AbilitySystemComp->InitAbilityActorInfo(OwnerActor, AvatarActor);
}

bool UAnubisAbilitySystemLibrary::AddStatusTags(AActor* Actor, FGameplayTagContainer GameplayTags)
{
	if (UAbilitySystemComponent* AbilitySysComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		AbilitySysComp->AddLooseGameplayTags(GameplayTags);
		return true;
	}
	return false;
}

bool UAnubisAbilitySystemLibrary::RemoveStatusTags(AActor* Actor, FGameplayTagContainer GameplayTags)
{
	if (UAbilitySystemComponent* AbilitySysComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		AbilitySysComp->RemoveLooseGameplayTags(GameplayTags);
		return true;
	}
	return false;
}

bool UAnubisAbilitySystemLibrary::GetOwnedGameplayTags(const AActor* Actor, FGameplayTagContainer& TagContainer)
{
	if (Actor == nullptr)
	{
		return false;
	}

	const IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor->FindComponentByInterface(UGameplayTagAssetInterface::StaticClass()));
	if (TagAssetInterface)
	{
		TagAssetInterface->GetOwnedGameplayTags(TagContainer);
		return true;
	}
	return false;
}

bool UAnubisAbilitySystemLibrary::HasMatchingGameplayTag(const AActor* Actor, FGameplayTag TagToCheck)
{
	if (Actor == nullptr)
	{
		return false;
	}
	const IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor->FindComponentByInterface(UGameplayTagAssetInterface::StaticClass()));
	if (TagAssetInterface)
	{
		return TagAssetInterface->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}

bool UAnubisAbilitySystemLibrary::HasAllMatchingGameplayTags(const AActor* Actor, FGameplayTagContainer TagContainer)
{
	if (Actor == nullptr)
	{
		return false;
	}

	const IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor->FindComponentByInterface(UGameplayTagAssetInterface::StaticClass()));
	if (TagAssetInterface)
	{
		return TagAssetInterface->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool UAnubisAbilitySystemLibrary::HasAnyMatchingGameplayTags(const AActor* Actor, FGameplayTagContainer TagContainer)
{
	if (Actor == nullptr)
	{
		return false;
	}
	const IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor->FindComponentByInterface(UGameplayTagAssetInterface::StaticClass()));
	if (TagAssetInterface)
	{
		return TagAssetInterface->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}

FGameplayTagContainer UAnubisAbilitySystemLibrary::FilterTagContainer(const FGameplayTagContainer& TagContainer, const FGameplayTagContainer& FilterTag, bool FilterExact)
{
	return FilterExact? TagContainer.FilterExact(FilterTag) : TagContainer.Filter(FilterTag);
}