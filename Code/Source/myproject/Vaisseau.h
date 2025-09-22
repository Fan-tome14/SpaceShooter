// Vaisseau.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vaisseau.generated.h"

UCLASS()
class MYPROJECT_API AVaisseau : public APawn
{
	GENERATED_BODY()

public:
	AVaisseau();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Mesh du vaisseau
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Composants", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshVaisseau;

	// Vitesse de déplacement
	UPROPERTY(EditAnywhere, Category = "Mouvement")
	float Vitesse = 500.0f;

	// Entrée clavier (X=avant/arrière, Y=gauche/droite)
	FVector InputActuel;

	// Fonctions pour le déplacement
	void DeplacerAvantArriere(float Valeur);
	void DeplacerGaucheDroite(float Valeur);

	// Récupérer la position de la souris et calculer la direction
	FVector ObtenirDirectionVersSouris();
};
