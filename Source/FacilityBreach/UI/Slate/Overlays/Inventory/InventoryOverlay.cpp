#include "InventoryOverlay.h"

#include "InventorySlot.h"
#include "IPropertyTable.h"
#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/Layout/SUniformGridPanel.h"

void SInventoryOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	TitleFont.Size = 22.f;

	// Populating grid
	GridPanel = SNew(SUniformGridPanel)
		.SlotPadding(10.f);
	Inventory.SetNum(Rows);
	for (int32 Row = 0; Row < Rows; Row++)
	{
		Inventory[Row].SetNum(Cols);
		for (int32 Col = 0; Col < Cols; Col++)
		{
			Inventory
			[
				Row
			]
			[
				Col
			] = SNew(SInventorySlot);
			GridPanel->AddSlot(Col, Row)
			[
				Inventory[Row][Col].ToSharedRef()
			];
		}
	}


	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(800.f)
			.HeightOverride(500.f)
			[
				SNew(SBorder)
				.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Inventory.Background"))
				[

					SNew(SBox)
					.Padding(30.f)
					[

						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Font(TitleFont)
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString("Inventory"))
						]

						+ SVerticalBox::Slot()
						[
							GridPanel.ToSharedRef()
						]
					]


				]
			]
		]
	];
}

void SInventoryOverlay::OnToggle()
{
	if (MainOverlay.IsValid())
	{
		if (MainOverlay->GetVisibility() == EVisibility::Collapsed)
		{
			MainOverlay->SetVisibility(EVisibility::Visible);
		}
		else
		{
			MainOverlay->SetVisibility(EVisibility::Collapsed);
		}
	}
}

void SInventoryOverlay::OnAddItem(FString ItemName)
{
	if (GridPanel.IsValid() == false)
	{
		return;
	}

	Inventory[CurrentRow][CurrentCol]->Update(FText::FromString(ItemName));

	CurrentCol++;
	if (CurrentCol >= Cols)
	{
		CurrentCol = 0;
		CurrentRow++;

		if (CurrentRow >= Inventory.Num())
		{
			// New Row
			TArray<TSharedPtr<SInventorySlot>> NewRow;
			NewRow.SetNum(Cols);

			Inventory.Add(NewRow);

			for (int32 Col = 0; Col < Cols; Col++)
			{
				Inventory
				[
					CurrentRow
				]
				[
					Col
				] = SNew(SInventorySlot);
				GridPanel->AddSlot(Col, CurrentRow)
				[
					Inventory[CurrentRow][Col].ToSharedRef()
				];
			}
		}
	}
}
