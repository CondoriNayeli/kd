// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKongCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"
#include "Components/PrimitiveComponent.h"   // Para UPrimitiveComponent
#include "Components/BoxComponent.h"         // Para UBoxComponent (si usas un Box Component)
#include "Components/SphereComponent.h"      // Para USphereComponent (si usas un Sphere Component)
#include "GameFramework/Actor.h"             // Para la clase base de Actor
#include "Mono.h"
#include "TimerManager.h"

ADonkeyKongCharacter::ADonkeyKongCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1500.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Inicializa HP y MaxHP
	HP = 100.0f; 
	MaxHP = 100.0f; 

	//Vidas = 3;



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADonkeyKongCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADonkeyKongCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADonkeyKongCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADonkeyKongCharacter::TouchStopped);

	
}

void ADonkeyKongCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ADonkeyKongCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ADonkeyKongCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}


void ADonkeyKongCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Obtener referencia al HUD
	MyHUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	// Comprobar si el HUD se ha inicializado correctamente
	//if (MyHUD)
	//{
	//	// Puedes hacer otras inicializaciones si es necesario
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No se pudo obtener el HUD!"));
	//}
}


void ADonkeyKongCharacter::ApplyDamage(int32 DamageAmount)
{
	HP -= DamageAmount; // Disminuye la salud

	// Comprobación si la salud es menor o igual a cero
	if (HP <= 0)
	{
		//HP = 0; // Asegúrate de que no sea negativa
		// Aquí puedes manejar lo que sucede cuando el personaje muere
		//UE_LOG(LogTemp, Warning, TEXT("¡El personaje ha muerto!"));
		this->Destroy(); // O cualquier otra lógica que necesites
	}

	// Actualiza la barra de salud en el HUD
	if (MyHUD)
	{
		MyHUD->UpdateHealthBar(HP); // actualización de la barra de salud
	}
}

//*****


