// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AnubisAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName = "Anubis Ability Component", Category = "Acting Anubis"))
class ACTINGANUBIS_API UAnubisAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAnubisAbilitySystemComponent();
	//virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	void AbilityInputTagTriggered(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override{};
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override{};
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec, const FGameplayTag& GameplayTag);
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec, const FGameplayTag& GameplayTag);
public:
	// DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitySystemComponentEvent, UAnubisAbilitySystemComponent* /* Ability system Component*/)
	// FAbilitySystemComponentEvent OnPreAbilitySystemComponentInit;
	// FAbilitySystemComponentEvent OnAbilitySystemComponentInitialized;
protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<TTuple<FGameplayAbilitySpecHandle, FGameplayTag>> InputTriggeredSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<TTuple<FGameplayAbilitySpecHandle, FGameplayTag>> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<TTuple<FGameplayAbilitySpecHandle, FGameplayTag>> InputHeldSpecHandles;
};