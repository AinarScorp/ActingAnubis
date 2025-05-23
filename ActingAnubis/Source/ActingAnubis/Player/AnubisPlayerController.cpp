// Fill out your copyright notice in the Description page of Project Settings.

#include "AnubisPlayerController.h"

#include "ActingAnubis/AbilitySystem/AnubisAbilitySystemComponent.h"
#include "ActingAnubis/AbilitySystem/Interfaces/IListenToInputTag.h"
#include "ActingAnubis/AbilitySystem/Library/AnubisAbilitySystemLibrary.h"
#include "ActingAnubis/Data/PawnData.h"
#include "ActingAnubis/Input/AnubisInputComponent.h"
#include "EnhancedInputSubsystems.h"

namespace
{
	UAnubisAbilitySystemComponent* FindAnubisAbilitySystemComponent(APawn* InPawn)
	{
		const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(InPawn);
		if (AbilitySystemInterface)
		{
			return Cast<UAnubisAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		}
		return InPawn->FindComponentByClass<UAnubisAbilitySystemComponent>();
	}
}

void AAnubisPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!PawnData)
	{
		return;
	}
	CacheAbilitySystemComponent(InPawn);
	UAnubisAbilitySystemLibrary::InitializeAbilitySystem(AnubisAbilitySystemComponent, PawnData, this, InPawn);
	PawnData->InitializeListenToInputTagSets(InPawn);
}

void AAnubisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!PawnData)
	{
		return;
	}
	SetupMappingContext();
	UAnubisInputComponent* AnubisInputComponent = CastChecked<UAnubisInputComponent>(InputComponent);
	if (AnubisInputComponent)
	{
		for (TObjectPtr<UInputConfig> InputConfig : PawnData->InputConfigs)
		{
			if (!InputConfig)
			{
				continue;
			}
			AnubisInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagTriggered, &ThisClass::AbilityInputTagReleased);
		}
	}
}

void AAnubisPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	GetAnubisAbilityComponent()->ProcessAbilityInput(DeltaTime, bGamePaused);

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UAbilitySystemComponent* AAnubisPlayerController::GetAbilitySystemComponent() const
{
	return GetAnubisAbilityComponent();
}

UAnubisAbilitySystemComponent* AAnubisPlayerController::BP_GetAbilityComponent() const
{
	return GetAnubisAbilityComponent();
}

void AAnubisPlayerController::AbilityInputTagTriggered(const FGameplayTag InputTag)
{
	GetAnubisAbilityComponent()->AbilityInputTagTriggered(InputTag);
	if (PawnData)
	{
		PawnData->ProcessComponentInput(InputTag,true);
	}
}

void AAnubisPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	GetAnubisAbilityComponent()->AbilityInputTagReleased(InputTag);
	if (PawnData)
	{
		PawnData->ProcessComponentInput(InputTag,false);
	}
}

UAnubisAbilitySystemComponent* AAnubisPlayerController::GetAnubisAbilityComponent() const
{
	return AnubisAbilitySystemComponent? AnubisAbilitySystemComponent.Get() : FindAnubisAbilitySystemComponent(GetPawn());
}

void AAnubisPlayerController::CacheAbilitySystemComponent(APawn* InPawn)
{
	if (AnubisAbilitySystemComponent)
	{
		return;
	}
	AnubisAbilitySystemComponent = FindAnubisAbilitySystemComponent(InPawn);
	check(AnubisAbilitySystemComponent);
}

void AAnubisPlayerController::SetupMappingContext() const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		for (TObjectPtr<UInputConfig> InputConfig : PawnData->InputConfigs)
		{
			if (!InputConfig)
			{
				continue;
			}
			for (const FMappingContextWithPriority& MappingContextWithPriority : InputConfig->MappingContexts)
			{
				Subsystem->AddMappingContext(MappingContextWithPriority.MappingContext, MappingContextWithPriority.Priority);
			}
		}
	}
}