#include "Vaisseau.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h" // ✅ pour la box
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Missile.h"
#include "Asteroide.h" // ✅ ton asteroide
#include "GameFramework/PlayerController.h"

AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// ✅ RootComponent générique
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// ✅ Box collision attachée au root
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(150.f, 150.f, 150.f)); // Taille de la box
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	// ✅ Mesh attaché à la box
	MeshVaisseau = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshVaisseau"));
	MeshVaisseau->SetupAttachment(CollisionBox);
	MeshVaisseau->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Initialisation
	Vie = 3;
	Vitesse = 500.0f;
	DernierTir = -1.0f;
	InputActuel = FVector2D::ZeroVector;
}

void AVaisseau::BeginPlay()
{
	Super::BeginPlay();

	// ✅ Bind la fonction de collision
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVaisseau::OnOverlapAsteroide);
}

void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Vie actuelle: %d"), Vie);

	FVector DirectionSouris = ObtenirDirectionVersSouris();
	if (!DirectionSouris.IsZero())
	{
		// Rotation du mesh vers la souris
		MeshVaisseau->SetWorldRotation(DirectionSouris.Rotation());

		// Déplacement
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
}

// ✅ Fonction appelée quand la box overlap un astéroïde
void AVaisseau::OnOverlapAsteroide(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAsteroide* Asteroide = Cast<AAsteroide>(OtherActor);
	if (Asteroide)
	{
		Asteroide->Destroy();   // détruire l’astéroïde
		PerdreVie();            // perdre une vie
	}
}
