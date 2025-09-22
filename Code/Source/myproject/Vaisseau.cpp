#include "Vaisseau.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Constructeur
AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// Racine (permet de gérer position/collisions)
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	// Mesh attaché à la racine
	MeshVaisseau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshVaisseau"));
	MeshVaisseau->SetupAttachment(RootComponent);

	// Échelle
	MeshVaisseau->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void AVaisseau::BeginPlay()
{
	Super::BeginPlay();
}

void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direction vers la souris
	FVector DirectionSouris = ObtenirDirectionVersSouris();
	if (!DirectionSouris.IsZero())
	{
		// Rotation seulement du mesh
		MeshVaisseau->SetRelativeRotation(DirectionSouris.Rotation());

		// Calcul déplacement
		FVector Deplacement = DirectionSouris.GetSafeNormal() * InputActuel.X; // avant/arrière
		FVector Tangente = FVector::CrossProduct(FVector::UpVector, DirectionSouris).GetSafeNormal();
		Deplacement += Tangente * InputActuel.Y; // gauche/droite

		// Position proposée
		FVector NouvellePosition = GetActorLocation() + Deplacement * Vitesse * DeltaTime;
		NouvellePosition.Z = GetActorLocation().Z; // reste sur le sol

		// 🔒 Clamp dans la zone [-3800, 3800]
		NouvellePosition.X = FMath::Clamp(NouvellePosition.X, -3800.0f, 3800.0f);
		NouvellePosition.Y = FMath::Clamp(NouvellePosition.Y, -3800.0f, 3800.0f);

		// Appliquer la nouvelle position
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
