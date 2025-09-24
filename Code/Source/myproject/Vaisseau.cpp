// Vaisseau.cpp
#include "Vaisseau.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Constructeur
AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	MeshVaisseau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshVaisseau"));
	MeshVaisseau->SetupAttachment(RootComponent);

	MeshVaisseau->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void AVaisseau::BeginPlay()
{
	Super::BeginPlay();
}

void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DirectionSouris = ObtenirDirectionVersSouris();
	if (!DirectionSouris.IsZero())
	{
		MeshVaisseau->SetRelativeRotation(DirectionSouris.Rotation());

		FVector Deplacement = DirectionSouris.GetSafeNormal() * InputActuel.X;
		FVector Tangente = FVector::CrossProduct(FVector::UpVector, DirectionSouris).GetSafeNormal();
		Deplacement += Tangente * InputActuel.Y;

		FVector NouvellePosition = GetActorLocation() + Deplacement * Vitesse * DeltaTime;
		NouvellePosition.Z = GetActorLocation().Z;

		NouvellePosition.X = FMath::Clamp(NouvellePosition.X, -3800.0f, 3800.0f);
		NouvellePosition.Y = FMath::Clamp(NouvellePosition.Y, -3800.0f, 3800.0f);

		SetActorLocation(NouvellePosition);
	}
}

void AVaisseau::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Avancer", this, &AVaisseau::DeplacerAvantArriere);
	PlayerInputComponent->BindAxis("Droite", this, &AVaisseau::DeplacerGaucheDroite);
}

void AVaisseau::DeplacerAvantArriere(float Valeur)
{
	InputActuel.X = FMath::Clamp(Valeur, -1.0f, 1.0f);
}

void AVaisseau::DeplacerGaucheDroite(float Valeur)
{
	InputActuel.Y = FMath::Clamp(Valeur, -1.0f, 1.0f);
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

// 🔻 Gestion des vies
void AVaisseau::PerdreVie()
{
	Vie--;
	UE_LOG(LogTemp, Warning, TEXT("Vie restante: %d"), Vie);

	// TODO : mettre à jour le HUD (cacher un cœur)

	if (Vie <= 0)
	{
		// Changer de scène
		UGameplayStatics::OpenLevel(this, "GameOver");
	}
}
