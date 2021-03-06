#pragma once
#include "World/Item.hpp"
#include <array>

enum class EquipmentSlot {
	Weapon = 0,
	Shield,
	Helmet,
	Gloves,
	Chest,
	Boots,
	Amulet,
	Ring,
	_DummyEnd,
	Pants,	//frozen
	Braces	//frozen
};

class PlayerEquipment {
	std::shared_ptr<Item> weapon;
	std::shared_ptr<Item> shield;
	std::shared_ptr<Item> helmet;
	std::shared_ptr<Item> chest;
	std::shared_ptr<Item> pants;
	std::shared_ptr<Item> boots;
	std::array<std::shared_ptr<Item>, 4> accessories;
public:
	bool setEquipment(EquipmentSlot slot, const std::shared_ptr<Item>& item) {
		if (!item) {
			switch (slot) {
			case EquipmentSlot::Weapon:
				weapon = nullptr;
				break;
			case EquipmentSlot::Shield:
				shield = nullptr;
				break;
			case EquipmentSlot::Helmet:
				helmet = nullptr;
				break;
			case EquipmentSlot::Chest:
				chest = nullptr;
				break;
			case EquipmentSlot::Pants:
				pants = nullptr;
				break;
			case EquipmentSlot::Boots:
				boots = nullptr;
				break;
			case EquipmentSlot::Ring:
				accessories[0] = nullptr;
				break;
			case EquipmentSlot::Amulet:
				accessories[1] = nullptr;
				break;
			case EquipmentSlot::Gloves:
				accessories[2] = nullptr;
				break;
			case EquipmentSlot::Braces:
				accessories[3] = nullptr;
				break;
			}
			return true;
		}
		
		switch(slot) {
			case EquipmentSlot::Weapon:
				if(!(item->getType() == ItemType::WeaponBow || item->getType() == ItemType::WeaponSword || item->getType() == ItemType::WeaponStaff))
					return false;
				weapon = item;
				break;
			case EquipmentSlot::Shield:
				if(!(item->getType() == ItemType::Shield) )
					return false;
				shield = item;
				break;
			case EquipmentSlot::Helmet:
				if(item->getType() != ItemType::ArmorHelmet)
					return false;
				helmet = item;
				break;
			case EquipmentSlot::Chest:
				if(item->getType() != ItemType::ArmorChest)
					return false;
				chest = item;
				break;
			case EquipmentSlot::Pants:
				if(item->getType() != ItemType::ArmorPants)
					return false;
				pants = item;
				break;
			case EquipmentSlot::Boots:
				if(item->getType() != ItemType::ArmorBoots)
					return false;
				boots = item;
				break;
			case EquipmentSlot::Ring:
				if(item->getType() != ItemType::EquipRing)
					return false;
				accessories[0] = item;
				break;
			case EquipmentSlot::Amulet:
				if(item->getType() != ItemType::EquipNecklace)
					return false;
				accessories[1] = item;
				break;
			case EquipmentSlot::Gloves:
				if(item->getType() != ItemType::EquipGloves)
					return false;
				accessories[2] = item;
				break;
			case EquipmentSlot::Braces:
				if(item->getType() != ItemType::EquipBraces)
					return false;
				accessories[3] = item;
				break;
			default: return false;
		}

		return true;
	}

	std::shared_ptr<Item> getEquipmentBySlot(EquipmentSlot slot) {
		switch(slot) {
			case EquipmentSlot::Weapon:
				return weapon;
			case EquipmentSlot::Shield:
				return shield;
			case EquipmentSlot::Helmet:
				return helmet;
			case EquipmentSlot::Chest:
				return chest;
			case EquipmentSlot::Pants:
				return pants;
			case EquipmentSlot::Boots:
				return boots;
			case EquipmentSlot::Ring:
				return accessories[0];
			case EquipmentSlot::Amulet:
				return accessories[1];
			case EquipmentSlot::Gloves:
				return accessories[2];
			case EquipmentSlot::Braces:
				return accessories[3];
		}

		return nullptr;
	}

	void clear() {
		weapon = nullptr;
		shield = nullptr;
		helmet = nullptr;
		chest = nullptr;
		pants = nullptr;
		boots = nullptr;
		for(unsigned i = 0; i < accessories.size(); ++i)
			accessories[i] = nullptr;
	}
};