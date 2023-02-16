// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyboardActor.generated.h"

UCLASS()
class VR_51_API AKeyboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AKeyboardActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=KeySound)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = KeySound)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = KeySound)
	class USoundBase* keySound;

	UPROPERTY(EditAnywhere, Category = KeySound)
	FLinearColor keyColor;
	
	UPROPERTY(EditAnywhere, Category = KeySound)
	float glowPower;

private:
	class UMaterialInstanceDynamic* keyMat;
	FTimerHandle colorHandle;

	UFUNCTION()
	void OnPlayKey(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ColorOff();
	void ChangeKeyColor(FVector4 color, float glow);
};
