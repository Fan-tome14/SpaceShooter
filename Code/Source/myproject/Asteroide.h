#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroide.generated.h"

class AVaisseau;

UCLASS()
class MYPROJECT_API AAsteroide : public AActor
{
	GENERATED_BODY()

public:
	// Constructeur
	AAsteroide();

protected:
	// Appelé au lancement du jeu
	virtual void BeginPlay() override;

public:
	// Tick à chaque frame
	virtual void Tick(float DeltaTime) override;

private:
	// Vitesse de déplacement de l’astéroïde
	UPROPERTY(EditAnywhere, Category = "Mouvement")
	float Vitesse = 200.0f;

	// Classe du Pawn à suivre
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cible", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> CibleClass;

	// Direction du mouvement (si jamais tu veux lui donner une direction fixe)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroide", meta = (AllowPrivateAccess = "true"))
	FVector Direction;

	// Instance réelle de la cible (joueur ou autre)
	APawn* CiblePawn = nullptr;

	// Mesh visible (modifiable dans les Blueprints !)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroide", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
};
