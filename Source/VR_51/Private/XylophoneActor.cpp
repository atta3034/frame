// Fill out your copyright notice in the Description page of Project Settings.


#include "XylophoneActor.h"
#include "Components/SceneComponent.h"
#include "Components/ChildActorComponent.h"
#include "KeyboardActor.h"


AXylophoneActor::AXylophoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	for (int32 i = 0; i < 8; i++)
	{
		FName componentName = FName(*FString::Printf(TEXT("Keyboard_%d"), i+1));
		childComps.Add(CreateDefaultSubobject<UChildActorComponent>(componentName));
		childComps[i]->SetupAttachment(RootComponent);
	}
}

void AXylophoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	//for (int32 i = 0; i < childComps.Num(); i++)
	//{
	//	childComps[i]->SetChildActorClass(keyboard);
	//}
}

void AXylophoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

