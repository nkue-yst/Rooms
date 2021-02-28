// Copyright 2021 Yoshito Nakaue


#include "GameMode/CPP_GameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ACPP_GameModeBase::ACPP_GameModeBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<APawn> PawnClass(TEXT("/Game/Blueprints/Character/BP_PlayerCharacter"));
    if (PawnClass.Succeeded())
        DefaultPawnClass = PawnClass.Class;

    static ConstructorHelpers::FClassFinder<AController> ControllerClass(TEXT("/Game/Blueprints/Character/BP_PlayerController"));
    if (ControllerClass.Succeeded())
        PlayerControllerClass = ControllerClass.Class;
}
