// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DonkeyKongCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Crear un componente de colisión (Usamos SphereComponent)
	USphereComponent* CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic")); // Hacer que bloquee todo


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Projectile(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_Projectile"));
	ProjectileMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ProjectileMesh->SetStaticMesh(Projectile.Object);
	ProjectileMesh->SetVisibility(true);/// es para que aparezca en la pantalla
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	SetRootComponent(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement_Projectile"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	// Añadir detección de impactos
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	InitialLifeSpan = 2.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
	void AProjectile::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

	}

	/*void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
	{
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
		}

		this->Destroy();
	}*/
	void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
	{
		// Verifica que el actor golpeado no sea nulo y no sea el proyectil mismo
		if (OtherActor != nullptr && OtherActor != this)
		{
			// Comprueba si el actor golpeado es un personaje
			ADonkeyKongCharacter* Character = Cast<ADonkeyKongCharacter>(OtherActor);
			if (Character)
			{
				// plica daño al personaje
				Character->ApplyDamage(5.0f); // Cambia 10.0f por el daño que deseas aplicar
			}

			// Si el objeto puede simular física, añade impulso
			if (OtherComp && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
			}
		}

		// Destruye el proyectil
		this->Destroy();
	}