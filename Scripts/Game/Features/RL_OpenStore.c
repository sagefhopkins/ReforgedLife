class RL_SellItem : ScriptedUserAction
{
	[Attribute("", UIWidgets.EditBox, desc: "Display Name of Item Being Purchased")]
	private string m_ExchangeItemDisplayName;
	
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Prefab what item is exchanged ")]
	private ResourceName m_ItemExchangedFor;
	
	[Attribute("", UIWidgets.EditBox, desc: "Sale Price")]
	private int m_ItemPrice;
		
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		array<IEntity> items = new array<IEntity>();
		
		// Play sound
		auto replication = RplComponent.Cast(pOwnerEntity.FindComponent(RplComponent));
		
		//Todo: Replace SCR_InventoryStorageManagerComponent RPL setup with own function or better synced sound alternative
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		inventoryManager.GetItems(items);
		foreach (IEntity item : items)
		{
			InventoryItemComponent pInvComp = InventoryItemComponent.Cast(  item .FindComponent( InventoryItemComponent ) );
			if( pInvComp )
			{
				ItemAttributeCollection attribs = pInvComp.GetAttributes();
				if( !attribs )
					break;
				string sName = attribs.GetUIInfo().GetName();
				PrintString("------------");
				PrintString(m_ExchangeItemDisplayName);
				PrintString(sName);
				PrintString("------------");
				if (sName == m_ExchangeItemDisplayName){
					int count = 0;
					inventoryManager.TryDeleteItem(item);
					PrintString("Removed " + sName + " from inventory");
					while (count < m_ItemPrice){
						inventoryManager.TrySpawnPrefabToStorage(m_ItemExchangedFor);
						PrintString("Added " + m_ItemExchangedFor + " to inventory");
						count++;
					}
				}

			}
		}
	}
	
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Sell %1 for $%2" , m_ExchangeItemDisplayName, m_ItemPrice);
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
 	{
		return true;
 	}
	
}