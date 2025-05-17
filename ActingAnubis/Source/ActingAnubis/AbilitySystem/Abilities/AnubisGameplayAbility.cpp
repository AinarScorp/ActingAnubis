// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisGameplayAbility.h"
#include "AbilitySystemComponent.h"

UAnubisGameplayAbility::UAnubisGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAnubisGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	UAbilitySystemComponent* Comp = ActorInfo->AbilitySystemComponent.Get();
	Comp->AddLooseGameplayTags(OwnedTagsUponGrantingAbility);
	BP_OnGiveAbility(*ActorInfo);
}

void UAnubisGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	UAbilitySystemComponent* Comp = ActorInfo->AbilitySystemComponent.Get();
	Comp->RemoveLooseGameplayTags(OwnedTagsUponGrantingAbility);
	BP_OnRemoveAbility(*ActorInfo);
}

void UAnubisGameplayAbility::InputTriggered(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag)
{
	BP_InputTriggered(*ActorInfo, InputTag);
}

void UAnubisGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag)
{
	BP_InputReleased(*ActorInfo, InputTag);
}

void UAnubisGameplayAbility::BP_CancelAllAbilities(FGameplayTagContainer AbilitiesToIgnore)
{
	if (const TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = GetActorInfo().AbilitySystemComponent; AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities(nullptr,&AbilitiesToIgnore, this);
	}
}