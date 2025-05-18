// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PawnData.generated.h"

class UPawnData;
class IListenToInputTagInterface;
class UAbilitySet;
class UInputConfig;

USTRUCT(BlueprintType)
struct FListenToInputComponentsInfo
{
	GENERATED_BODY()
	//MUST BE inheriting from ListenToInputTagInterface
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|Pawn Data")
	TSubclassOf<UActorComponent> ListenToTagComponent;
	//DEPRECATED: Use InputTags Instead
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|PawnData", meta = (Categories = "Input", DeprecatedProperty, DeprecatedMessage = "Use Input Tags Instead"))
	FGameplayTag InputTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|PawnData", meta = (Categories = "Input"))
	FGameplayTagContainer InputTags;
};

UCLASS()
class ACTINGANUBIS_API UListenToTagInputSet : public UDataAsset
{
	GENERATED_BODY()
	friend UPawnData;
public:
	//TodoEinar Remove Tag use only Tags
	void CreatedComponent(const FGameplayTag& Tag, const FGameplayTagContainer& Tags, UActorComponent* ActorComponent);
private:
	void InputProcess(const FGameplayTag& Tag, bool InputTriggered);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|PawnData")
	TArray<FListenToInputComponentsInfo> ListenToInputComponentsInfos;
private:
	TMap<FGameplayTag, TArray<TWeakObjectPtr<UActorComponent>>> ComponentsWaitingForInput;
};
/**
 *
 */
UCLASS()
class ACTINGANUBIS_API UPawnData : public UDataAsset
{
	GENERATED_BODY()
public:
	void ProcessComponentInput(const FGameplayTag& Tag, bool InputTriggered) const;
	void InitializeListenToInputTagSets(AActor* OwnerActor) const;
public:
	// Input Config that will be used to bind inputs to this pawn
	UPROPERTY(EditDefaultsOnly, Category="Acting Anubis|PawnData")
	TArray<TObjectPtr<UInputConfig>> InputConfigs;
	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|PawnData")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acting Anubis|PawnData")
	TArray<TObjectPtr<UListenToTagInputSet>> ListenToTagInputSets;

};