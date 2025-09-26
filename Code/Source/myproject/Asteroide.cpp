#include "Asteroide.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceShooterGameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

AAsteroide::AAsteroide()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshAsteroide"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

void AAsteroide::BeginPlay()
{
	Super::BeginPlay();

	// --- Vie ---
	Vies = FMath::RandRange(1, 3);
	InialLife = Vies;

	// --- Cible ---
	if (CibleClass)
	{
		TArray<AActor*> FoundPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CibleClass, FoundPawns);
		if (FoundPawns.Num() > 0)
			CiblePawn = Cast<APawn>(FoundPawns[0]);
	}

	if (!CiblePawn)
		CiblePawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	// --- Scale aléatoire ---
	float RandomScale = FMath::FRandRange(0.008f, 0.01f);
	Mesh->SetWorldScale3D(FVector(RandomScale));

	// --- Rotation initiale aléatoire ---
	RotationSpeed = FVector(
		FMath::FRandRange(-60.f, 60.f), // Pitch speed en deg/s
		FMath::FRandRange(-60.f, 60.f), // Yaw speed
		FMath::FRandRange(-60.f, 60.f)  // Roll speed
	);
}

void AAsteroide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// --- Rotation du mesh ---
	if (Mesh)
	{
		FRotator NewRotation = Mesh->GetComponentRotation();
		NewRotation.Pitch += RotationSpeed.X * DeltaTime;
		NewRotation.Yaw += RotationSpeed.Y * DeltaTime;
		NewRotation.Roll += RotationSpeed.Z * DeltaTime;
		Mesh->SetWorldRotation(NewRotation);
	}

	// --- Mouvement vers le vaisseau ---
	if (!CiblePawn) return;

	FVector DirectionToVaisseau = (CiblePawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + DirectionToVaisseau * Vitesse * DeltaTime;
	SetActorLocation(NewLocation);

	FVector VaisseauPos = CiblePawn->GetActorLocation();
	FVector Pos = GetActorLocation();
	if (Pos.X < VaisseauPos.X - 1130.f || Pos.X > VaisseauPos.X + 1130.f ||
		Pos.Y < VaisseauPos.Y - 2460.f || Pos.Y > VaisseauPos.Y + 2460.f)
	{
		Destroy();
	}
}

void AAsteroide::RecevoirDegat()
{
	Vies--;

	if (ASpaceshooterGameMode* GM = Cast<ASpaceshooterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->addScore();
	}

	if (Vies <= 0)
	{
		// Cacher le mesh immédiatement
		if (Mesh)
		{
			Mesh->SetVisibility(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// Jouer le son d'explosion une seule fois
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
		}

		// Spawn de l’effet d’explosion attaché à l’acteur
		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(
				ExplosionEffect,
				RootComponent,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true // AutoDestroy
			);
		}

		// Arrêter le mouvement en désactivant le tick
		SetActorTickEnabled(false);

		// Détruire l’astéroïde (et l’effet attaché) après 1 seconde
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&AAsteroide::DestroyAsteroide,
			1.0f,
			false
		);
	}

}

void AAsteroide::DestroyAsteroide()
{
	Destroy();
}
