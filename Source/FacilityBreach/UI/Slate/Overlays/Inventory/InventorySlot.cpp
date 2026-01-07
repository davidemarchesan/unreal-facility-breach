#include "InventorySlot.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SInventorySlot::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(MainBorder, SBorder)
		.BorderImage(Brush_Standard)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(7.f)
			[
				SAssignNew(TextBlock, STextBlock)
				.Justification(ETextJustify::Center)
				.Text(FText::GetEmpty())
			]
			
		]

	];
}

void SInventorySlot::Update(FText InText)
{
	if (MainBorder.IsValid())
	{
		MainBorder->SetBorderImage(Brush_Active);
	}
	if (TextBlock.IsValid())
	{
		TextBlock->SetText(InText);
	}
}
