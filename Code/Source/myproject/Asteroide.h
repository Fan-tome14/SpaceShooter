#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroide.generated.h"

UCLASS()
class MYPROJECT_API AAsteroide : public AActor
{
	GENERATED_BODY()

public:
	AAsteroide();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Fonction appel�e quand un missile touche l'ast�ro�de
	void PrendreDegat();

private:
	UPROPERTY(EditAnywhere, Category = "Mouvement")
	float Vitesse = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Cible")
	TSubclassOf<APawn> CibleClass;

	APawn* CiblePawn = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	// --- Gestion des vies ---
	UPROPERTY(EditAnywhere, Category = "Asteroide")
	int Vies = 1; // Valeur par d�faut
};
