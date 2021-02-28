// Copyright 2021 Yoshito Nakaue


#include "Gimmick/DoorBase.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorBase::ADoorBase()
	: ForwardOpenedRot(0.f, 100.f, 0.f)
	, bIsInFront(true)
	, bIsRotating(false)
	, bCanChangeLevel(true)
	, OpeningSound(nullptr)
	, ClosingSound(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Door frame mesh
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	RootComponent = DoorFrameMesh;

	// Initialize Door mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);
	DoorMesh->AddLocalOffset(FVector(55.0f, 0.f, 0.f));

	// Initialize box Collision
	FrontCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontBoxCollision"));
	FrontCollision->SetupAttachment(DoorFrameMesh);
	FrontCollision->InitBoxExtent(FVector(100.f, 35.f, 100.f));
	FrontCollision->SetRelativeLocation(FVector(0.f, 90.f, 100.f));
	FrontCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorBase::OnFrontBeginOverlap);

	BehindCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BehindBoxCollision"));
	BehindCollision->SetupAttachment(DoorFrameMesh);
	BehindCollision->InitBoxExtent(FVector(100.f, 35.f, 100.f));
	BehindCollision->SetRelativeLocation(FVector(0.f, -90.f, 100.f));
	BehindCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorBase::OnBehindBeginOverlap);

}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	// Load initial levels
	if (DestLevelsForward.IsValidIndex(0))
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, DestLevelsForward[0], true, true, LatentInfo);
	}

	if (DestLevelsBackward.IsValidIndex(0))
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, DestLevelsBackward[0], true, true, LatentInfo);
	}
}

// Door open and close
void ADoorBase::RotateDoor(float RotateAlpha)
{
	DoorMesh->SetRelativeRotation(RotateAlpha * ForwardOpenedRot);
}

// Called when front collision begin overlap
void ADoorBase::OnFrontBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerPawn(this, 0) == OtherActor)
	{
		bIsInFront = true;
	}
}

// Called when behind collision begin overlap
void ADoorBase::OnBehindBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerPawn(this, 0) == OtherActor)
	{
		bIsInFront = false;
	}
}

void ADoorBase::BeginDoorRotation()
{
	bIsRotating = true;
	switch (DoorState)
	{
	case EDoorState::Closed:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpeningSound, this->GetActorLocation());
		break;

	case EDoorState::ForwardOpened:
	case EDoorState::BackOpened:
		TFunction<void(void)> _Func = [this]()
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClosingSound, this->GetActorLocation());
		};
		FTimerHandle _Handle;
		GetWorld()->GetTimerManager().SetTimer(_Handle, (TFunction<void(void)>&&)_Func, 0.5f, false);
		break;

	}
}

void ADoorBase::EndDoorRotation()
{
	bIsRotating = false;
	if (DoorState == EDoorState::Closed)
	{

	}
}

// Called when interacted from character
void ADoorBase::Interact_Implementation()
{
	if (bIsRotating)
		return;

	// Call event in Blueprint
	this->InteractDoor(bIsInFront);

	// Update DoorState
	switch (DoorState)
	{
	case EDoorState::Closed:
		if (bIsInFront)
			DoorState = EDoorState::ForwardOpened;
		else
			DoorState = EDoorState::BackOpened;
		break;

	case EDoorState::ForwardOpened:
	case EDoorState::BackOpened:
		DoorState = EDoorState::Closed;
		break;

	default:
		break;
	}
}
