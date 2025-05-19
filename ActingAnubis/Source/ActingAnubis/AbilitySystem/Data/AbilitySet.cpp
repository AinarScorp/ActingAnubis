// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySet.h"

#include "ActingAnubis/AbilitySystem/Abilities/AnubisGameplayAbility.h"
#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpecHandle.h"

void FAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);

}

void FAbilitySet_GrantedHandles::AddGameplayTags(const FGameplayTagContainer& TagContainer)
{
	GrantedGameplayTags.AppendTags(TagContainer);
}

void FAbilitySet_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		AbilitySystemComponent->ClearAbility(Handle);
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		AbilitySystemComponent->RemoveSpawnedAttribute(Set);
	}

	AbilitySystemComponent->RemoveLooseGameplayTags(GrantedGameplayTags);

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
	GrantedGameplayTags.Reset();
}

void UAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject, AActor* NewOwner)
{
	check(AbilitySystemComponent);

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(NewOwner ? NewOwner : AbilitySystemComponent->GetOwner(), SetToGrant.AttributeSet);
		AbilitySystemComponent->AddSpawnedAttribute(NewSet);
		GrantedHandles.AddAttributeSet(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}

	// Grant gameplay tags.
	if (GrantedGameplayTags.IsValid())
	{
		AbilitySystemComponent->AddLooseGameplayTags(GrantedGameplayTags);
		GrantedHandles.AddGameplayTags(GrantedGameplayTags);
	}

	// Grant gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AppendTags(AbilityToGrant.InputTags);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
		GrantedHandles.AddAbilitySpecHandle(AbilitySpecHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, AbilitySystemComponent->MakeEffectContext());
		GrantedHandles.AddGameplayEffectHandle(GameplayEffectHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

}

void UAbilitySet::RemoveFromAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
}