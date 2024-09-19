// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayAttribute ModifiedAttribute = Data.EvaluatedData.Attribute;

    if (ModifiedAttribute == GetStaminaAttribute())
    {
        // Use FMath::Clamp to ensure Stamina is within the correct range
        float NewStamina = FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue());
        Stamina.SetCurrentValue(NewStamina);

        // Get the current and max stamina
        float CurrentStamina = Stamina.GetCurrentValue();
        float MaxStaminaValue = MaxStamina.GetCurrentValue();

        // Cap stamina at max stamina
        if (CurrentStamina > MaxStaminaValue)
        {
            Stamina.SetCurrentValue(MaxStaminaValue); // Set the current value
        }

        /* Optionally set the base value to ensure it doesn't affect future effects
        float BaseStamina = Stamina.GetBaseValue();
        if (BaseStamina > MaxStaminaValue)
        {
            Stamina.SetBaseValue(MaxStaminaValue);
        }*/
	}
}