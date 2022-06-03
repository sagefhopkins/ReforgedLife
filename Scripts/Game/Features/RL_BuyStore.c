class RL_BuyItem : ScriptedUserAction
{
	[Attribute("", UIWidgets.EditBox, desc: "Money")]
	private string m_ExchangeItemDisplayName;
	
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Prefab what item is being purchased ")]
	private ResourceName m_ItemExchangedFor;
	[Attribute("", UIWidgets.EditBox, desc: "Display name for prefab being purchased")]
	private string m_ItemName;
	
	[Attribute("", UIWidgets.EditBox, desc: "Price")]
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
			//Todo: Loop doesn't work properly. When an item is purchased, the function takes 
			//all money rather than a set amount of money. Additionally, the function sometimes
			//spawns more than one prefab draining the players money.
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
					inventoryManager.TrySpawnPrefabToStorage(m_ItemExchangedFor);
					PrintString("Removed " + sName + " from inventory");
					while (count < m_ItemPrice){
						inventoryManager.TryDeleteItem(item);
						PrintString("Removed " + sName + " from inventory");
						count++;
					}
				}

			}
		}
	}
	
	override bool GetActionNameScript(out string outName)
	{
		outName = string.Format("Buy %1 for $%2" , m_ItemName, m_ItemPrice);
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
 	{
		return true;
 	}
}