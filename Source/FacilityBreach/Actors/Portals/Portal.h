// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UENUM(BlueprintType)
enum class EPortalType : uint8
{
	PORTAL_Blue,
	PORTAL_Red
};

UCLASS()
class FACILITYBREACH_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

	virtual void Tick(float DeltaTime) override;

	void SetType(EPortalType InType);

	void LinkPortal(APortal* InPortal);
	void UnlinkPortal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly)
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(Category="Material", EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> PortalMaterialBlue;

	UPROPERTY(Category="Material", EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> PortalMaterialRed;

	UPROPERTY(Category="Material", EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> PortalMaterialCamera;

private:

	EPortalType Type;

	bool bLinked = false;
	TObjectPtr<APortal> OtherPortal;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetMeshMaterial();

};
