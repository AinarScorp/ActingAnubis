// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisSimpleAbilitySystemComponent.h"

#include "Abilities/AnubisGameplayAbility.h"
#include "Data/AbilitySet.h"
#include "Library/AnubisAbilitySystemLibrary.h"

UAnubisSimpleAbilitySystemComponent::UAnubisSimpleAbilitySystemComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	ReplicationMode = EGameplayEffectReplicationMode::Minimal;
}

void UAnubisSimpleAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	GiveToAbilitySystem();
	UAnubisAbilitySystemLibrary::InitializeAbilitySystemWithAbilitySets(this,AbilitySets, GetOwner(), GetOwner());
}

void UAnubisSimpleAbilitySystemComponent::GiveToAbilitySystem()
{
	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const TSubclassOf<UAttributeSet>& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(GetOwner(), SetToGrant);
		AddSpawnedAttribute(NewSet);
	}

	// Grant gameplay tags.
	if (GrantedGameplayTags.IsValid())
	{
		AddLooseGameplayTags(GrantedGameplayTags);
	}

	// Grant gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const TSubclassOf<UAnubisGameplayAbility>& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}
		UGameplayAbility* AbilityCDO = AbilityToGrant->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
		GiveAbility(AbilitySpec);
	}

	// Grant gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const TSubclassOf<UGameplayEffect>& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant->GetDefaultObject<UGameplayEffect>();
		ApplyGameplayEffectToSelf(GameplayEffect, 1, MakeEffectContext());
	}
}