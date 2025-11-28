// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "mcharacter.h"


AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerCharacterClass(
		TEXT("/Game/Blueprint/Character/BP_Character"));
		DefaultPawnClass = PlayerCharacterClass.Class;
	
	
	
}
