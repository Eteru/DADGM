
#include "Inventory.h"
#include "PrintUtils.h"
// 
// Inventory::Inventory(size_t size)
// 	: m_capacity(size), m_used_capacity(0)
// {
// }
// 
// Inventory::~Inventory()
// {
// }
// 
// void Inventory::Resize(size_t size)
// {
// 	if (size <= m_used_capacity)
// 	{
// 		PrintUtils::PrintI("Inventory: Resize to a smaller capacity than used");
// 		return;
// 	}
// 
// 	m_capacity = size;
// }
// 
// void Inventory::AddItem(Item * item)
// {
// 	if (m_used_capacity + item->GetOccupiedSpace() > m_capacity)
// 	{
// 		PrintUtils::PrintI("Inventory: item can't be added to inventory due to capacity");
// 		return;
// 	}
// 
// 	if (m_items.find(item->GetName()) == m_items.end())
// 	{
// 		m_items[item->GetName()] = item;
// 	}
// }
// 
// void Inventory::RemoveItem(Item * item)
// {
// 	m_items.erase(item->GetName());
// }
