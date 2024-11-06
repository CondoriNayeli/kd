// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "DonkeyKongCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Mono.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();

   
 if (Canvas)  // Verifica que Canvas no sea null
    {
        BarradeSalud();
    }
}


void AMyHUD::UpdateHealthBar(float Health)
{
   
}

void AMyHUD::BarradeSalud()
{
    ADonkeyKongCharacter* avatar = Cast<ADonkeyKongCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (avatar && Canvas)  // Verifica que el avatar y el Canvas existan
    {
        // calcula el porcentaje de salud
        float Salud = FMath::Clamp(avatar->HP / avatar->MaxHP, 0.0f, 1.0f);

        // se dibuja la barra de salud
        FVector2D ViewPortSize;
        GetWorld()->GetGameViewport()->GetViewportSize(ViewPortSize); //tamaño de la ventan
        float x = ViewPortSize.X - ViewPortSize.X * 0.3f - 10.0f;
        float y = Canvas->SizeY - ViewPortSize.Y + 10.0f;
        float w = ViewPortSize.X * 0.3f * Salud;
        float h = ViewPortSize.Y * 0.05f;

        DrawRect(FLinearColor::Black, x, y, w, h);  // Fondo negro
        DrawRect(FLinearColor(1-Salud, Salud,0,1),x+10,y+10,w*Salud-10,h-20);  // Barra de salud

        // Mensaje de depuración para verificar que el avatar existe
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Barra de Salud Dibujada"));
    }
    //else
    //{
    //    // Mensaje de depuración para saber si no encuentra el personaje o el Canvas
    //    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("FIN DEL JUEGO"));
    //}
    

}
