// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

UScoreWidget::UScoreWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StoreWidgetAnimations();
	ComboFadeAnimation = GetAnimationByName(TEXT("ComboFade"));
	ComboShakeAnimation = GetAnimationByName(TEXT("ComboShake"));
}

void UScoreWidget::UpdateScore(int32 Value)
{
    // only update if more than one hit
    if (TXTScore && Value > 0)
    {
        if (TXTScore->Visibility == ESlateVisibility::Hidden)
        {
            TXTScore->SetVisibility(ESlateVisibility::Visible);
        }
        TXTScore->SetText(FText::FromString(("Score : " + FString::FromInt(Value))));
        if (ComboShakeAnimation)
        {
            PlayAnimation(ComboShakeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
        }
        if (ComboFadeAnimation)
        {
            PlayAnimation(ComboFadeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
        }
    }
}

void UScoreWidget::StoreWidgetAnimations()
{
    AnimationsMap.Empty();
    FProperty* Prop = GetClass()->PropertyLink;
    // check each property of this class
    while (Prop)
    {
        // only evaluate object properties, skip rest
        if (Prop->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty* ObjProp = CastField<FObjectProperty>(Prop);
            // only get back properties that are of type widget animation
            if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
                // only get back properties that are of type widget animation
                UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);
                // if casting worked update map with new animation
                if (WidgetAnimation && WidgetAnimation->MovieScene)
                {
                    FName AnimName = WidgetAnimation->MovieScene->GetFName();
                    //GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, AnimName.ToString());
                    AnimationsMap.Add(AnimName, WidgetAnimation);
                }
            }
        }
        Prop = Prop->PropertyLinkNext;
    }
}

UWidgetAnimation* UScoreWidget::GetAnimationByName(FName AnimationName) const
{
    UWidgetAnimation* const* WidgetAnimation = AnimationsMap.Find(AnimationName);
    if (WidgetAnimation)
    {
        return *WidgetAnimation;
    }
    return nullptr;
}