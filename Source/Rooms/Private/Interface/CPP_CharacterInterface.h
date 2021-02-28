// Copyright 2021 Yoshito Nakaue


#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_CharacterInterface.generated.h"

UINTERFACE(Blueprintable)
class UCPP_CharacterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ROOMS_API ICPP_CharacterInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// Interaction to Object(Door etc...)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();

};
