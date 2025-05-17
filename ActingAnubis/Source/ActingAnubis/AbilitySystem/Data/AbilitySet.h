// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySet.generated.h"

class UAnubisAbilitySystemComponent;
class UAnubisGameplayAbility;
class UAttributeSet;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;

USTRUCT(BlueprintType, Category = "Acting Anubis|AbilitySet")
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	//This is used only for setting the name of the property
	UPROPERTY(EditDefaultsOnly)
	FString TitleProperty = "Not Set";
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnubisGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input"))
	FGameplayTagContainer InputTags;
};

USTRUCT(BlueprintType, Category = "Acting Anubis|AbilitySet")
struct FAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	//This is used only for setting the name of the property
	UPROPERTY(EditDefaultsOnly)
	FString TitleProperty = "Not Set";
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType, Category = "Acting Anubis|AbilitySet")
struct FAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

USTRUCT(BlueprintType, Category = "Acting Anubis|AbilitySet")
struct FAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);
	void AddGameplayTags(const FGameplayTagContainer& TagContainer);
	void TakeFromAbilitySystem(UAnubisAbilitySystemComponent* AbilitySystemComponent);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;

	// Granted GameplayTags
	UPROPERTY()
	FGameplayTagContainer GrantedGameplayTags;
};
/**
 *
 */
UCLASS()
class ACTINGANUBIS_API UAbilitySet final : public UDataAsset
{
	GENERATED_BODY()
public:
	void GiveToAbilitySystem(UAnubisAbilitySystemComponent* AbilitySystemComponent, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr, AActor* NewOwner = nullptr);
	void RemoveFromAbilitySystem(UAnubisAbilitySystemComponent* AbilitySystemComponent);

private:
	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FAbilitySet_AttributeSet> GrantedAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagContainer GrantedGameplayTags;

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty="TitleProperty"))
	TArray<FAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=TitleProperty))
	TArray<FAbilitySet_GameplayEffect> GrantedGameplayEffects;

private:
	FAbilitySet_GrantedHandles GrantedHandles;
};