// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "IListenToInputTag.generated.h"

class UAbilityAsync_WaitGameplayEvent;
// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UListenToInputTagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ACTINGANUBIS_API IListenToInputTagInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Acting Anubis")
	bool StoreWaitAsyncTask(UAbilityAsync_WaitGameplayEvent* WaitGameplayEvent);
	UFUNCTION(BlueprintNativeEvent, Category = "Acting Anubis")
	void ReceiveTriggeredInputFromTag(FGameplayTag EventData);
	UFUNCTION(BlueprintNativeEvent, Category = "Acting Anubis")
	void ReceiveReleasedInputFromTag(FGameplayTag EventData);
	UFUNCTION(BlueprintNativeEvent, Category = "Acting Anubis")
	FTransform RelativeTransform();
};