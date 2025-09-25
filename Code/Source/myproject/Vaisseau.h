#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vaisseau.generated.h"

class UStaticMeshComponent;
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

protected:
    // Mesh du vaisseau (Root et collision)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Composants")
    UStaticMeshComponent* MeshVaisseau;

    // Vitesse de déplacement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouvement")
    float Vitesse;

    // Entrées actuelles pour X/Y
    FVector2D InputActuel;

    void DeplacerAvantArriere(float Valeur);
    void DeplacerGaucheDroite(float Valeur);

    FVector ObtenirDirectionVersSouris();

    // Classe de missile
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tir")
    TSubclassOf<AMissile> MissileClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* CollisionBox;

    UFUNCTION()
    void OnOverlapAsteroide(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


    void Tirer(float Valeur);

    float DernierTir;

    // Vies du vaisseau
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vie")
    int Vie;
};
