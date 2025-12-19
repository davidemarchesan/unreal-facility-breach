// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "Components/SceneCaptureComponent2D.h"

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCapture2D"));
	SceneCapture->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPlayerBeginOverlap);
		BoxComponent->SetGenerateOverlapEvents(true);
	}

	// if (SceneCapture && RenderTarget2D)
	// {
	// SceneCapture->TextureTarget = RenderTarget2D;
	// SceneCapture->CaptureSource = SCS_FinalColorLDR;
	// SceneCapture->bCaptureEveryFrame = true;
	// SceneCapture->bCaptureOnMovement = true;
	// }
}

void APortal::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("APortal::OnPlayerBeginOverlap"));

	// AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(OtherActor);
	// if (Character && OtherPortal)
	// {
	// 	
	// 	Character->SetActorLocation(OtherPortal->GetActorLocation());
	// }
}

void APortal::ResetMeshMaterial()
{
	if (MeshComponent)
	{
		switch (Type)
		{
		case EPortalType::PORTAL_Blue:
			MeshComponent->SetMaterial(0, PortalMaterialBlue);
			break;

		default:
		case EPortalType::PORTAL_Red:
			MeshComponent->SetMaterial(0, PortalMaterialRed);
			break;
		}
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLinked == true)
	{
		
	}
}

void APortal::SetType(EPortalType InType)
{
	Type = InType;

	ResetMeshMaterial();
}

void APortal::LinkPortal(APortal* InPortal)
{
	OtherPortal = InPortal;
	if (MeshComponent)
	{
		MeshComponent->SetMaterial(0, PortalMaterialCamera);
	}

	bLinked = true;
}

void APortal::UnlinkPortal()
{
	OtherPortal = nullptr;

	ResetMeshMaterial();

	bLinked = false;
}
