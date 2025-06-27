// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SHPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionSystem/SHInteractableTargetActor.h"
#include "UI/SHGameHUD.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASHPlayerController::ASHPlayerController()
{

}

void ASHPlayerController::ResumeGame()
{
	if (!IsPaused())
	{
		return;
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	SetPause(false);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(CharacterMappingContext, 0, ModifyContextOptions);
		Subsystem->AddMappingContext(OpenUIMappingContext, 1, ModifyContextOptions);
	}

	ASHGameHUD* HUD = GetHUD<ASHGameHUD>();
	if (HUD != nullptr)
	{
		HUD->CloseWidgets();
	}

	if (GetViewTarget() != GetPawn())
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 0;
		TransitionParams.BlendExp = 0.0f;
		TransitionParams.BlendFunction = EViewTargetBlendFunction::VTBlend_PreBlended;
		TransitionParams.bLockOutgoing = true;

		SetViewTarget(GetPawn(), TransitionParams);
	}
}

void ASHPlayerController::OpenPauseMenu()
{
	PauseGame();

	ASHGameHUD* HUD = GetHUD<ASHGameHUD>();
	if (HUD != nullptr)
	{
		HUD->OpenPauseWidget();
	}
}

void ASHPlayerController::OpenItems()
{
	PauseGame();

	ASHGameHUD* HUD = GetHUD<ASHGameHUD>();
	if (HUD != nullptr)
	{
		HUD->OpenInventoryWidget();
	}
}

void ASHPlayerController::OpenMap()
{
	PauseGame();

	ASHGameHUD* HUD = GetHUD<ASHGameHUD>();
	if (HUD != nullptr)
	{
		HUD->OpenInventoryWidget(FSHInventoryPages::Map);
	}
}

void ASHPlayerController::OpenRecords()
{
	PauseGame();

	ASHGameHUD* HUD = GetHUD<ASHGameHUD>();
	if (HUD != nullptr)
	{
		HUD->OpenInventoryWidget(FSHInventoryPages::Records);
	}
}

void ASHPlayerController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);

	if (NewViewTarget->IsA<ASHInteractableTargetActor>())
	{
		OpenItems();
	}
}

void ASHPlayerController::PauseGame()
{
	if (IsPaused())
	{
		return;
	}

	SetPause(true);

	FInputModeGameAndUI InputMode = FInputModeGameAndUI();
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(PauseMappingContext, 0, ModifyContextOptions);
	}
}

void ASHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &ASHPlayerController::InputPause);
		EnhancedInputComponent->BindAction(OpenMapAction, ETriggerEvent::Started, this, &ASHPlayerController::InputMap);
		EnhancedInputComponent->BindAction(OpenItemsAction, ETriggerEvent::Started, this, &ASHPlayerController::InputItems);
		EnhancedInputComponent->BindAction(OpenRecordsAction, ETriggerEvent::Started, this, &ASHPlayerController::InputRecords);
	}
}

void ASHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->AddMappingContext(CharacterMappingContext, 0, ModifyContextOptions);
		Subsystem->AddMappingContext(OpenUIMappingContext, 1, ModifyContextOptions);
	}
}

void ASHPlayerController::InputPause(const FInputActionValue& Value)
{
	OpenPauseMenu();
}

void ASHPlayerController::InputMap(const FInputActionValue& Value)
{
	OpenMap();
}

void ASHPlayerController::InputItems(const FInputActionValue& Value)
{
	OpenItems();
}

void ASHPlayerController::InputRecords(const FInputActionValue& Value)
{
	OpenRecords();
}
