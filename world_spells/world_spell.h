#ifndef WORLD_SPELL_H
#define WORLD_SPELL_H

#include "scene/main/node.h"

#include "core/vector.h"
#include "scene/resources/packed_scene.h"

#include "../spell_enums.h"

#include "world_spell_data.h"


class WorldSpell : public Node {
	GDCLASS(WorldSpell, Node);

public:
	NodePath get_body_path();
	void set_body_path(NodePath value);
	Node *get_body();

	int get_data_id();
	void set_data_id(int value);

	Ref<WorldSpellData> get_data();
	void set_data(Ref<WorldSpellData> value);

	SpellEnums::ColliderType get_collider_type();
	void set_collider_type(SpellEnums::ColliderType value);

	Vector3 get_collider_box_extents();
	void set_collider_box_extents(Vector3 value);

	float get_collider_sphere_radius();
	void set_collider_sphere_radius(float value);

	SpellEnums::TargetType get_target_type();
	void set_target_type(SpellEnums::TargetType value);

	int get_target_bone_id();
	void set_target_bone_id(int value);

	bool get_move();
	void set_move(bool value);

	float get_movement_speed();
	void set_movement_speed(float value);

	Vector3 get_movement_dir();
	void set_movement_dir(Vector3 value);

	float get_max_dist();
	void set_max_dist(float value);

	Ref<PackedScene> get_effect();
	void set_effect(Ref<PackedScene> value);

	Vector3 get_effect_offset();
	void set_effect_offset(Vector3 value);

	WorldSpell();
	~WorldSpell();

protected:
	static void _bind_methods();

private:
	NodePath _body_path;
	Node *_body;

	int _data_id;
	Ref<WorldSpellData> _data;

	SpellEnums::ColliderType _collider_type;
	Vector3 _collider_box_extents;
	float _collider_sphere_radius;

	SpellEnums::TargetType _target_type;
	int _target_bone_id;

	bool _move;
	float _movement_speed;
	Vector3 _movement_dir;

	float _max_dist;

	Ref<PackedScene> _effect;
	Vector3 _effect_offset;
};

#endif
