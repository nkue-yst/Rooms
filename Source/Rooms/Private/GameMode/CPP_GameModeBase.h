// Copyright 2021 Yoshito Nakaue

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameModeBase.generated.h"

UCLASS()
class ACPP_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACPP_GameModeBase(const FObjectInitializer& ObjectInitializer);
};
