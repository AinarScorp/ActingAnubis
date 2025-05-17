// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "AnubisInputComponent.generated.h"

class UInputAction;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "Anubis Input Component")
class ACTINGANUBIS_API UAnubisInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UAnubisInputComponent(const FObjectInitializer& ObjectInitializer){};

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound = false);

	template <class UserClass, typename TriggeredFuncType, typename StartedFuncType, typename OngoingFuncType, typename CancelledFuncType, typename CompletedFuncType>
	void BindNativeAction(const UInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, TriggeredFuncType TriggeredFunc, StartedFuncType StartedFunc, OngoingFuncType OngoingFunc, CancelledFuncType CancelledFunc, CompletedFuncType CompletedFunc, bool bLogIfNotFound);

	template <class UserClass, typename TriggeredFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc, ReleasedFuncType ReleasedFunc);
	template <class UserClass, typename TriggeredFuncType, typename StartedFuncType, typename OngoingFuncType, typename CancelledFuncType, typename CompletedFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc, StartedFuncType StartedFunc = nullptr, OngoingFuncType OngoingFunc = nullptr,
							CancelledFuncType CancelledFunc = nullptr, CompletedFuncType CompletedFunc = nullptr);
};

template <class UserClass, typename FuncType>
void UAnubisInputComponent::BindNativeAction(const UInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func, InputTag);
	}
}

template <class UserClass, typename TriggeredFuncType, typename StartedFuncType, typename OngoingFuncType, typename CancelledFuncType, typename CompletedFuncType>
void UAnubisInputComponent::BindNativeAction(const UInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, TriggeredFuncType TriggeredFunc, StartedFuncType StartedFunc, OngoingFuncType OngoingFunc, CancelledFuncType CancelledFunc, CompletedFuncType CompletedFunc, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		if (TriggeredFunc)
		{
			BindAction(IA, ETriggerEvent::Triggered, Object, TriggeredFunc, InputTag);
		}
		if (StartedFunc)
		{
			BindAction(IA, ETriggerEvent::Started, Object, StartedFunc, InputTag);
		}
		if (OngoingFunc)
		{
			BindAction(IA, ETriggerEvent::Ongoing, Object, OngoingFunc, InputTag);
		}
		if (CancelledFunc)
		{
			BindAction(IA, ETriggerEvent::Canceled, Object, CancelledFunc, InputTag);
		}
		if (CompletedFunc)
		{
			BindAction(IA, ETriggerEvent::Completed, Object, CompletedFunc, InputTag);
		}
	}
}

template <class UserClass, typename TriggeredFuncType, typename ReleasedFuncType>
void UAnubisInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FAnubisInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (TriggeredFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, TriggeredFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}

template <class UserClass, typename TriggeredFuncType, typename StartedFuncType, typename OngoingFuncType, typename CancelledFuncType, typename CompletedFuncType>
void UAnubisInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc, StartedFuncType StartedFunc, OngoingFuncType OngoingFunc, CancelledFuncType CancelledFunc, CompletedFuncType CompletedFunc)
{
	check(InputConfig);
	for (const FAnubisInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (TriggeredFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, TriggeredFunc, Action.InputTag);
			}
			if (StartedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, StartedFunc, Action.InputTag);
			}
			if (OngoingFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Ongoing, Object, OngoingFunc, Action.InputTag);
			}
			if (CancelledFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Canceled, Object, CancelledFunc, Action.InputTag);
			}
			if (CompletedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, CompletedFunc, Action.InputTag);
			}
		}
	}
}