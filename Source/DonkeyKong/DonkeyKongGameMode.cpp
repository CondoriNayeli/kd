// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKongGameMode.h"
#include "DonkeyKongCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ComponentPlataforma.h"
#include "Obstaculo.h"
#include "CubosDisparo.h"
#include "MuroLadrillo.h"
#include "Muro.h"
#include "MuroPegajoso.h"
#include "MuroHielo.h"
#include "MuroElectrico.h"
#include "MyHUD.h"

ADonkeyKongGameMode::ADonkeyKongGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Asigna tu clase HUD personalizada al GameMode
	HUDClass = AMyHUD::StaticClass();

}

void ADonkeyKongGameMode::BeginPlay()
{
	Super::BeginPlay();
	//tiempo para cubo disparo
	float TiempoInicial = FMath::RandRange(2.0f, 3.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_GenerarCubosDisparo, this, &ADonkeyKongGameMode::GenerarCuboDisparo, TiempoInicial, false);

	//PLATAFORMA
	FTransform SpawnLocationLadrillo;
	SpawnLocationLadrillo.SetLocation(FVector(1206.0f, -1117.0f, 1367.0f));
	SpawnLocationLadrillo.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	ladrillo = GetWorld()->SpawnActor<AMuroLadrillo>(AMuroLadrillo::StaticClass(), SpawnLocationLadrillo);

	FTransform SpawnLocationHielo;
	SpawnLocationHielo.SetLocation(FVector(1136.0f, 1182.0f, 2247.0f));
	SpawnLocationHielo.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	hielo = GetWorld()->SpawnActor<AMuroHielo>(AMuroHielo::StaticClass(), SpawnLocationHielo);

	FTransform SpawnLocationPegajoso;
	SpawnLocationPegajoso.SetLocation(FVector(1136.0f, -1217.0f, 2367.0f));
	SpawnLocationPegajoso.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	pegajoso = GetWorld()->SpawnActor<AMuroPegajoso>(AMuroPegajoso::StaticClass(), SpawnLocationPegajoso);

	FTransform SpawnLocationElectrico;
	SpawnLocationElectrico.SetLocation(FVector(1206.0f, 1162.0f, 557.0f));
	SpawnLocationElectrico.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	electrico = GetWorld()->SpawnActor<AMuroElectrico>(AMuroElectrico::StaticClass(), SpawnLocationElectrico);

	//PLATAFORMAS

	FVector posicionInicial = FVector(1250.0f, -1100.0f, -400.f);
	FRotator rotacionInicial = FRotator(0.0f, 0.0f, 10.0f);
	FTransform SpawnLocationCP;


	float anchoComponentePlataforma = 600.0f;
	float incrementoAltoComponentePlataforma = -105.0f;
	float incrementoAltoEntrePisos = 800.0f;
	float incrementoInicioPiso = 100.0f;

	bool obstaculoEnLadoIzquierdo = true;

	for (int npp = 0; npp < 7; npp++)
	{
		rotacionInicial.Roll = rotacionInicial.Roll * -1;
		incrementoInicioPiso = incrementoInicioPiso * -1;
		incrementoAltoComponentePlataforma = incrementoAltoComponentePlataforma * -1;
		SpawnLocationCP.SetRotation(FQuat(rotacionInicial));

		for (int ncp = 0; ncp < 5; ncp++)//5
		{
			if (ncp == 0 || ncp == 1 || ncp == 4)//4
			{
				float ajusteY = anchoComponentePlataforma * ncp;
				SpawnLocationCP.SetLocation(FVector(posicionInicial.X, posicionInicial.Y + ajusteY, posicionInicial.Z));
				AComponentPlataforma* plataforma = GetWorld()->SpawnActor<AComponentPlataforma>(AComponentPlataforma::StaticClass(), SpawnLocationCP);
				componentesPlataforma.Add(plataforma);

				if (ncp == 1)
				{
					FVector posicionObjetivo = FVector(posicionInicial.X, posicionInicial.Y + anchoComponentePlataforma * 3, posicionInicial.Z);
					plataforma->IniciarMovimiento(posicionObjetivo);

					FVector PosicionObstaculo;

					if (obstaculoEnLadoIzquierdo)
					{
						PosicionObstaculo = FVector(posicionInicial.X, posicionInicial.Y + 450.0f, posicionInicial.Z + 300.0f);//-200  x+100
					}
					else
					{
						PosicionObstaculo = FVector(posicionInicial.X, posicionInicial.Y + ajusteY + 1300.0f, posicionInicial.Z + 100.0f);//ajustey+200
					}

					AObstaculo* Obstaculo = GetWorld()->SpawnActor<AObstaculo>(AObstaculo::StaticClass(), PosicionObstaculo, FRotator::ZeroRotator);
					FVector PosicionObjetivoObstaculo = PosicionObstaculo + FVector(0.0f, 0.0f, 300.0f);// obstaculos cuadrados se muevan 
					Obstaculo->IniciarMovimiento(PosicionObjetivoObstaculo, 100.0f);//100 velocidad de movimiento
					Obstaculos.Add(npp, Obstaculo);

					obstaculoEnLadoIzquierdo = !obstaculoEnLadoIzquierdo;
				}

				if (ncp < 4)
				{
					posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma;
				}
			}
		}


		posicionInicial.Z = posicionInicial.Z + incrementoAltoEntrePisos;
		posicionInicial.Y = posicionInicial.Y + incrementoInicioPiso;
	}

}
	
void ADonkeyKongGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADonkeyKongGameMode::GenerarCuboDisparo()
{
	// Verificar que hay plataformas disponibles
	if (componentesPlataforma.Num() > 0)
	{
		int32 IndicePlataforma = FMath::RandRange(0, componentesPlataforma.Num() - 1);
		AComponentPlataforma* PlataformaSeleccionada = componentesPlataforma[IndicePlataforma];

		if (PlataformaSeleccionada && !PlataformasConCubos.Contains(PlataformaSeleccionada)) // Ver si la plataforma ya tiene un cubo de disparo
		{
			FVector PosicionPlataforma = PlataformaSeleccionada->GetActorLocation();
			FVector PosicionCubosDisparo = PosicionPlataforma + FVector(0.0f, 0.0f, 150.0f);

			// Crear el cubo de disparo
			ACubosDisparo* CuboDisparo = GetWorld()->SpawnActor<ACubosDisparo>(ACubosDisparo::StaticClass(), PosicionCubosDisparo, FRotator::ZeroRotator);

			if (CuboDisparo)
			{
				CubosDisparoArray.Add(CuboDisparo);
				PlataformasConCubos.Add(PlataformaSeleccionada);

				//temporizador para eliminar el cubo después de un tiempo
				FTimerHandle TimerHandle_DesaparecerCubo;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_DesaparecerCubo, [this, CuboDisparo, PlataformaSeleccionada]()
					{
						CuboDisparo->Destroy();
						PlataformasConCubos.Remove(PlataformaSeleccionada);

						// Después de destruir, generar otro cubo en otra plataform
						GenerarCuboDisparo();

					}, FMath::RandRange(3.0f, 6.0f), false);  // Desaparecer después de 3-6 segundos
			}

			// Tiempo hasta el próximo cubo
			float TiempoSiguienteCubo = FMath::RandRange(2.0f, 5.0f);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_GenerarCubosDisparo, this, &ADonkeyKongGameMode::GenerarCuboDisparo, TiempoSiguienteCubo, false);
		}
		else
		{
			GenerarCuboDisparo();
		}
	}
}

//void ADonkeyKongGameMode::GenerarCuboDisparo()
//{
//	// Verificar que hay plataformas disponibles
//	if (componentesPlataforma.Num() > 0)
//	{
//		int32 IndicePlataforma = FMath::RandRange(0, componentesPlataforma.Num() - 1);
//		AComponentPlataforma* PlataformaSeleccionada = componentesPlataforma[IndicePlataforma];
//
//		if (PlataformaSeleccionada && !PlataformasConCubos.Contains(PlataformaSeleccionada)) // Ver si la plataforma ya tiene un cubo de disparo
//		{
//			FVector PosicionPlataforma = PlataformaSeleccionada->GetActorLocation();
//			FVector PosicionCubosDisparo = PosicionPlataforma + FVector(0.0f, 0.0f, 150.0f);
//
//			// Crear el cubo de disparo
//			ACubosDisparo* CuboDisparo = GetWorld()->SpawnActor<ACubosDisparo>(ACubosDisparo::StaticClass(), PosicionCubosDisparo, FRotator::ZeroRotator);
//
//			if (CuboDisparo)
//			{
//				CubosDisparoArray.Add(CuboDisparo);
//				PlataformasConCubos.Add(PlataformaSeleccionada);
//
//				//temporizador para eliminar el cubo después de un tiempo
//				FTimerHandle TimerHandle_DesaparecerCubo;
//				GetWorld()->GetTimerManager().SetTimer(TimerHandle_DesaparecerCubo, [this, CuboDisparo, PlataformaSeleccionada]()
//					{
//						CuboDisparo->Destroy();
//						PlataformasConCubos.Remove(PlataformaSeleccionada);
//
//						// Después de destruir, generar otro cubo en otra plataform
//						GenerarCuboDisparo();
//
//					}, FMath::RandRange(3.0f, 6.0f), false);  // Desaparecer después de 3-6 segundos
//			}
//
//			// Tiempo hasta el próximo cubo
//			float TiempoSiguienteCubo = FMath::RandRange(2.0f, 5.0f);
//			GetWorld()->GetTimerManager().SetTimer(TimerHandle_GenerarCubosDisparo, this, &ADonkeyKongGameMode::GenerarCuboDisparo, TiempoSiguienteCubo, false);
//		}
//		else
//		{
//			GenerarCuboDisparo();
//		}
//	}
//}
//
//





