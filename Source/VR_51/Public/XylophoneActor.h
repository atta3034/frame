// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XylophoneActor.generated.h"

UCLASS()
class VR_51_API AXylophoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AXylophoneActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = BaseSettings)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, Category=BaseSettings)
	TArray<class UChildActorComponent*> childComps;

	UPROPERTY(EditDefaultsOnly, Category = BaseSettings)
	TSubclassOf<class AKeyboardActor> keyboard;

private:
	

};
