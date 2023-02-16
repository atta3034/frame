// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "VR_Player.generated.h"

UCLASS()
class VR_51_API AVR_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVR_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UCameraComponent* cam;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UTextRenderComponent* rightLog;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
	class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
	TArray<class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
	class UGraspComponent* graspComp;
	

private:
	void OnTriggerLeft(const struct FInputActionValue& value);
	

};
