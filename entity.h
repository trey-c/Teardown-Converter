#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <iosfwd>
#include <stdexcept>
#include <string>

#include "lua_table.h"
#include "math_utils.h"

using namespace std;

template <typename T>
class Vec {
private:
	uint32_t size;
	T* data;
public:
	Vec() {
		size = 0;
		data = nullptr;
	}
	void resize(uint32_t size) {
		this->size = size;
		if (size > 0)
			data = new T[size];
	}
	~Vec() {
		size = 0;
		delete[] data;
		data = nullptr;
	}
	uint32_t getSize() const {
		return size;
	}
	Vec& operator=(const Vec& other) {
		if (this != &other) {
			resize(other.getSize());
			for (unsigned int i = 0; i < size; i++)
				data[i] = other[i];
		}
		return *this;
	}
	T operator[](uint32_t index) const {
		if (index > size - 1)
			throw out_of_range("Position out of range");
		return data[index];
	}
	T& operator[](uint32_t index) {
		if (index > size - 1)
			throw out_of_range("Position out of range");
		return data[index];
	}
};

#define SmallVec Vec

extern const char* EntityKindName[];

typedef uint16_t BitFlags;

struct Vertex { // vertex
	float pos[2]; // pos
};

struct Registry {
	string key;
	string value;
};

struct Rgba {
	float r, g, b, a;
};

struct Sound {
	string path;
	float volume;
};

// ------------------------------------

enum EntityKind { // uint8_t
	KindBody = 1,
	KindShape,
	KindLight,
	KindLocation,
	KindWater,
	KindJoint,
	KindVehicle,
	KindWheel,
	KindScreen,
	KindTrigger,
	KindScript,
};

struct Tag {
	string name;
	string value;
};

struct Entity {
	uint8_t kind_byte;
	uint32_t handle;
	SmallVec<Tag> tags; // uint8_t size
	string desc;		// desc
	void* kind;
	Vec<Entity*> children;
	uint32_t beef_beef;

	Entity* parent;
	~Entity();
};

struct Body {
	BitFlags entity_flags;
	Transform transform;
	float velocity[3];
	float angular_velocity[3];
	bool dynamic;				// dynamic
	uint8_t body_flags;
};

struct Voxels {
	uint32_t size[3];
	// if the shape volume is not empty, voxels are stored using run length encoding
	// with pairs (n-1, i) in the zyx order
	// the next array stores a voxel matrix and not the binary data
	Vec<uint8_t> palette_index;
};

// TODO: bool collide; 
struct Shape {
	BitFlags flags;
	Transform transform;
	uint8_t z_u8_4[4];
	float density;
	float strength;				// strength
	uint16_t texture_tile;		// texture
	uint16_t blendtexture_tile;	// blendtexture
	float texture_weight;		// texture
	float blendtexture_weight;	// blendtexture
	float starting_world_position[3];
	float z_f32;
	uint8_t z1_u8;
	uint8_t z2_u8;
	Voxels voxels;
	uint32_t palette;
	float scale;				// scale = 10.0 * this
	int z_i32_2[2];
	uint8_t z3_u8;

	Transform old_transform;	// used for screens
};

enum LightType { // uint8_t
	Sphere = 1,
	Capsule,
	Cone,
	Area,
};

struct Light {
	bool is_on;
	uint8_t type;		// type
	Transform transform;
	Rgba color;			// color = pow(this, 0.454545)
	float scale;		// scale
	float reach;		// reach
	float size;			// size
	float unshadowed;	// unshadowed
	float angle;		// angle = 2.0 * deg(acos(this))
	float penumbra;		// penumbra = 2.0 * deg(acos(angle) - acos(this))
	float fogiter;		// fogiter
	float fogscale;		// fogscale
	float area_size[2];	// size = 2.0 * this
	float capsule_size;	// size.x = 2.0 * this
	uint8_t z_u8_13[13];
	float z2_f32;
	Sound sound;		// sound
	float glare;		// glare
};

struct Location {
	BitFlags flags;
	Transform transform;
};

struct Water {
	BitFlags flags;
	Transform transform;
	float depth;		// depth
	float wave;			// wave
	float ripple;		// ripple
	float motion;		// motion
	float foam;			// foam
	Rgba color;			// color
	Vec<Vertex> water_vertices;
};

enum JointType { // uint32_t
	Ball = 1,
	Hinge,
	Prismatic,
	_Rope,
};

struct Knot {
	float from[3];
	float to[3];
};

// TODO: float slack
struct Rope {
	Rgba color;			// color
	float z_f32;
	float strength;		// strength
	float maxstretch;	// maxstretch
	float z_f32_2[2];
	uint8_t z_u8;
	Vec<Knot> knots;
};

struct Joint {
	uint32_t type;			// type
	uint32_t shape_handles[2];
	float shape_positions[2][3];
	float shape_axes[2][3];
	bool connected;
	bool collide;			// collide
	float rotstrength;		// rotstrength
	float rotspring;		// rotspring
	float ball_rot[4];
	float limits[2];		// limits (in degrees for hinge, meters for prismatic)
	float z_f32[2];
	float size;				// size
	bool sound;				// sound
	bool autodisable;		// autodisable
	uint32_t z_u32_2[2];
	Rope rope;				// Only if type = Rope
};

struct VehicleSound {
	string name;
	float pitch;
};

struct VehicleProperties {
	float topspeed;		// topspeed = 3.6 * this
	float z1_f32;
	float spring;		// spring
	float damping;		// damping
	float acceleration;	// acceleration
	float strength;		// strength
	float friction;		// friction
	float z2_f32;
	uint8_t z1_u8;
	float antispin;		// antispin
	float steerassist;	// steerassist
	float z3_f32;
	float antiroll;		// antiroll
	VehicleSound sound;	// sound
};

struct Exhaust {
	Transform transform;
	float z_f32;
};

struct Vital {
	uint32_t body_handle;
	float pos[3];
	float z_f32;
	uint32_t shape_index;
};

// TODO: bool driven;
struct Vehicle {
	BitFlags flags;
	uint32_t body_handle;
	Transform transform;
	float velocity[3];
	float angular_velocity[3];
	float z_f32_not_health;
	Vec<uint32_t> wheel_handles;
	VehicleProperties properties;
	float camera[3];		// camera
	float player[3];		// player
	float exit[3];			// exit
	float propeller[3];		// propeller
	float difflock;			// difflock
	float z6_f32_eq_1;
	uint32_t z_u32;
	uint8_t z2_u8;
	float z7_f32_eq_0;
	Vec<uint32_t> refs;
	Vec<Exhaust> exhausts;	// exhaust
	Vec<Vital> vitals;		// vital
	float z2_f32;
	uint8_t z3_u8;
	float z3_f32;
};

struct Wheel {
	BitFlags flags;
	uint32_t vehicle;
	uint32_t vehicle_body;
	uint32_t body;
	uint32_t shape;
	Transform transform;
	float z_f32_7[7];
	float steer;			// steer
	float drive;			// drive
	float travel[2];		// travel
	float z1_f32_2[2];
	float angular_speed;
	float z2_f32_2[2];
};

// TODO: float fxglitch;
struct Screen {
	BitFlags flags;
	Transform transform;
	float size[2];			// size
	float bulge;			// bulge
	uint32_t resolution[2];	// resolution
	string script;			// script
	bool enabled;			// enabled
	bool interactive;		// interactive
	float emissive;			// emissive
	float fxraster;			// fxraster
	float fxca;				// fxca
	float fxnoise;			// fxnoise
	float z_f32;
};

enum TriggerKind { // uint32_t
	TrSphere = 1,
	TrBox,
	TrPolygon,
};

struct TriggerSound {
	string path;		// sound
	float soundramp;	// soundramp
	uint8_t z_u8_1;
	float volume;		// sound
};

struct Trigger {
	BitFlags flags;
	Transform transform;
	uint32_t type;				// type
	float sphere_size;			// size
	float box_size[3];			// size = 2.0 * this
	float polygon_size;			// size
	Vec<Vertex> polygon_vertices;
	TriggerSound sound;
};

enum ScriptSoundKind { // uint32_t
	Normal = 1,
	Loop,
	Unknown,
};

enum TransitionKind { // uint8_t
	Cosine,
	EaseIn,
	EaseOut,
	Bounce
};

struct ScriptSound {
	uint32_t kind;
	string name;
};

struct ValueTransition {
	string variable;
	uint8_t kind;
	float transition_time;
	double time;
	uint8_t z_u8_4[4];
};

struct Script {
	BitFlags flags;
	string file;			// file
	Vec<Registry> params;	// param%d
	float last_update;
	float time;
	uint8_t z_u8_4[4];
	LuaTable table;
	Vec<uint32_t> entity_handles;
	Vec<ScriptSound> sounds;
	Vec<ValueTransition> transitions;
};

#endif
