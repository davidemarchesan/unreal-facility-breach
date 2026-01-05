// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectivesManager.h"

// Sets default values
AGameObjectivesManager::AGameObjectivesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameObjectivesManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameObjectivesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

