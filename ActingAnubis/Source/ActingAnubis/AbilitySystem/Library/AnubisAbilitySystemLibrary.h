// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnubisAbilitySystemLibrary.generated.h"

class UPawnData;
class UAbilitySet;
class UAnubisAbilitySystemComponent;
/**
 *
 */
UCLASS()
class ACTINGANUBIS_API UAnubisAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem")
	static void InitializeAbilitySystem(UAnubisAbilitySystemComponent* AbilitySystemComp, const UPawnData* PawnData, AActor* OwnerActor, AActor* AvatarActor);
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem", DisplayName = "Initialize Ability System With Ability Sets", meta = (Keywords = "Init"))
	static void InitializeAbilitySystemWithAbilitySets(UAnubisAbilitySystemComponent* AbilitySystemComp, const TArray<UAbilitySet*> AbilitySets, AActor* OwnerActor, AActor* AvatarActor);

	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem|Tags")
	static bool GetOwnedGameplayTags(const AActor* Actor, FGameplayTagContainer& TagContainer);
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem|Tags")
	static bool HasMatchingGameplayTag(const AActor* Actor, FGameplayTag TagToCheck);
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem|Tags")
	static bool HasAllMatchingGameplayTags(const AActor* Actor, const FGameplayTagContainer& TagContainer);
	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem|Tags")
	static bool HasAnyMatchingGameplayTags(const AActor* Actor, const FGameplayTagContainer& TagContainer);

	UFUNCTION(BlueprintCallable, Category = "Acting Anubis|Library|AbilitySystem|Tags")
	static FGameplayTagContainer FilterTagContainer(const FGameplayTagContainer& TagContainer, const FGameplayTagContainer& FilterTag, bool FilterExact);
};