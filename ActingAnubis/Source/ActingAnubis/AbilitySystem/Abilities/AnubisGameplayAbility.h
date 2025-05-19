// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AnubisGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class ACTINGANUBIS_API UAnubisGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UAnubisGameplayAbility();
	UFUNCTION(BlueprintImplementableEvent, Category = "Acting Anubis|Ability", DisplayName = "InputTriggered")
	void BP_InputTriggered(UPARAM(ref) const FGameplayAbilityActorInfo& ActorInfo, const FGameplayTag& InputTag);
	UFUNCTION(BlueprintImplementableEvent, Category = "Acting Anubis|Ability", DisplayName = "InputReleased")
	void BP_InputReleased(UPARAM(ref) const FGameplayAbilityActorInfo& ActorInfo, const FGameplayTag& InputTag);
	UFUNCTION(BlueprintImplementableEvent, Category = "Acting Anubis|Ability", DisplayName = "OnGiveAbility")
	void BP_OnGiveAbility(UPARAM(ref) const FGameplayAbilityActorInfo& ActorInfo);
	UFUNCTION(BlueprintImplementableEvent, Category = "Acting Anubis|Ability", DisplayName = "OnRemoveAbility")
	void BP_OnRemoveAbility(UPARAM(ref) const FGameplayAbilityActorInfo& ActorInfo);


	UFUNCTION(BlueprintCallable, Category = Ability)
	APlayerController* GetPlayerControllerFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = Ability)
	UMovementComponent* GetMovementComponentFromActorInfo() const;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override{}
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override{}
	void InputTriggered(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag);
	void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTag& InputTag);

protected:
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Ability", DisplayName =  "Cancel All Abilties")
	void BP_CancelAllAbilities(FGameplayTagContainer AbilitiesToIgnore);
protected:
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="OwnedTagsCategory"))
	FGameplayTagContainer OwnedTagsUponGrantingAbility;
};