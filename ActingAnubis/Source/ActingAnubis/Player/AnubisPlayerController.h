// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "AnubisPlayerController.generated.h"

class UPawnData;
class UAnubisAbilitySystemComponent;
class UInputMappingContext;
struct FGameplayTag;
/**
 *
 */
UCLASS()
class ACTINGANUBIS_API AAnubisPlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = " Get Anubis Ability Component", Category = "Acting Anubis|Controller")
	UAnubisAbilitySystemComponent* BP_GetAbilityComponent() const;
protected:
	void AbilityInputTagTriggered(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
private:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;


	UAnubisAbilitySystemComponent* GetAnubisAbilityComponent() const;
	void CacheAbilitySystemComponent(APawn* InPawn);

	void SetupMappingContext() const;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Acting Anubis|Controller", meta = (DisplayPriority = "1"))
	TObjectPtr<const UPawnData> PawnData;
	UPROPERTY()
	TObjectPtr<UAnubisAbilitySystemComponent> AnubisAbilitySystemComponent;

};