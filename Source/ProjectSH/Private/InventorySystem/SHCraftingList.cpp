// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHCraftingList.h"

bool USHCraftingList::GetCraftRecipe(USHBaseItem* FirstItem, USHBaseItem* SecondItem, FSHCraftRecipe& CraftRecipe) const
{
	for (FSHCraftRecipe Recipe : Crafts)
	{
		if ((Recipe.FirstItem == FirstItem && Recipe.SecondItem == SecondItem) ||
			(Recipe.FirstItem == SecondItem && Recipe.SecondItem == FirstItem))
		{
			CraftRecipe = Recipe;

			return true;
		}
	}

	return false;
}
