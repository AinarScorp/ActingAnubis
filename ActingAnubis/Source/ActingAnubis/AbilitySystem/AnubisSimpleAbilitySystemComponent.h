// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AnubisSimpleAbilitySystemComponent.generated.h"

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
};