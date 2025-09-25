#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vaisseau.generated.h"

class AMissile;

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

    void PerdreVie();

protected: // <-- important pour BlueprintReadWrite
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Composants")
    class UStaticMeshComponent* MeshVaisseau;

    UPROPERTY(EditAnywhere, Category = "Mouvement")
    float Vitesse = 500.0f;

    FVector InputActuel;

    void DeplacerAvantArriere(float Valeur);
    void DeplacerGaucheDroite(float Valeur);

    FVector ObtenirDirectionVersSouris();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
    TSubclassOf<class AMissile> MissileClass;

    void Tirer(float Valeur);

    float DernierTir = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Vie")
    int Vie = 3;
};
