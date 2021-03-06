#ifndef EQUIPMENT_DATA_H
#define EQUIPMENT_DATA_H

#include "core/resource.h"

#include "../item_enums.h"
#include "equipment_data_entry.h"

class EquipmentData : public Resource {
	GDCLASS(EquipmentData, Resource);

public:
	Ref<EquipmentDataEntry> get_slot(int index);
	void set_slot(int index, Ref<EquipmentDataEntry> entry);

	Ref<ItemInstance> get_item(int index);

	EquipmentData();
	~EquipmentData();

protected:
	static void _bind_methods();

private:
	Ref<EquipmentDataEntry> _entries[ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX];
};

#endif
