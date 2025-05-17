// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisAbilitySystemComponent.h"

#include "Abilities/AnubisGameplayAbility.h"

void UAnubisAbilitySystemComponent::AbilityInputTagTriggered(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
		{
			InputTriggeredSpecHandles.AddUnique(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec.Handle, InputTag));
			InputHeldSpecHandles.AddUnique(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec.Handle, InputTag));
		}
	}
}

void UAnubisAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
		{
			InputReleasedSpecHandles.AddUnique(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec.Handle, InputTag));
			InputHeldSpecHandles.Remove(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec.Handle, InputTag));
		}
	}
}

void UAnubisAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	// if (HasMatchingGameplayTag(InputTags::Input_Blocked))
	// {
	// 	ClearAbilityInput();
	// 	return;
	// }

	static TArray<TTuple<FGameplayAbilitySpecHandle, FGameplayTag>> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (TTuple<FGameplayAbilitySpecHandle, FGameplayTag> SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle.Key))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				AbilitiesToActivate.AddUnique(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec->Handle, SpecHandle.Value));
			}
		}
	}

	for (TTuple<FGameplayAbilitySpecHandle, FGameplayTag> SpecHandle : InputTriggeredSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle.Key))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec,SpecHandle.Value);
				}
				else
				{
					AbilitiesToActivate.AddUnique(TTuple<FGameplayAbilitySpecHandle, FGameplayTag>(AbilitySpec->Handle, SpecHandle.Value));
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (TTuple<FGameplayAbilitySpecHandle, FGameplayTag> AbilitySpecHandle : AbilitiesToActivate)
	{
		FGameplayEventData Payload;
		Payload.EventTag = AbilitySpecHandle.Value;
		Payload.Instigator = AbilityActorInfo->AvatarActor.Get();
		TriggerAbilityFromGameplayEvent(AbilitySpecHandle.Key, AbilityActorInfo.Get(), AbilitySpecHandle.Value, &Payload, *this);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (TTuple<FGameplayAbilitySpecHandle, FGameplayTag> SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle.Key))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec,SpecHandle.Value);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputTriggeredSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UAnubisAbilitySystemComponent::ClearAbilityInput()
{
	InputTriggeredSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UAnubisAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec, const FGameplayTag& GameplayTag)
{
	if (!Spec.IsActive())
	{
		return;
	}
	for (UGameplayAbility* Instance : Spec.GetAbilityInstances())
	{
		if (UAnubisGameplayAbility* Ability = Cast<UAnubisGameplayAbility>(Instance))
		{
			Ability->InputTriggered(Spec.Handle, AbilityActorInfo.Get(), GameplayTag);
		}
	}
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
	const FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);

}

void UAnubisAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec, const FGameplayTag& GameplayTag)
{
	if (!Spec.IsActive())
	{
		return;
	}
	for (UGameplayAbility* Instance : Spec.GetAbilityInstances())
	{
		if (UAnubisGameplayAbility* Ability = Cast<UAnubisGameplayAbility>(Instance))
		{
			Ability->InputReleased(Spec.Handle, AbilityActorInfo.Get(), GameplayTag);
		}
	}
}