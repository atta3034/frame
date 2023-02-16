// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyboardActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


AKeyboardActor::AKeyboardActor()
{
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetBoxExtent(FVector(50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AKeyboardActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKeyboardActor::OnPlayKey);

	// 다이나믹 머티리얼 인스턴스를 생성한다.
	keyMat = UMaterialInstanceDynamic::Create(meshComp->GetMaterial(0), this);
	meshComp->SetMaterial(0, keyMat);
}

void AKeyboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyboardActor::OnPlayKey(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// 할당된 키 사운드를 출력한다.
	UGameplayStatics::PlaySoundAtLocation(this, keySound, GetActorLocation());

	// 키의 색상을 녹색으로 빛나게 한다.
	ChangeKeyColor(keyColor, glowPower);
	GetWorldTimerManager().ClearTimer(colorHandle);
	GetWorldTimerManager().SetTimer(colorHandle, this, &AKeyboardActor::ColorOff, 0.5f, false);
}

void AKeyboardActor::ColorOff()
{
	ChangeKeyColor(FVector4(1, 1, 1, 1), 0);
}

void AKeyboardActor::ChangeKeyColor(FVector4 color, float glow)
{
	keyMat->SetVectorParameterValue(TEXT("EmissiveColor"), color);
	keyMat->SetScalarParameterValue(TEXT("Glow"), glow);
}

