// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"
#include "Components/CapsuleComponent.h"
#include "TeleportRingActor.h"
#include "NiagaraComponent.h"


// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// 주인인 플레이어 클래스를 캐싱한다.
	player = Cast<AVR_Player>(GetOwner());
	
	// 현재 월드 클래스를 캐싱한다.
	currentWorld = GetWorld();
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsShowLine)
	{
		DrawMoveLine();
	}
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UMoveComponent::ShowLine);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UMoveComponent::HideLine);
}

void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D val = value.Get<FVector2D>();
	FVector direction = FVector(val.Y, val.X, 0);
	
	player->AddMovementInput(direction.GetSafeNormal(), 1, false);
}

void UMoveComponent::RotateAxis(const FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();

	// axis 값을 이용해서 캐릭터(콘트롤러)를 회전한다.
	player->AddControllerPitchInput(axis.Y * -1.0f);
	player->AddControllerYawInput(axis.X);
}

void UMoveComponent::DrawMoveLine()
{
	// 이동할 곡선을 화면에 그린다.
	// 1. 이동할 위치 구간을 구한다.(TArray<FVector>)
	// 1-1. p = p0 + vt - 0.5 * g * t^2

	float timeInterval = 0.02f;
	int32 timeSegment = 50;

	//FVector dir = (FVector::ForwardVector + FVector::UpVector);
	//dir = player->leftHand->GetComponentTransform().TransformVector(dir);

	// 왼손 모델링의 로컬 좌표에 맞춰서 forward 방향과 up 방향을 다시 계산한다.
	FVector handForward = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;
	
	// 다시 계산한 손 방향으로 dir을 구한다.
	FVector dir = handForward + handUp;

	lineLoc.Empty();

	for (int32 i = 0; i < timeSegment; i++)
	{
		// 소요시간
		float timeTaken = timeInterval * i;
		// p = p0 + vt
		FVector prediction = player->leftHand->GetComponentLocation() + dir.GetSafeNormal() * power * timeTaken;

		// p.Z -= 0.5 * g * t^2
		prediction.Z += 0.5f * currentWorld->GetDefaultGravityZ() * timeTaken * timeTaken;

		// 1-2. 0.1초씩 0~50번째 구간을 계산해서 배열에 넣는다.
		lineLoc.Add(prediction);

		// 라인트레이스를 구간마다 체크한다.
		FHitResult hitInfo;

		if (i > 0)
		{
			if (currentWorld->LineTraceSingleByChannel(hitInfo, lineLoc[i - 1], lineLoc[i], ECC_Visibility))
			{
				// 부딪힌 지점을 마지막 좌표로 넣고 반복문을 강제 종료한다. 
				lineLoc.Add(hitInfo.ImpactPoint);
				break;
			}
		}
	}
	

	// 2. 앞에서 구한 위치 마다 선으로 연결한 그림을 그린다(DrawLine).
	for (int32 i = 0; i < lineLoc.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(), lineLoc[i], lineLoc[i + 1], FColor::Green, false, -1, 0, 2);
	}

	// 텔레포트 링 이펙트를 마지막 라인 위치에 배치한다.
	if (spawned_fx == nullptr)
	{
		// 이펙트를 생성한다.
		spawned_fx = currentWorld->SpawnActor<ATeleportRingActor>(teleport_fx, lineLoc[lineLoc.Num() - 1], FRotator::ZeroRotator);
	}
	else
	{
		// 안보이게 처리된 이펙트를 다시 보이게 한다.
		spawned_fx->niagara_fx->SetVisibility(true);
		spawned_fx->SetActorLocation(lineLoc[lineLoc.Num() - 1]);
	}
}

void UMoveComponent::Teleport()
{
	// 가리킨 지점으로 순간 이동(즉시 좌표 변경)한다.
	FVector targetLoc = lineLoc[lineLoc.Num() - 1];
	targetLoc.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	player->SetActorLocation(targetLoc, false, nullptr, ETeleportType::None);

	if (spawned_fx != nullptr)
	{
		spawned_fx->niagara_fx->SetVisibility(false);
	}
}

void UMoveComponent::ShowLine()
{
	bIsShowLine = true;
}

void UMoveComponent::HideLine()
{
	bIsShowLine = false;
	//Teleport();
	TeleportFade();
	
}

void UMoveComponent::TeleportFade()
{
	// 카메라를 Fade in 한다.
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 1.0f, 1.0f, FLinearColor::Black);

	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this, &UMoveComponent::Teleport, 1.0f, false);
	//GetWorld()->GetTimerManager().SetTimer(fadeTimer, this, [&]() { };, 1.0f, false);
}
