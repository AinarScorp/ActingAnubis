// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AnubisSimpleAbilitySystemComponent.generated.h"

class UAnubisGameplayAbility;
class UAbilitySet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTINGANUBIS_API UAnubisSimpleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAnubisSimpleAbilitySystemComponent();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Acting Anubis")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Acting Anubis|No Ability Set Setup")
	TArray<TSubclassOf<UAttributeSet>> GrantedAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Acting Anubis|No Ability Set Setup")
	FGameplayTagContainer GrantedGameplayTags;

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Acting Anubis|No Ability Set Setup")
	TArray<TSubclassOf<UAnubisGameplayAbility>> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Acting Anubis|No Ability Set Setup")
	TArray<TSubclassOf<UGameplayEffect>> GrantedGameplayEffects;
private:
	void GiveToAbilitySystem();

};