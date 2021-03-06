#ifndef ENTITY_H
#define ENTITY_H

#include "core/io/networked_multiplayer_peer.h"

#include "scene/main/node.h"

#include "../data/craft_recipe.h"
#include "../data/item_instance.h"
#include "../data/spell.h"
#include "../data/xp_data.h"
#include "./data/entity_data.h"
#include "core/hash_map.h"
#include "core/object.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "../data/spell.h"
#include "./resources/entity_resource.h"
#include "stats/stat.h"

#include "../autoloads/entity_data_manager.h"
#include "../entity_enums.h"
#include "../utility/entity_create_info.h"

#include "../inventory/bag.h"
#include "../utility/category_cooldown.h"
#include "../utility/cooldown.h"
#include "./data/entity_data_container.h"

#include "../profile_manager/actionbar/action_bar_profile.h"

#include "./ai/entity_ai.h"

#include "../data/aura_group.h"

#include "../profile_manager/input/input_profile.h"
#include "../profile_manager/profile_manager.h"

class EntityData;
class AuraData;
class Spell;
class SpellDamageInfo;
class SpellHealInfo;
class SpellCastInfo;
class EntityCreateInfo;
class TalentRowData;
class CharacterSpec;
class EntitySkill;

enum SpellCastDataSignals {
	CastFailed,
	StartCasting,
	CastFinished,
	SetTarget
};

VARIANT_ENUM_CAST(SpellCastDataSignals);

enum PlayerStates {
	STATE_FREEZE,
	STATE_NORMAL
};

VARIANT_ENUM_CAST(PlayerStates);

enum PlayerSendFlags {
	SEND_FLAG_NONE,
	SEND_FLAG_INVENTORY,
	SEND_FLAG_SPELL_DATA,
	SEND_FLAG_AURAS,
};

#define ISSERVER() (is_inside_tree() && (!get_tree()->has_network_peer() || (get_tree()->has_network_peer() && get_tree()->is_network_server())))
#define ISCLIENT() (is_inside_tree() && get_tree()->has_network_peer() && !get_tree()->is_network_server())

#define SET_RPC_OFF(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_DISABLED);
#define SET_RPC_REMOTE(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_REMOTE);
#define SET_RPC_MASTER(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_MASTER);
#define SET_RPC_PUPPET(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_PUPPET);
#define SET_RPC_REMOTESYNC(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_REMOTESYNC);
#define SET_RPC_MASTERSYNC(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_MASTERSYNC);
#define SET_RPC_PUPPETSYNC(p_method_name) rpc_config(p_method_name, MultiplayerAPI::RPC_MODE_PUPPETSYNC);

// f.e.   RPC(method, arg0, arg1, etc)
#define RPC(func, ...)                                        \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		rpc(#func, ##__VA_ARGS__);                            \
	}                                                         \
	func(__VA_ARGS__);

#define VRPC(func, ...)                                       \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		vrpc(#func, ##__VA_ARGS__);                           \
	}                                                         \
	func(__VA_ARGS__);

#define ORPC(func, ...)                                                   \
	if (is_inside_tree() && get_tree()->has_network_peer()) {             \
		if (get_tree()->is_network_server() && get_network_master() != 1) \
			rpc_id(get_network_master(), #func, ##__VA_ARGS__);           \
	}                                                                     \
	func(__VA_ARGS__);

#define RPCS(func, ...)                                       \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		if (get_tree()->is_network_server()) {                \
			func(__VA_ARGS__);                                \
		} else {                                              \
			rpc_id(1, #func, ##__VA_ARGS__);                  \
		}                                                     \
	} else {                                                  \
		func(__VA_ARGS__);                                    \
	}

//RPC Objects

#define RPCOBJ(rpcfunc, rpc_var, normalfunc, normal_var)      \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		rpc(#rpcfunc, rpc_var);                               \
	}                                                         \
	normalfunc(normal_var);

#define VRPCOBJ(rpcfunc, rpc_var, normalfunc, normal_var)     \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		vrpc(#rpcfunc, rpc_var);                              \
	}                                                         \
	normalfunc(normal_var);

#define VRPCOBJP(rpcfunc, rpc_var1, rpc_var2, normalfunc, normal_var1, normal_var2)  \
	if (is_inside_tree() && get_tree()->has_network_peer()) {                        \
		vrpc(#rpcfunc, rpc_var1, rpc_var2);                                          \
	}                                                                                \
	normalfunc(normal_var1, normal_var2);

#define ORPCOBJ(rpcfunc, rpc_var, normalfunc, normal_var)                 \
	if (is_inside_tree() && get_tree()->has_network_peer()) {             \
		if (get_tree()->is_network_server() && get_network_master() != 1) \
			rpc_id(get_network_master(), #rpcfunc, rpc_var);              \
	}                                                                     \
	normalfunc(normal_var);

#define RPCSOBJ(rpcfunc, rpc_var, normalfunc, normal_var)     \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		if (get_tree()->is_network_server()) {                \
			normalfunc(normal_var);                           \
		} else {                                              \
			rpc_id(1, #rpcfunc, rpc_var);                     \
		}                                                     \
	} else {                                                  \
		normalfunc(normal_var);                               \
	}

#define ORPCOBJP(rpcfunc, rpc_var1, rpc_var2, normalfunc, normal_var1, normal_var2) \
	if (is_inside_tree() && get_tree()->has_network_peer()) {                       \
		if (get_tree()->is_network_server() && get_network_master() != 1)           \
			rpc_id(get_network_master(), #rpcfunc, rpc_var1, rpc_var2);             \
	}                                                                               \
	normalfunc(normal_var1, normal_var2);

// f.e. RSET(rset("property", "value"), property, value)
#define RSET(rset_func, variable, value)                      \
	if (is_inside_tree() && get_tree()->has_network_peer()) { \
		rset_func;                                            \
	}                                                         \
	variable = value;

class Entity : public Node {
	GDCLASS(Entity, Node);

public:
	void initialize(Ref<EntityCreateInfo> info);

	////    Base    ////

	NodePath get_body_path();
	void set_body_path(NodePath value);
	Node *get_body();

	NodePath get_character_skeleton_path();
	void set_character_skeleton_path(NodePath value);
	Node *get_character_skeleton();

	//EntityType
	EntityEnums::EntityType gets_entity_type();
	void sets_entity_type(EntityEnums::EntityType value);

	EntityEnums::EntityType getc_entity_type();
	void setc_entity_type(EntityEnums::EntityType value);

	//EntityInteractionType
	EntityEnums::EntityInteractionType gets_entity_interaction_type();
	void sets_entity_interaction_type(EntityEnums::EntityInteractionType value);

	EntityEnums::EntityInteractionType getc_entity_interaction_type();
	void setc_entity_interaction_type(EntityEnums::EntityInteractionType value);

	int gets_immunity_flags();
	void sets_immunity_flags(int value);

	int gets_entity_flags();
	void sets_entity_flags(int value);

	int getc_entity_flags();
	void setc_entity_flags(int value);

	String gets_entity_name();
	void sets_entity_name(String value);

	String getc_entity_name();
	void setc_entity_name(String value);

	EntityEnums::EntityGender gets_gender();
	void sets_gender(EntityEnums::EntityGender value);

	EntityEnums::EntityGender getc_gender();
	void setc_gender(EntityEnums::EntityGender value);

	int gets_level();
	void sets_level(int value);

	int getc_level();
	void setc_level(int value);

	int gets_xp();
	void sets_xp(int value);

	int getc_xp();
	void setc_xp(int value);

	int gets_money();
	void sets_money(int value);

	int getc_money();
	void setc_money(int value);

	Ref<EntityData> gets_entity_data();
	void sets_entity_data(Ref<EntityData> value);

	Ref<EntityData> getc_entity_data();
	void setc_entity_data(Ref<EntityData> value);

	int gets_entity_data_id();
	void sets_entity_data_id(int value);

	int getc_entity_data_id();
	void setc_entity_data_id(int value);

	EntityEnums::AIStates gets_ai_state() const;
	void sets_ai_state(EntityEnums::AIStates state);

	EntityEnums::AIStates gets_ai_state_stored() const;
	void sets_ai_state_stored(EntityEnums::AIStates state);

	int gets_seed();
	void sets_seed(int value);

	int getc_seed();
	void setc_seed(int value);

	////     Stats    ////

	_FORCE_INLINE_ Ref<Stat> get_health() { return _stats[Stat::STAT_ID_HEALTH]; }
	_FORCE_INLINE_ Ref<Stat> get_mana() { return _stats[Stat::STAT_ID_MANA]; }
	_FORCE_INLINE_ Ref<Stat> get_energy() { return _stats[Stat::STAT_ID_RAGE]; }
	_FORCE_INLINE_ Ref<Stat> get_rage() { return _stats[Stat::STAT_ID_ENERGY]; }
	_FORCE_INLINE_ Ref<Stat> get_speed() { return _stats[Stat::STAT_ID_SPEED]; }
	_FORCE_INLINE_ Ref<Stat> get_gcd() { return _stats[Stat::STAT_ID_GLOBAL_COOLDOWN]; }

	_FORCE_INLINE_ Ref<Stat> get_melee_crit() { return _stats[Stat::STAT_ID_MELEE_CRIT]; }
	_FORCE_INLINE_ Ref<Stat> get_melee_crit_bonus() { return _stats[Stat::STAT_ID_MELEE_CRIT_BONUS]; }
	_FORCE_INLINE_ Ref<Stat> get_spell_crit() { return _stats[Stat::STAT_ID_SPELL_CRIT]; }
	_FORCE_INLINE_ Ref<Stat> get_spell_crit_bonus() { return _stats[Stat::STAT_ID_SPELL_CRIT_BONUS]; }

	_FORCE_INLINE_ Ref<Stat> get_block() { return _stats[Stat::STAT_ID_BLOCK]; }
	_FORCE_INLINE_ Ref<Stat> get_parry() { return _stats[Stat::STAT_ID_PARRY]; }
	_FORCE_INLINE_ Ref<Stat> get_damage_reduction() { return _stats[Stat::STAT_ID_DAMAGE_REDUCTION]; }
	_FORCE_INLINE_ Ref<Stat> get_melee_damage_reduction() { return _stats[Stat::STAT_ID_MELEE_DAMAGE_REDUCTION]; }
	_FORCE_INLINE_ Ref<Stat> get_spell_damage_reduction() { return _stats[Stat::STAT_ID_SPELL_DAMAGE_REDUCTION]; }
	_FORCE_INLINE_ Ref<Stat> get_damage_taken() { return _stats[Stat::STAT_ID_DAMAGE_TAKEN]; }
	_FORCE_INLINE_ Ref<Stat> get_heal_taken() { return _stats[Stat::STAT_ID_HEAL_TAKEN]; }
	_FORCE_INLINE_ Ref<Stat> get_melee_damage() { return _stats[Stat::STAT_ID_MELEE_DAMAGE]; }
	_FORCE_INLINE_ Ref<Stat> get_spell_damage() { return _stats[Stat::STAT_ID_SPELL_DAMAGE]; }

	Ref<Stat> get_stat_int(int index);
	void set_stat_int(int index, Ref<Stat> entry);

	Ref<Stat> get_stat_enum(Stat::StatId stat_id);
	void set_stat_enum(Stat::StatId stat_id, Ref<Stat> entry);

	void sdie();
	void cdie();

	void ons_stat_changed(Ref<Stat> stat);
	void onc_stat_changed(Ref<Stat> stat);

	void ssend_stat(int id, int ccurrent, int cmax);
	void creceive_stat(int id, int ccurrent, int cmax);

	////    Equip Slots    ////

	bool should_deny_equip(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item);

	void son_equip_success(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot);
	void son_equip_fail(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot);

	void con_equip_success(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot);
	void con_equip_fail(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot);

	void crequest_equip(ItemEnums::EquipSlots equip_slot, int bag_slot);
	void sequip(ItemEnums::EquipSlots equip_slot, int bag_slot);
	void _sequip(ItemEnums::EquipSlots equip_slot, int bag_slot);

	void cequip_success(ItemEnums::EquipSlots equip_slot, int bag_slot);
	void cequip_fail(ItemEnums::EquipSlots equip_slot, int bag_slot);

	Ref<ItemInstance> gets_equip_slot(int index);
	void sets_equip_slot(int index, Ref<ItemInstance> item);

	Ref<ItemInstance> getc_equip_slot(int index);
	void setc_equip_slot(int index, Ref<ItemInstance> item);

	bool can_equip_item(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item);
	bool _can_equip_item(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item);

	void sapply_item(Ref<ItemInstance> item);
	void sdeapply_item(Ref<ItemInstance> item);

	void _sapply_item(Ref<ItemInstance> item);
	void _sdeapply_item(Ref<ItemInstance> item);

	void capply_item(Ref<ItemInstance> item);
	void cdeapply_item(Ref<ItemInstance> item);

	void _capply_item(Ref<ItemInstance> item);
	void _cdeapply_item(Ref<ItemInstance> item);

	////    Resources    ////

	Ref<EntityResource> gets_resource_index(int index);
	Ref<EntityResource> gets_resource_id(int id);
	void adds_resource(Ref<EntityResource> resource);
	int gets_resource_count();
	void removes_resource(int index);
	void clears_resource();

	void addc_resource_rpc(int index, String data);

	Ref<EntityResource> getc_resource_index(int index);
	Ref<EntityResource> getc_resource_id(int id);
	void addc_resource(int index, Ref<EntityResource> resource);
	int getc_resource_count();
	void removec_resource(int index);
	void clearc_resource();

	void sends_resource_current(int index, int current);
	void sends_resource_curr_max(int index, int current, int max);
	void sends_resource_data(int index, String data);

	void creceive_resource_current(int index, int current);
	void creceive_resource_curr_max(int index, int current, int max);
	void creceive_resource_data(int index, String data);

	////    Global Cooldown    ////

	bool getc_has_global_cooldown();
	bool gets_has_global_cooldown();
	bool getc_global_cooldown();
	bool gets_global_cooldown();
	void sstart_global_cooldown(float value);
	void cstart_global_cooldown(float value);

	////    States    ////
	int getc_state();
	void setc_state(int state);

	int gets_state();
	void sets_state(int state);

	void sadd_state_ref(int state_index);
	void sremove_state_ref(int state_index);

	////    Crafting System    ////

	void crequest_craft(int id);
	void scraft(int id);

	bool hass_craft_recipe(Ref<CraftRecipe> craft_recipe);
	bool hass_craft_recipe_id(int id);
	void adds_craft_recipe(Ref<CraftRecipe> craft_recipe);
	void adds_craft_recipe_id(int id);
	void removes_craft_recipe(Ref<CraftRecipe> craft_recipe);
	void removes_craft_recipe_id(int id);
	Ref<CraftRecipe> gets_craft_recipe(int index);
	Ref<CraftRecipe> gets_craft_recipe_id(int id);
	int gets_craft_recipe_count();

	bool hasc_craft_recipe(Ref<CraftRecipe> craft_recipe);
	bool hasc_craft_recipe_id(int id);
	void addc_craft_recipe(Ref<CraftRecipe> craft_recipe);
	void addc_craft_recipe_id(int id);
	void removec_craft_recipe(Ref<CraftRecipe> craft_recipe);
	void removec_craft_recipe_id(int id);
	Ref<CraftRecipe> getc_craft_recipe(int index);
	int getc_craft_recipe_count();

	////    SpellSystem    ////

	//EventHandlers
	void son_before_aura_applied(Ref<AuraData> data);
	void son_after_aura_applied(Ref<AuraData> data);

	void son_before_damage_hit(Ref<SpellDamageInfo> info);
	void son_hit(Ref<SpellDamageInfo> info);
	void son_before_damage(Ref<SpellDamageInfo> info);
	void son_damage_receive(Ref<SpellDamageInfo> info);
	void son_dealt_damage(Ref<SpellDamageInfo> info);
	void son_damage_dealt(Ref<SpellDamageInfo> info);

	void son_before_heal_hit(Ref<SpellHealInfo> info);
	void son_before_heal(Ref<SpellHealInfo> info);
	void son_heal_receive(Ref<SpellHealInfo> info);
	void son_dealt_heal(Ref<SpellHealInfo> info);
	void son_heal_dealt(Ref<SpellHealInfo> info);

	void son_before_cast(Ref<SpellCastInfo> info);
	void son_before_cast_target(Ref<SpellCastInfo> info);
	void son_cast_finished_target(Ref<SpellCastInfo> info);
	void son_cast_finished(Ref<SpellCastInfo> info);
	void son_cast_started(Ref<SpellCastInfo> info);
	void son_cast_failed(Ref<SpellCastInfo> info);
	void son_spell_cast_success(Ref<SpellCastInfo> info);

	void son_death();

	void son_cooldown_added(Ref<Cooldown> cooldown);
	void son_cooldown_removed(Ref<Cooldown> cooldown);

	void son_category_cooldown_added(Ref<CategoryCooldown> category_cooldown);
	void son_category_cooldown_removed(Ref<CategoryCooldown> category_cooldown);

	void son_gcd_started();
	void son_gcd_finished();
	void con_gcd_started();
	void con_gcd_finished();

	void son_physics_process(float delta);

	void son_xp_gained(int value);
	void son_level_up(int value);

	//Clientside EventHandlers
	void con_cast_failed(Ref<SpellCastInfo> info);
	void con_cast_started(Ref<SpellCastInfo> info);
	void con_cast_state_changed(Ref<SpellCastInfo> info);
	void con_cast_finished(Ref<SpellCastInfo> info);
	void con_spell_cast_success(Ref<SpellCastInfo> info);

	void con_death();

	void con_cooldown_added(Ref<Cooldown> cooldown);
	void con_cooldown_removed(Ref<Cooldown> cooldown);
	void con_category_cooldown_added(Ref<CategoryCooldown> category_cooldown);
	void con_category_cooldown_removed(Ref<CategoryCooldown> category_cooldown);

	void con_aura_added(Ref<AuraData> data);
	void con_aura_removed(Ref<AuraData> data);
	void con_aura_refresh(Ref<AuraData> data);

	void con_damage_dealt(Ref<SpellDamageInfo> info);
	void con_dealt_damage(Ref<SpellDamageInfo> info);
	void con_heal_dealt(Ref<SpellHealInfo> info);
	void con_dealt_heal(Ref<SpellHealInfo> info);

	void con_xp_gained(int value);
	void con_level_up(int value);

	//Modifiers/Requesters
	void sapply_passives_damage_receive(Ref<SpellDamageInfo> info);
	void sapply_passives_damage_deal(Ref<SpellDamageInfo> info);

	void sapply_passives_heal_receive(Ref<SpellHealInfo> info);
	void sapply_passives_heal_deal(Ref<SpellHealInfo> info);

	//Spell operations
	void scast_spell(int spell_id);
	void crequest_spell_cast(int spell_id);

	//Damage Operations
	void stake_damage(Ref<SpellDamageInfo> info);
	void sdeal_damage_to(Ref<SpellDamageInfo> info);

	//Heal Operations
	void stake_heal(Ref<SpellHealInfo> info);
	void sdeal_heal_to(Ref<SpellHealInfo> info);

	//Damage, Heal RPCs
	void cdamage_dealt_rpc(String data);
	void cdealt_damage_rpc(String data);
	void cheal_dealt_rpc(String data);
	void cdealt_heal_rpc(String data);

	//Interactions
	bool cans_interact();
	void sinteract();

	bool canc_interact();
	void crequest_interact();

	void ssend_open_window(int window_id);
	void copen_window(int window_id);

	//XP Operations
	void adds_xp(int value);
	void addc_xp(int value);
	void slevelup(int value);
	void clevelup(int value);

	//Aura Manipulation
	void sadd_aura(Ref<AuraData> aura);
	void sremove_aura(Ref<AuraData> aura);
	void sremove_aura_exact(Ref<AuraData> aura);
	void sremove_aura_expired(Ref<AuraData> aura);
	void sremove_aura_dispelled(Ref<AuraData> aura);
	void saura_refreshed(Ref<AuraData> aura);

	void cadd_aura_rpc(String data);
	void cremove_aura_rpc(String data);
	void cremove_aura_exact_rpc(String data);
	void cremove_aura_expired_rpc(String data);
	void cremove_aura_dispelled_rpc(String data);
	void caura_refreshed_rpc(String data);

	void cadd_aura(Ref<AuraData> aura);
	void cremove_aura(Ref<AuraData> aura);
	void cremove_aura_exact(Ref<AuraData> aura);
	void cremove_aura_expired(Ref<AuraData> aura);
	void cremove_aura_dispelled(Ref<AuraData> aura);
	void caura_refreshed(Ref<AuraData> aura);

	void sremove_auras_with_group(Ref<AuraGroup> aura_group);

	//NOTE: No reason for shas_aura_by, just query it, and check for null.
	int sget_aura_count();
	Ref<AuraData> sget_aura(int index);
	Ref<AuraData> sget_aura_by(Entity *caster, int aura_id);
	Ref<AuraData> sget_aura_by_bind(Node *caster, int aura_id);

	Ref<AuraData> sget_aura_with_group_by(Entity *caster, Ref<AuraGroup> aura_group);
	Ref<AuraData> sget_aura_with_group_by_bind(Node *caster, Ref<AuraGroup> aura_group);

	int cget_aura_count();
	Ref<AuraData> cget_aura(int index);

	//Hooks
	void moved();

	void onc_mouse_enter();
	void onc_mouse_exit();

	void onc_targeted();
	void onc_untargeted();

	//Update
	void update_auras(float delta);

	bool gets_is_dead();
	bool getc_is_dead();

	////    Casting System    ////

	bool sis_casting();
	bool cis_casting();

	Ref<SpellCastInfo> gets_spell_cast_info();
	void sets_spell_cast_info(Ref<SpellCastInfo> info);

	Ref<SpellCastInfo> getc_spell_cast_info();
	void setc_spell_cast_info(Ref<SpellCastInfo> info);

	void sstart_casting(Ref<SpellCastInfo> info);
	void sfail_cast();
	void sdelay_cast();
	void sfinish_cast();
	void sinterrupt_cast();

	void cstart_casting_rpc(String data);
	void cstart_casting(Ref<SpellCastInfo> info);
	void cfail_cast();
	void cdelay_cast();
	void cfinish_cast();
	void cinterrupt_cast();

	void sspell_cast_success(Ref<SpellCastInfo> info);

	void cspell_cast_success_rpc(String data);
	void cspell_cast_success(Ref<SpellCastInfo> info);

	////    Cooldowns    ////

	Vector<Ref<Cooldown> > *gets_cooldowns();
	Vector<Ref<Cooldown> > *getc_cooldowns();

	HashMap<int, Ref<Cooldown> > *gets_cooldown_map();
	HashMap<int, Ref<Cooldown> > *getc_cooldown_map();

	bool hass_cooldown(int spell_id);
	void adds_cooldown(int spell_id, float value);
	void removes_cooldown(int spell_id);
	Ref<Cooldown> gets_cooldown(int spell_id);
	Ref<Cooldown> gets_cooldown_index(int index);
	int gets_cooldown_count();

	bool hasc_cooldown(int spell_id);
	void addc_cooldown(int spell_id, float value);
	void removec_cooldown(int spell_id);
	Ref<Cooldown> getc_cooldown(int spell_id);
	Ref<Cooldown> getc_cooldown_index(int index);
	int getc_cooldown_count();

	//Category Cooldowns

	Vector<Ref<CategoryCooldown> > gets_category_cooldowns();
	Vector<Ref<CategoryCooldown> > getc_category_cooldowns();

	bool hass_category_cooldown(int category_id);
	void adds_category_cooldown(int category_id, float value);
	void removes_category_cooldown(int category_id);
	Ref<CategoryCooldown> gets_category_cooldown(int category_id);
	Ref<CategoryCooldown> gets_category_cooldown_index(int index);
	int gets_category_cooldown_count();

	bool hasc_category_cooldown(int category_id);
	void addc_category_cooldown(int category_id, float value);
	void removec_category_cooldown(int spell_id);
	Ref<CategoryCooldown> getc_category_cooldown(int category_id);
	Ref<CategoryCooldown> getc_category_cooldown_index(int index);
	int getc_category_cooldown_count();

	//Known Spells

	int gets_free_spell_points();
	void sets_free_spell_points(int value);

	int getc_free_spell_points();
	void setc_free_spell_points(int value);

	void crequest_spell_learn(int id);
	void slearn_spell(int id);

	bool hass_spell(Ref<Spell> spell);
	bool hass_spell_id(int id);
	void adds_spell(Ref<Spell> spell);
	void removes_spell(Ref<Spell> spell);
	Ref<Spell> gets_spell(int index);
	int gets_spell_count();

	bool hasc_spell(Ref<Spell> spell);
	bool hasc_spell_id(int id);
	void addc_spell(Ref<Spell> spell);
	void removec_spell(Ref<Spell> spell);
	Ref<Spell> getc_spell(int index);
	int getc_spell_count();

	void addc_spell_rpc(int id);
	void removec_spell_rpc(int id);

	//Skills

	bool hass_skill_id(int id);
	bool hass_skill(Ref<EntitySkill> skill);
	void adds_skill(Ref<EntitySkill> skill);
	void removes_skill(Ref<EntitySkill> skill);
	Ref<EntitySkill> gets_skill(int skill);
	int gets_skill_count();

	bool hasc_skill_id(int id);
	bool hasc_skill(Ref<EntitySkill> skill);
	void addc_skill(Ref<EntitySkill> skill);
	void removec_skill(Ref<EntitySkill> skill);
	Ref<EntitySkill> getc_skill(int index);
	int getc_skill_count();

	void sskill_current_changed(Ref<EntitySkill> skill);
	void sskill_max_changed(Ref<EntitySkill> skill);

	void addc_skill_id(int skill_id, int value, int max_value);
	void removec_skill_id(int skill_id);
	void changec_skill(int skill_id, int value);
	void changec_skill_max(int skill_id, int value);

	////    Target    ////

	void crequest_target_change(NodePath path);
	void net_sets_target(NodePath path);
	void net_setc_target(NodePath path);

	Entity *gets_target();
	void sets_target(Node *p_target);

	Entity *getc_target();
	void setc_target(Node *p_target);

	////    Talents    ////

	int gets_free_talent_points();
	void sets_free_talent_points(int value);

	int getc_free_talent_points();
	void setc_free_talent_points(int value);

	void crequest_talent_learn(int spec_index, int talent_row, int talent_culomn);
	void sreceive_talent_learn_request(int spec_index, int talent_row, int talent_culomn);
	void _sreceive_talent_learn_request(int spec_index, int talent_row, int talent_culomn);

	void crequest_talent_reset();
	void sreceive_reset_talent_request();
	void _sreceive_reset_talent_request();

	void sreset_talents();
	void creset_talents();

	void adds_talent(int talent);
	void removes_talent(int talent);
	bool hass_talent(int talent);
	int gets_talent(int index);
	int gets_talent_count();
	void sclear_talents();

	void addc_talent(int talent);
	void removec_talent(int talent);
	bool hasc_talent(int talent);
	int getc_talent(int index);
	int getc_talent_count();
	void cclear_talents();

	////    Inventory    ////

	Ref<Bag> gets_bag() const;
	void sets_bag(const Ref<Bag> bag);

	Ref<Bag> getc_bag() const;
	void setc_bag(const Ref<Bag> bag);

	Ref<Bag> gets_target_bag() const;
	void sets_target_bag(const Ref<Bag> bag);

	Ref<Bag> getc_target_bag() const;
	void setc_target_bag(const Ref<Bag> bag);

	void setc_bag_rpc(String data);
	void setc_target_bag_rpc(String data);

	void crequest_loot(int index);
	void sloot(int index);
	void cloot(int index);

	void ons_item_added(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void cadd_item_rpc(int slot_id, String item_data);
	void cadd_item(int slot_id, Ref<ItemInstance> item);

	//Bag
	void ons_item_removed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void sremove_item(const int slot_id);
	void cremove_item(const int slot_id);
	void cdenyremove_item(const int slot_id);
	void crequest_remove_item(const int slot_id);

	void ons_items_swapped(Ref<Bag> bag, int slot_id_1, int slot_id_2);
	void sswap_items(int slot_id_1, int slot_id_2);
	void cswap_items(int slot_id_1, int slot_id_2);
	void cdeny_item_swap(int slot_id_1, int slot_id_2);
	void crequest_item_swap(int slot_id_1, int slot_id_2);

	void ons_item_count_changed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void cchange_item_count(int slot_id, int new_count);

	void ons_overburdened(Ref<Bag> bag);
	void ons_overburden_removed(Ref<Bag> bag);

	//Target Bag
	void ons_target_item_added(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void cadd_target_item_rpc(int slot_id, String item_data);
	void cadd_target_item(int slot_id, Ref<ItemInstance> item);

	void ons_target_item_removed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void sremove_target_item(const int slot_id);
	void cremove_target_item(const int slot_id);
	void cdenyremove_target_item(const int slot_id);
	void crequest_target_remove_item(const int slot_id);

	void ons_target_items_swapped(Ref<Bag> bag, int slot_id_1, int slot_id_2);
	void sswap_target_items(int slot_id_1, int slot_id_2);
	void cswap_target_items(int slot_id_1, int slot_id_2);
	void cdeny_target_item_swap(int slot_id_1, int slot_id_2);
	void crequest_target_item_swap(int slot_id_1, int slot_id_2);

	void ons_target_item_count_changed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id);
	void cchange_target_item_count(int slot_id, int new_count);

	////    Data    ////

	void adds_data(Ref<EntityDataContainer> data);
	void removes_data(int index);
	Ref<EntityDataContainer> gets_data(int index);
	int gets_data_count();

	void addc_data(Ref<EntityDataContainer> data);
	void removec_data(int index);
	Ref<EntityDataContainer> getc_data(int index);
	int getc_data_count();

	////    Actionbars    ////

	bool get_actionbar_locked();
	void set_actionbar_locked(bool value);

	Ref<ActionBarProfile> get_action_bar_profile();

	void loaded();

	String random_name();

	void setup();
	virtual void _setup();
	void setup_actionbars();

	////    AI    ////

	bool gets_is_pet();
	bool getc_is_pet();

	Entity *gets_pet_owner();
	void sets_pet_owner(Entity *entity);
	void sets_pet_owner_bind(Node *entity);

	int gets_pet_formation_index();
	void sets_pet_formation_index(int value);

	EntityEnums::AIStates gets_pet_ai_state();
	void sets_pet_ai_state(EntityEnums::AIStates value);

	EntityEnums::EntityController gets_original_entity_controller();
	void sets_original_entity_controller(EntityEnums::EntityController value);

	EntityEnums::EntityController gets_entity_controller();
	void sets_entity_controller(EntityEnums::EntityController value);

	Ref<EntityAI> gets_ai();
	void sets_ai(Ref<EntityAI> value);

	////    Pets    ////

	void adds_pet(Entity *entity);
	void adds_pet_bind(Node *entity);
	Entity *gets_pet(int index);
	void removes_pet_index(int index);
	void removes_pet(Entity *entity);
	void removes_pet_bind(Node *entity);
	int gets_pet_count();

	void addc_pet_path(NodePath path);

	void addc_pet(Entity *entity);
	void addc_pet_bind(Node *entity);
	Entity *getc_pet(int index);
	void removec_pet_index(int index);
	void removec_pet(Entity *entity);
	void removec_pet_bind(Node *entity);
	int getc_pet_count();

	//void pets_attack();
	//void pets_follow();
	//void pets_stop();

	////    Serialization    ////

	bool is_deserialized();

	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	virtual Dictionary _to_dict();
	virtual void _from_dict(const Dictionary &dict);

	// Networking
	Entity *gets_sees(int index);
	void removes_sees_index(int index);
	void removes_sees(Entity *entity);
	void removes_sees_bind(Node *entity);
	void adds_sees(Entity *entity);
	void adds_sees_bind(Node *entity);
	int gets_sees_count();

	Entity *gets_seen_by(int index);
	void removes_seen_by_index(int index);
	void removes_seen_by(Entity *entity);
	void removes_seen_by_bind(Node *entity);
	void adds_seen_by(Entity *entity);
	void adds_seen_by_bind(Node *entity);
	int gets_seen_by_count();

	void vrpc(const StringName &p_method, VARIANT_ARG_LIST);
	Variant _vrpc_bind(const Variant **p_args, int p_argcount, Variant::CallError &r_error);

	Dictionary data_as_dict(String &data);

	void register_for_physics_process(Ref<SpellCastInfo> info);

	bool get_maunal_process() const;
	void set_maunal_process(bool value);

	void update(float delta);

	Entity();
	~Entity();

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);

private:
	bool _maunal_process;

	const float SAVE_BASE_SECONDS = 10.0;
	bool _deserialized;

	////    Paths    ////

	NodePath _body_path;
	Node *_body;

	NodePath _character_skeleton_path;
	Node *_character_skeleton;

	////    PlayerData    ////

	int _s_guid;
	int _c_guid;

	int _s_class_id;
	int _c_class_id;

	int _s_type;
	int _c_type;

	EntityEnums::EntityGender _s_gender;
	EntityEnums::EntityGender _c_gender;

	int _s_level;
	int _c_level;

	int _s_xp;
	int _c_xp;

	int _s_money;
	int _c_money;

	Ref<EntityData> _s_entity_data;
	Ref<EntityData> _c_entity_data;

	int _s_send_flag;

	String _s_entity_name;
	String _c_entity_name;

	EntityEnums::EntityInteractionType _s_interaction_type;
	EntityEnums::EntityInteractionType _c_interaction_type;

	int _s_is_dead;
	int _c_is_dead;

	int _s_seed;
	int _c_seed;

	////     Stats    ////

	Ref<Stat> _stats[Stat::STAT_ID_TOTAL_STATS];

	////    Equipment    ////

	Ref<ItemInstance> _s_equipment[ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX];
	Ref<ItemInstance> _c_equipment[ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX];

	////    Resources    ////

	Vector<Ref<EntityResource> > _s_resources;
	Vector<Ref<EntityResource> > _c_resources;

	////    GCD    ////

	float _s_gcd;
	float _c_gcd;

	////    States    ////

	int _s_states[EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX];

	int _s_state;
	int _c_state;

	////    SpellCastData    ////

	Ref<SpellCastInfo> _s_spell_cast_info;
	Ref<SpellCastInfo> _c_spell_cast_info;

	//// AuraComponent    ////

	Vector<Ref<AuraData> > _s_auras;
	Vector<Ref<AuraData> > _c_auras;

	EntityEnums::EntityType _s_entity_type;
	EntityEnums::EntityType _c_entity_type;

	int _s_immunity_flags;

	int _s_entity_flags;
	int _c_entity_flags;

	////    Cooldowns    ////
	Vector<Ref<Cooldown> > _s_cooldowns;
	Vector<Ref<Cooldown> > _c_cooldowns;

	HashMap<int, Ref<Cooldown> > _s_cooldown_map;
	HashMap<int, Ref<Cooldown> > _c_cooldown_map;

	Vector<Ref<CategoryCooldown> > _s_category_cooldowns;
	Vector<Ref<CategoryCooldown> > _c_category_cooldowns;

	int _s_active_category_cooldowns;
	int _c_active_category_cooldowns;

	////    Targeting    ////

	int _s_target_guid;
	Entity *_s_target;
	Entity *_c_target;

	////    Talents    ////

	int _s_free_talent_points;
	int _c_free_talent_points;

	Vector<int> _s_talents;
	Vector<int> _c_talents;

	////    Data    ////

	Vector<Ref<EntityDataContainer> > _s_data;
	Vector<Ref<EntityDataContainer> > _c_data;

	////    Actionbars    ////

	bool _actionbar_locked;
	Ref<ActionBarProfile> _action_bar_profile;

	////    Crafting System    ////

	Vector<Ref<CraftRecipe> > _s_craft_recipes;
	Vector<Ref<CraftRecipe> > _c_craft_recipes;

	////    Known Spells    ////

	int _s_free_spell_points;
	int _c_free_spell_points;

	Vector<Ref<Spell> > _s_spells;
	Vector<Ref<Spell> > _c_spells;

	////    Skills    ////

	Vector<Ref<EntitySkill> > _s_skills;
	Vector<Ref<EntitySkill> > _c_skills;

	////    Stat Allocations    ////

	//int _unallocated_stats;
	//int _stat_allocations[Stat::STAT_ID_TOTAL_STATS];

	////    Inventory    ////

	Ref<Bag> _s_bag;
	Ref<Bag> _c_bag;

	Ref<Bag> _s_target_bag;
	Ref<Bag> _c_target_bag;

	// AI
	
	EntityEnums::AIStates _sai_state;
	EntityEnums::AIStates _sai_state_stored;

	EntityEnums::EntityController _s_original_entity_controller;
	EntityEnums::EntityController _s_entity_controller;
	Ref<EntityAI> _s_ai;

	//Pets

	Entity *_s_pet_owner;
	Entity *_c_pet_owner;

	int _s_pet_formation_index;

	EntityEnums::AIStates _s_pet_ai_state;

	Vector<Entity *> _s_pets;
	Vector<Entity *> _c_pets;

	// Networking

	Vector<Entity *> _s_sees;
	Vector<Entity *> _s_seen_by;

	// Callbacks

	Vector<Ref<SpellCastInfo> > _physics_process_scis;
};

#endif
