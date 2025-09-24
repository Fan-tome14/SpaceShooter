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

	// Gestion des vies
	void PerdreVie();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Composants", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshVaisseau;

	UPROPERTY(EditAnywhere, Category = "Mouvement")
	float Vitesse = 500.0f;

	FVector InputActuel;

	void DeplacerAvantArriere(float Valeur);
	void DeplacerGaucheDroite(float Valeur);

	FVector ObtenirDirectionVersSouris();

	// Vies du joueur
	UPROPERTY(EditAnywhere, Category = "Vie")
	int Vie = 3;
};
