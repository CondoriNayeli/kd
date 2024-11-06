// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */

UCLASS()
class DONKEYKONG_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMyHUD();
public:
	void BarradeSalud(); 
	void DrawHUD();
	// Nuevo método para actualizar la barra de salud
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealthBar(float Health); //actualizar
};

