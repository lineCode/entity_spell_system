#ifndef MOB_DATA_H
#define MOB_DATA_H

#include "core/resource.h"
#include "core/ustring.h"

#include "../data/character_class.h"
#include "../loot/loot_data_base.h"

//Add it to the data manager too.

class MobData : public Resource {
	GDCLASS(MobData, Resource);

public:
    int get_id() const;
    void set_id(const int id);
    
    Ref<LootDataBase> get_loot_db() const;
    void set_loot_db(const Ref<LootDataBase> lootdb);
    
    Ref<CharacterClass> get_character_class() const;
    void set_character_class(const Ref<CharacterClass> character_class);
    
    String get_player_name();
    
    String get_static_player_name() const;
    void set_static_player_name(const String name);
    
	MobData();
	~MobData();

protected:
	static void _bind_methods();

private:
    int _id;
    Ref<LootDataBase> _lootdb;
    Ref<CharacterClass> _character_class;
    String _static_player_name;
    //Skeleton -> basic gear
};


#endif