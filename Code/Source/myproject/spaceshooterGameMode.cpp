#include "spaceshooterGameMode.h"


// conflit depuis main =)
ASpaceshooterGameMode::ASpaceshooterGameMode(){
	PlayerScore = 0;

}

void ASpaceshooterGameMode::addScore()
{
	PlayerScore += Points;
}
