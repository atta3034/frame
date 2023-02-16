// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Player.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MoveComponent.h"
#include "GraspComponent.h"


// Sets default values
AVR_Player::AVR_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->SetupAttachment(RootComponent);

	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	headMesh->SetupAttachment(cam);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	leftController->SetupAttachment(RootComponent);
	// 모션 소스 선택
	leftController->MotionSource = "Left";

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	bUseControllerRotationPitch = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 액터 컴포넌트들 추가
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	graspComp = CreateDefaultSubobject<UGraspComponent>(TEXT("Grasp Component"));

}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();
	
	// 헤드 장비의 기준 위치를 설정한다.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	// 1. 플레이어 콘트롤러를 가져온다.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	// 2. 플레이어 콘트롤러에서 EnhancedInputLocalPlayerSubsystem를 가져온다.
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	// 3. 가져온 Subsystem에 IMC를 등록한다.(우선순위 0번)
	subsys->AddMappingContext(myMapping, 0);
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);

		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void AVR_Player::OnTriggerLeft(const FInputActionValue& value)
{
	float val = value.Get<float>();

	// 왼손 로그에 값을 출력한다.
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}



