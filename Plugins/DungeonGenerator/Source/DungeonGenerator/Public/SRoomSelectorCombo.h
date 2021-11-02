// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


#define LOCTEXT_NAMESPACE "SRoomSelectorCombo"

/**
 * 
 */
template< typename OptionType >
class DUNGEONGENERATOR_API SRoomSelectorCombo : public SCompoundWidget
{
	public:
	using TSelectorComboType = OptionType;

	SLATE_BEGIN_ARGS(SRoomSelectorCombo)
		: _Source()
	{}
		SLATE_ATTRIBUTE(TArray<TSelectorComboType>*, Source)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs)
	{
		Options = InArgs._Source.Get();
	
		ChildSlot
		[
			SNew(SComboBox<TSelectorComboType>)
			.OptionsSource(Options)
			.OnSelectionChanged(this, &SRoomSelectorCombo::OnSelectionChanged)
			.OnGenerateWidget(this, &SRoomSelectorCombo::MakeWidgetForOption)
			.InitiallySelectedItem(CurrentOption)
			[
				SNew(STextBlock)//.Text(LOCTEXT("None", "None"))
				.Text_Lambda([this]() -> FText
				{
					return CurrentOption ? FText::FromString(*CurrentOption->GetName()) : LOCTEXT("None", "None");
				})
			]
			//SNew(STextBlock).Text(LOCTEXT("None", "None"))
		];
	
	}

	//void AddOptionItem(TSelectorComboType Item);
	void OnSelectionChanged(TSelectorComboType NewValue, ESelectInfo::Type)
	{
		CurrentOption = NewValue;
	}
	
	TSharedRef<SWidget> MakeWidgetForOption(TSelectorComboType InOption)
	{
		return SNew(STextBlock).Text(FText::FromString(*InOption->GetName()));
	}

	TArray<TSelectorComboType>* Options;
	TSelectorComboType CurrentOption;
};

#undef LOCTEXT_NAMESPACE