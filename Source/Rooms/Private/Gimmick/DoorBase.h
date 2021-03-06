// Copyright 2021 Yoshito Nakaue

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CPP_CharacterInterface.h"
#include "DoorBase.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed,
	ForwardOpened,
	BackOpened,
};

UCLASS()
class ACPP_DoorBase : public AActor, public ICPP_CharacterInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_DoorBase();

	UFUNCTION(BlueprintPure)
	EDoorState GetDoorState() const { return DoorState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Door open and close
	UFUNCTION(BlueprintCallable)
	void RotateDoor(float RotateAlpha);

	// Called when do action to door
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InteractDoor(bool IsForward);

	// Called when change the level
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeLevel();

	// Called when front collision begin overlap
	UFUNCTION()
	void OnFrontBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when behind collision begin overlap
	UFUNCTION()
	void OnBehindBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when begin door rotation
	UFUNCTION(BlueprintCallable)
	void BeginDoorRotation();

	// Called when end door rotation
	UFUNCTION(BlueprintCallable)
	void EndDoorRotation();

	// Door frame mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorFrameMesh;

	// Door mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorMesh;

	// Box collision in front of door
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* FrontCollision;

	// Box collision behind the door
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BehindCollision;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gimmick")
	TArray<TSoftObjectPtr<class UWorld>> DestLevelsForward;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gimmick")
	TArray<TSoftObjectPtr<class UWorld>> DestLevelsBackward;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Gimmick")
	bool bCanChangeLevel;

	// Door Rotation definition
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	FRotator ForwardOpenedRot;

	// Is player in front of door
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsInFront;

	// Is rotating door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsRotating;

	// Sound of opening the door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* OpeningSound;

	// Sound of closing the door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* ClosingSound;

	// Door opening state
	EDoorState DoorState;

	// Current level index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int LevelForwardIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int LevelBackwardIdx;

	/* Implementation interface function */
	// Called when interacted from character
	virtual void Interact_Implementation() override;

};
