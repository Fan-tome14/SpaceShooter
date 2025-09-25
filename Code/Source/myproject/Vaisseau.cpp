#include "Vaisseau.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Missile.h"
#include "GameFramework/PlayerController.h"

AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root fixe
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	RootComponent->SetMobility(EComponentMobility::Movable);

	// Capsule de collision
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(RootComponent);
	CapsuleCollision->InitCapsuleSize(50.f, 100.f);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Mesh du vaisseau
	MeshVaisseau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshVaisseau"));
	MeshVaisseau->SetupAttachment(CapsuleCollision);
	MeshVaisseau->SetRelativeLocation(FVector::ZeroVector);

	// Initialisation
	Vie = 3;
	Vitesse = 500.0f;
	DernierTir = -1.0f;
	InputActuel = FVector2D::ZeroVector;
}

void AVaisseau::BeginPlay()
{
	Super::BeginPlay();
}

void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Vie actuelle: %d"), Vie);

	FVector DirectionSouris = ObtenirDirectionVersSouris();
	if (!DirectionSouris.IsZero())
	{
		MeshVaisseau->SetRelativeRotation(DirectionSouris.Rotation());

		FVector Deplacement = DirectionSouris.GetSafeNormal() * InputActuel.X;
		FVector Tangente = FVector::CrossProduct(FVector::UpVector, DirectionSouris).GetSafeNormal();
		Deplacement += Tangente * InputActuel.Y;

		FVector NouvellePosition = GetActorLocation() + Deplacement * Vitesse * DeltaTime;
		NouvellePosition.Z = GetActorLocation().Z;

		SetActorLocation(NouvellePosition);
	}
}

void AVaisseau::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Avancer", this, &AVaisseau::DeplacerAvantArriere);
	PlayerInputComponent->BindAxis("Droite", this, &AVaisseau::DeplacerGaucheDroite);
	PlayerInputComponent->BindAxis("Tirer", this, &AVaisseau::Tirer);
}

void AVaisseau::DeplacerAvantArriere(float Valeur)
{
	InputActuel.X = FMath::Clamp(Valeur, -1.f, 1.f);
}

void AVaisseau::DeplacerGaucheDroite(float Valeur)
{
	InputActuel.Y = FMath::Clamp(Valeur, -1.f, 1.f);
}

FVector AVaisseau::ObtenirDirectionVersSouris()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return FVector::ZeroVector;

	FVector SourisLocation, SourisDirection;
	if (PC->DeprojectMousePositionToWorld(SourisLocation, SourisDirection))
	{
		FVector ActorLocation = GetActorLocation();
		float Distance = (ActorLocation.Z - SourisLocation.Z) / SourisDirection.Z;
		FVector PointSurSol = SourisLocation + SourisDirection * Distance;

		FVector Direction = PointSurSol - ActorLocation;
		Direction.Z = 0;
		return Direction;
	}
	return FVector::ZeroVector;
}

void AVaisseau::Tirer(float Valeur)
{
	if (Valeur > 0.5f && MissileClass)
	{
		float TempsActuel = GetWorld()->GetTimeSeconds();
		if (TempsActuel - DernierTir > 0.3f)
		{
			DernierTir = TempsActuel;

			FVector SpawnLocation = MeshVaisseau->GetComponentLocation() + MeshVaisseau->GetForwardVector() * 150.f;
			FRotator SpawnRotation = MeshVaisseau->GetComponentRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AMissile* Missile = GetWorld()->SpawnActor<AMissile>(MissileClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (Missile) Missile->InitDirection(MeshVaisseau->GetForwardVector());
		}
	}
}

void AVaisseau::PerdreVie()
{
	Vie--;
	UE_LOG(LogTemp, Warning, TEXT("Vie perdue ! Vie restante: %d"), Vie);
	if (Vie <= 0)
	{
		UGameplayStatics::OpenLevel(this, "GameOver");
	}
}
