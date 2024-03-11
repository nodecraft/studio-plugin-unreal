// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerCardLabel.h"
#include "Components/Image.h"


void UServerCardLabel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UServerCardLabel::SetLabelType(EServerCardLabelType InLabelType)
{
	LabelType = InLabelType;
	Background->SetBrushColor(StyleMap.Find(LabelType)->BackgroundColor);
	LabelText->SetText(StyleMap.Find(LabelType)->LabelTitle);
	//check for icon present
	if (StyleMap.Find(LabelType)->LabelIconImage)
	{
		Icon->SetVisibility(ESlateVisibility::Visible);
		Icon->SetBrushFromTexture(StyleMap.Find(LabelType)->LabelIconImage);
	}
	else
	{
		Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

