// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APickUpActor::APickUpActor()
{
	PrimaryActorTick.bCanEverTick = false;

	
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

