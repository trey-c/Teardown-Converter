#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "entity.h"
#include "math_utils.h"
#include "parser.h"
#include "scene.h"
#include "vox_writer.h"
#include "xml_writer.h"
#include "../lib/tinyxml2.h"

void WriteXML::WriteScene() {
	char version[32];
	int ret = snprintf(version, 32, "%d.%d.%d", scene.version[0], scene.version[1], scene.version[2]);
	assert(ret >= 0 && ret < 32);
	xml.AddAttribute(xml.getScene(), "version", version);
	xml.AddFloatNAttribute(xml.getScene(), "shadowVolume", scene.shadowVolume, 3);
}

void WriteXML::WriteTransform(XMLElement* element, Transform tr) {
	float tr_pos[3] = {tr.pos.x, tr.pos.y, tr.pos.z};
	xml.AddFloatNAttribute(element, "pos", tr_pos, 3);
	float rotation[3];
	QuatToEuler(tr.rot, rotation[0], rotation[1], rotation[2]);
	xml.AddFloatNAttribute(element, "rot", rotation, 3);
}

void WriteXML::WriteSpawnpoint() {
	XMLElement* spawnpoint = xml.CreateElement("spawnpoint");
	xml.AddElement(xml.getScene(), spawnpoint);
	scene.spawnpoint.rot = scene.spawnpoint.rot * QuatEuler(0, 180, 0);
	WriteTransform(spawnpoint, scene.spawnpoint);
}

void WriteXML::WriteEnvironment() {
	Skybox* skybox = &scene.environment.skybox;
	Fog* fog = &scene.environment.fog;
	EnvWater* water = &scene.environment.water;
	Snow* snow = &scene.environment.snow;

	XMLElement* environment = xml.CreateElement("environment");
	xml.AddElement(xml.getScene(), environment);

	string skybox_texture = skybox->texture;
	string prefix = "data/env/";
	if (skybox_texture.find(prefix) == 0)
		skybox_texture = skybox_texture.substr(prefix.size());

	xml.AddStrAttribute(environment, "skybox", skybox_texture);
	xml.AddRgbaAttribute(environment, "skyboxtint", skybox->tint);
	xml.AddFloatAttribute(environment, "skyboxrot", deg(skybox->rot));
	xml.AddRgbaAttribute(environment, "constant", skybox->constant, true);
	xml.AddFloatAttribute(environment, "ambient", skybox->ambient);
	xml.AddFloatAttribute(environment, "ambientexponent", skybox->ambientexponent);
	xml.AddRgbaAttribute(environment, "fogColor", fog->color);
	xml.AddFloat4Attribute(environment, "fogParams", fog->start, fog->start + fog->distance, fog->amount, fog->exponent);
	xml.AddFloatAttribute(environment, "sunBrightness", skybox->sun.brightness);
	xml.AddRgbaAttribute(environment, "sunColorTint", skybox->sun.colorTint);
	xml.AddFloatNAttribute(environment, "sunDir", skybox->sun.dir, 3);
	xml.AddFloatAttribute(environment, "sunSpread", skybox->sun.spread);
	xml.AddFloatAttribute(environment, "sunLength", skybox->sun.length);
	xml.AddFloatAttribute(environment, "sunFogScale", skybox->sun.fogScale);
	xml.AddFloatAttribute(environment, "sunGlare", skybox->sun.glare);
	xml.AddFloatNAttribute(environment, "exposure", scene.environment.exposure, 2);
	xml.AddFloatAttribute(environment, "brightness", scene.environment.brightness);
	xml.AddFloatAttribute(environment, "wetness", water->wetness);
	xml.AddFloatAttribute(environment, "puddleamount", water->puddleamount);
	xml.AddFloatAttribute(environment, "puddlesize", 0.01 / water->puddlesize);
	xml.AddFloatAttribute(environment, "rain", water->rain);
	xml.AddBoolAttribute(environment, "nightlight", scene.environment.nightlight);
	xml.AddStrFloatAttribute(environment, "ambience", scene.environment.ambience.path, scene.environment.ambience.volume);
	xml.AddFloatAttribute(environment, "fogscale", scene.environment.fogscale);
	xml.AddFloatAttribute(environment, "slippery", scene.environment.slippery);
	xml.AddFloatAttribute(environment, "waterhurt", scene.environment.waterhurt);
	xml.AddFloat4Attribute(environment, "snowdir", snow->dir[0], snow->dir[1], snow->dir[2], snow->spread);
	xml.AddFloatAttribute(environment, "snowamount", snow->amount);
	xml.AddBoolAttribute(environment, "snowonground", snow->onground && !remove_snow);
	xml.AddFloatNAttribute(environment, "wind", scene.environment.wind, 3);
	if (snow->onground && !remove_snow) printf("Here comes the snow!\n");
	remove_snow = remove_snow && snow->onground; // Only remove snow if there is snow to remove
	if (remove_snow) printf("Removing the snow...\n");
}

void WriteXML::WriteBoundary() {
	int vertex_count = scene.boundary.vertices.getSize();
	if (vertex_count == 0) {
		printf("A world with no boundaries.\n");
		return;
	}

	XMLElement* boundary = xml.CreateElement("boundary");
	xml.AddElement(xml.getScene(), boundary);

	xml.AddFloatAttribute(boundary, "padleft", -scene.boundary.padleft);
	xml.AddFloatAttribute(boundary, "padright", scene.boundary.padright);
	xml.AddFloatAttribute(boundary, "padtop", -scene.boundary.padtop);
	xml.AddFloatAttribute(boundary, "padbottom", scene.boundary.padbottom);

	for (int i = 0; i < vertex_count; i++) {
		XMLElement* vertex = xml.CreateElement("vertex");
		xml.AddElement(boundary, vertex);
		xml.AddFloatNAttribute(vertex, "pos", scene.boundary.vertices[i].pos, 2);
	}
}

void WriteXML::WritePostProcessing() {
	XMLElement* postprocessing = xml.CreateElement("postprocessing");
	xml.AddElement(xml.getScene(), postprocessing);

	xml.AddFloatAttribute(postprocessing, "saturation", scene.postpro.saturation);
	xml.AddRgbaAttribute(postprocessing, "colorbalance", scene.postpro.colorbalance);
	xml.AddFloatAttribute(postprocessing, "brightness", scene.postpro.brightness);
	xml.AddFloatAttribute(postprocessing, "gamma", scene.postpro.gamma);
	xml.AddFloatAttribute(postprocessing, "bloom", scene.postpro.bloom);
}

void WriteXML::SaveXML() {
	string main_xml_path = save_path + "main.xml";
	xml.SaveFile(main_xml_path.c_str());
	printf("XML file saved!\n");
}

void WriteXML::SaveVoxFiles() {
	printf("Saving vox files...\n");
	for (map<uint32_t, MV_FILE*>::iterator it = vox_files.begin(); it != vox_files.end(); it++)
		it->second->SaveModel();
	for (list<MV_FILE*>::iterator it = compound_files.begin(); it != compound_files.end(); it++)
		(*it)->SaveModel();
	progress = 1;
}

void WriteXML::WriteEntities() {
	for (unsigned int i = 0; i < scene.entities.getSize(); i++)
		WriteEntity(xml.getScene(), scene.entities[i]);
	for (unsigned int i = 0; i < scene.entities.getSize(); i++)
		WriteEntity2ndPass(scene.entities[i]);
}

void WriteXML::WriteShape(XMLElement* &entity_element, Shape* shape, uint32_t handle) {
	entity_element->SetName("vox");
	xml.AddIntFloatAttribute(entity_element, "texture", shape->texture_tile, shape->texture_weight);
	xml.AddIntFloatAttribute(entity_element, "blendtexture", shape->blendtexture_tile, shape->blendtexture_weight);
	xml.AddFloatAttribute(entity_element, "density", shape->density);
	xml.AddFloatAttribute(entity_element, "strength", shape->strength);
	//xml.AddBoolAttribute(entity_element, "collide", shape->collide);
	xml.AddStrAttribute(entity_element, "prop", "false");

	uint8_t collide = shape->z_u8_4[0];
	if (collide != 33 && collide != 34 && collide != 53 && collide != 54 && collide != 58)
		printf("Unknow byte: %d, please report.\n", collide);

	int sizex = shape->voxels.size[0];
	int sizey = shape->voxels.size[1];
	int sizez = shape->voxels.size[2];
	unsigned int volume = sizex * sizey * sizez;
	if (volume == 0) {
		entity_element = NULL; // Ignore empty shapes
		return;
	}

	Palette palette = scene.palettes[shape->palette];
	shape->old_transform = shape->transform;

	bool is_filled_voxbox = true;
	uint8_t index = shape->voxels.palette_index[0];
	for (unsigned int i = 0; i < shape->voxels.palette_index.getSize(); i++)
		is_filled_voxbox = is_filled_voxbox && shape->voxels.palette_index[i] == index;

	if (is_filled_voxbox) {
		entity_element->SetName("voxbox");
		xml.AddFloat3Attribute(entity_element, "size", sizex, sizey, sizez);
		uint8_t index = shape->voxels.palette_index[0];
		Material palette_entry = palette.materials[index];
		xml.AddStrAttribute(entity_element, "material", MaterialKindName[palette_entry.kind]);
		xml.AddRgbaAttribute(entity_element, "color", palette_entry.rgba);
		xml.AddFloat4Attribute(entity_element, "pbr", palette_entry.reflectivity, palette_entry.shinyness, palette_entry.metalness, palette_entry.emissive);
		WriteTransform(entity_element, shape->transform);
		return;
	}

	Vector axis_offset(0.05f * (sizex - sizex % 2), 0.05f * (sizey - sizey % 2), 0);
	if (int(shape->scale * 10.0) == 2) // Scaled x2
		axis_offset = Vector(0.1f * (sizex - sizex % 2), 0.1f * (sizey - sizey % 2), 0);
	else if (int(shape->scale * 10.0) == 4) // Scaled x4
		axis_offset = Vector(0.2f * (sizex - sizex % 2), 0.2f * (sizey - sizey % 2), 0);

	shape->transform.pos = shape->transform.pos + shape->transform.rot * axis_offset;
	shape->transform.rot = shape->transform.rot * QuatEuler(90, 0, 0);
	WriteTransform(entity_element, shape->transform);

	if (volume > 0 && sizex <= 256 && sizey <= 256 && sizez <= 256) {
		string vox_filename = save_path + "vox\\palette" + to_string(shape->palette) + ".vox";
		string vox_path = "MOD/vox/palette" + to_string(shape->palette) + ".vox";
		string vox_object = "shape" + to_string(handle);

		MV_FILE* vox_file;
		bool is_new_palette = true;
		if (vox_files.find(shape->palette) == vox_files.end()) {
			vox_file = new MV_FILE(vox_filename.c_str());
			vox_files[shape->palette] = vox_file;
		} else {
			vox_file = vox_files[shape->palette];
			is_new_palette = false;
		}

		MVShape mvshape = { sizex, sizey, sizez, NULL, vox_object.c_str(), 0, 0, sizez / 2 };
		mvshape.voxels = MatrixInit(sizex, sizey, sizez);

		bool is_wheel_shape = false;
		if (remove_snow) {
			Entity* this_entity = entity_mapping[handle];
			Entity* vox_parent = this_entity->parent;
			if (vox_parent != NULL && vox_parent->kind_byte == KindBody) {
				Entity* vox_grandparent = vox_parent->parent;
				if (vox_grandparent != NULL && vox_grandparent->kind_byte == KindWheel)
					is_wheel_shape = true;
			}
			if (!is_wheel_shape) {
				mvshape.voxels[0][0][0] = 255;
				mvshape.voxels[sizex-1][sizey-1][sizez-1] = 255;
				vox_file->AddColor(255, 255, 0, 0);
			}
		}

		unsigned int k = 0;
		for (int z = 0; z < sizez; z++)
			for (int y = 0; y < sizey; y++)
				for (int x = 0; x < sizex; x++) {
					uint8_t index = shape->voxels.palette_index[k];
					if (index != 0) {
						Material palette_entry = palette.materials[index];
						mvshape.voxels[x][y][z] = index;

						if (remove_snow && !is_wheel_shape && palette_entry.kind == MaterialKind::Unphysical &&
							int(255.0 * palette_entry.rgba.r) == 229 && int(255.0 * palette_entry.rgba.g) == 229 && int(255.0 * palette_entry.rgba.b) == 229)
							mvshape.voxels[x][y][z] = 0;

						if (!vox_file->is_index_used[index]) {
							vox_file->AddColor(index, 255.0 * palette_entry.rgba.r, 255.0 * palette_entry.rgba.g, 255.0 * palette_entry.rgba.b);
							vox_file->AddPBR(index, palette_entry.kind, palette_entry.reflectivity, palette_entry.shinyness, palette_entry.metalness, palette_entry.emissive, palette_entry.rgba.a);
						}
					}
					k++;
				}

		bool duplicated = false;
		if (!is_new_palette)
			for (vector<MVShape>::iterator it = vox_file->models.begin(); it != vox_file->models.end() && !duplicated; it++)
				if (*it == mvshape) {
					duplicated = true;
					vox_object = it->name;
				}
		if (!duplicated)
			vox_file->AddShape(mvshape);

		xml.AddStrAttribute(entity_element, "file", vox_path);
		xml.AddStrAttribute(entity_element, "object", vox_object);
		xml.AddFloatAttribute(entity_element, "scale", 10.0 * shape->scale);
	} else {
		string compound_filename = save_path + "compounds\\compound" + to_string(handle) + ".vox";
		string compound_path = "MOD/compounds/compound" + to_string(handle) + ".vox";
		MV_FILE* compound_vox = new MV_FILE(compound_filename.c_str());
		compound_files.push_back(compound_vox);

		entity_element->SetName("compound");
		for (int i = 0; i < (sizex + 256 - 1) / 256; i++)
			for (int j = 0; j < (sizey + 256 - 1) / 256; j++)
				for (int k = 0; k < (sizez + 256 - 1) / 256; k++)
					WriteCompound(compound_vox, compound_path, entity_element, shape, i, j, k);
	}
}

void WriteXML::WriteCompound(MV_FILE* compound_vox, string vox_file, XMLElement* compound_xml, Shape* shape, int i, int j, int k) {
	XMLElement* shape_xml = xml.CreateElement("vox");
	Palette palette = scene.palettes[shape->palette];

	int offsetx = 256 * i;
	int offsety = 256 * j;
	int offsetz = 256 * k;
	int sizex = shape->voxels.size[0];
	int sizey = shape->voxels.size[1];
	int sizez = shape->voxels.size[2];
	int part_sizex = min(256, sizex - offsetx);
	int part_sizey = min(256, sizey - offsety);
	int part_sizez = min(256, sizez - offsetz);

	float pos_x = 25.6 * i + (part_sizex / 2) * 0.1;
	float pos_y = 25.6 * k;
	float pos_z = -25.6 * j - (part_sizey / 2) * 0.1;
	pos_x -= (sizex / 2) * 0.1;
	pos_z += (sizey / 2) * 0.1;

	string vox_object = "part" + to_string(i) + to_string(j) + to_string(k);
	xml.AddFloat3Attribute(shape_xml, "pos", pos_x, pos_y, pos_z);
	xml.AddStrAttribute(shape_xml, "file", vox_file);
	xml.AddStrAttribute(shape_xml, "object", vox_object);

	MVShape mvshape = { part_sizex, part_sizey, part_sizez, NULL, vox_object, 0, 0, part_sizez / 2 };
	mvshape.voxels = MatrixInit(part_sizex, part_sizey, part_sizez);
	mvshape.voxels[0][0][0] = 255;
	mvshape.voxels[part_sizex-1][part_sizey-1][part_sizez-1] = 255;
	compound_vox->AddColor(255, 255, 0, 0);

	bool empty = true;
	unsigned int it = 0; // TODO: optimize
	for (int z = 0; z < sizez; z++)
		for (int y = 0; y < sizey; y++)
			for (int x = 0; x < sizex; x++) {
				if (x >= offsetx && x < part_sizex + offsetx && y >= offsety && y < part_sizey + offsety && z >= offsetz && z < part_sizez + offsetz) {
					uint8_t index = shape->voxels.palette_index[it];
					if (index != 0) {
						Material palette_entry = palette.materials[index];
						mvshape.voxels[x - offsetx][y - offsety][z - offsetz] = index;

						if (remove_snow && palette_entry.kind == MaterialKind::Unphysical &&
							int(255.0 * palette_entry.rgba.r) == 229 && int(255.0 * palette_entry.rgba.g) == 229 && int(255.0 * palette_entry.rgba.b) == 229)
							mvshape.voxels[x - offsetx][y - offsety][z - offsetz] = 0;

						if (!compound_vox->is_index_used[index]) {
							compound_vox->AddColor(index, 255.0 * palette_entry.rgba.r, 255.0 * palette_entry.rgba.g, 255.0 * palette_entry.rgba.b);
							compound_vox->AddPBR(index, palette_entry.kind, palette_entry.reflectivity, palette_entry.shinyness, palette_entry.metalness, palette_entry.emissive, palette_entry.rgba.a);
						}
						empty = false;
					}
				}
				it++;
			}
	if (!empty) {
		xml.AddElement(compound_xml, shape_xml);
		compound_vox->AddShape(mvshape);
	}
}

void WriteXML::WriteEntity(XMLElement* parent, Entity* entity) {
	XMLElement* entity_element = xml.CreateElement("unknown");
	assert(entity->handle != 0);

	string tags = "";
	for (unsigned int i = 0; i < entity->tags.getSize(); i++) {
		tags += entity->tags[i].name;
		if (entity->tags[i].value.length() > 0)
			tags += "=" + entity->tags[i].value;
		if (i != entity->tags.getSize() - 1)
			tags += " ";
	}
	if (tags.length() > 0)
		xml.AddStrAttribute(entity_element, "tags", tags);

	switch (entity->kind_byte) {
		case KindBody: {
			Body* body = static_cast<Body*>(entity->kind);
			entity_element->SetName("body");

			Entity* body_parent = entity->parent;
			if (body_parent != NULL && body_parent->kind_byte == KindVehicle) {
				Vehicle* parent_vehicle = (Vehicle*)body_parent->kind;
				body->transform = TransformToLocalTransform(parent_vehicle->transform, body->transform);
			}
			WriteTransform(entity_element, body->transform); 

			if (body->dynamic == true)
				xml.AddBoolAttribute(entity_element, "dynamic", body->dynamic);
			else if (entity->parent == NULL && entity->children.getSize() > 0 && entity->tags.getSize() == 0) {
				entity_element->SetName("group"); // World Body
				xml.AddStrAttribute(entity_element, "name", "Static");
			} else if (entity->tags.getSize() == 0)
				entity_element = NULL; // Ignore static bodies with no tags
		}
			break;
		case KindShape: {
			Shape* shape = static_cast<Shape*>(entity->kind);
			WriteShape(entity_element, shape, entity->handle);
		}
			break;
		case KindLight: {
			Light* light = static_cast<Light*>(entity->kind);
			entity_element->SetName("light");

			Entity* light_parent = entity->parent;
			if (light_parent != NULL && light_parent->kind_byte == KindShape) {
				Shape* parent_shape = (Shape*)light_parent->kind;
				Transform light_tr = TransformToLocalTransform(parent_shape->transform, light->transform);
				WriteTransform(entity_element, light_tr);
			} else if (light_parent != NULL && light_parent->kind_byte != KindScreen && light_parent->kind_byte != KindLight)
				WriteTransform(entity_element, light->transform);

			if (light->type == Sphere)
				xml.AddAttribute(entity_element, "type", "sphere");
			else if (light->type == Capsule)
				xml.AddAttribute(entity_element, "type", "capsule");
			else if (light->type == Cone)
				xml.AddAttribute(entity_element, "type", "cone");
			else if (light->type == Area)
				xml.AddAttribute(entity_element, "type", "area");

			xml.AddFloat4Attribute(entity_element, "color", pow(light->color.r, 0.454545), pow(light->color.g, 0.454545), pow(light->color.b, 0.454545), light->color.a);
			xml.AddFloatAttribute(entity_element, "scale", light->scale);

			if (light->type == Cone) {
				xml.AddFloatAttribute(entity_element, "angle", 2.0 * deg(acos(light->angle)));
				xml.AddFloatAttribute(entity_element, "penumbra", 2.0 * deg(acos(light->angle) - acos(light->penumbra)));
			}

			if (light->type == Area)
				xml.AddFloat2Attribute(entity_element, "size", 2.0 * light->area_size[0], 2.0 * light->area_size[1]);
			else if (light->type == Capsule)
				xml.AddFloat2Attribute(entity_element, "size", 2.0 * light->capsule_size, light->size);
			else
				xml.AddFloatAttribute(entity_element, "size", light->size);

			xml.AddFloatAttribute(entity_element, "reach", light->reach);
			xml.AddFloatAttribute(entity_element, "unshadowed", light->unshadowed);
			xml.AddFloatAttribute(entity_element, "fogscale", light->fogscale);
			xml.AddFloatAttribute(entity_element, "fogiter", light->fogiter);
			xml.AddStrFloatAttribute(entity_element, "sound",  light->sound.path, light->sound.volume);
			xml.AddFloatAttribute(entity_element, "glare", light->glare);

			if (entity->parent == NULL)
				entity_element = NULL; // Ignore player flashlight
		}
			break;
		case KindLocation: {
			Location* location = static_cast<Location*>(entity->kind);
			entity_element->SetName("location");

			Entity* location_parent = entity->parent;
			if (location_parent != NULL && location_parent->kind_byte == KindShape) {
				// The location is inside a static shape
				Shape* parent_shape = (Shape*)location_parent->kind;
				Transform loc_tr = TransformToLocalTransform(parent_shape->transform, location->transform);
				WriteTransform(entity_element, loc_tr);

				// The location is inside a dynamic body
				Entity* location_grandparent = location_parent->parent;
				if (location_grandparent != NULL && location_grandparent->kind_byte == KindBody) {
					Body* grandparent_body = (Body*)location_grandparent->kind;
					if (grandparent_body->dynamic) {
						Transform location_tr = TransformToLocalTransform(grandparent_body->transform, location->transform);
						location_tr = TransformToLocalTransform(parent_shape->transform, location_tr);
						WriteTransform(entity_element, location_tr);
					}
				}
			} else if (location_parent != NULL && location_parent->kind_byte == KindBody) {
				Body* parent_body = (Body*)location_parent->kind;
				Transform loc_tr = TransformToLocalTransform(parent_body->transform, location->transform);
				WriteTransform(entity_element, loc_tr);
			} else
				WriteTransform(entity_element, location->transform);
		}
			break;
		case KindWater: {
			Water* water = static_cast<Water*>(entity->kind);
			entity_element->SetName("water");
			WriteTransform(entity_element, water->transform);
			xml.AddStrAttribute(entity_element, "type", "polygon");
			xml.AddFloatAttribute(entity_element, "depth", water->depth);
			xml.AddFloatAttribute(entity_element, "wave", water->wave);
			xml.AddFloatAttribute(entity_element, "ripple", water->ripple);
			xml.AddFloatAttribute(entity_element, "motion", water->motion);
			xml.AddFloatAttribute(entity_element, "foam", water->foam);
			xml.AddRgbaAttribute(entity_element, "color", water->color);

			int vertex_count = water->water_vertices.getSize();
			for (int i = 0; i < vertex_count; i++) {
				XMLElement* vertex = xml.CreateElement("vertex");
				xml.AddElement(entity_element, vertex);
				xml.AddFloatNAttribute(vertex, "pos", water->water_vertices[i].pos, 2);
			}
		}
			break;
		case KindJoint: {
			Joint* joint = static_cast<Joint*>(entity->kind);
			if (joint->type == _Rope) {
				entity_element->SetName("rope");
				xml.AddFloatAttribute(entity_element, "size", joint->size);
				xml.AddRgbaAttribute(entity_element, "color", joint->rope.color);
				xml.AddFloatAttribute(entity_element, "strength", joint->rope.strength);
				xml.AddFloatAttribute(entity_element, "maxstretch", joint->rope.maxstretch);

				int knot_count = joint->rope.knots.getSize();
				if (knot_count > 0) {
					XMLElement* location_from = xml.CreateElement("location");
					xml.AddElement(entity_element, location_from);
					xml.AddFloatNAttribute(location_from, "pos", joint->rope.knots[0].from, 3);

					XMLElement* location_to = xml.CreateElement("location");
					xml.AddElement(entity_element, location_to);
					xml.AddFloatNAttribute(location_to, "pos", joint->rope.knots[knot_count - 1].to, 3);

					/*Vector rope_start = joint->rope.knots[0].from;
					Vector rope_end = joint->rope.knots[knot_count - 1].to;
					Vector rope_dir = rope_end - rope_start;
					float rope_length = rope_dir.length();
					xml.AddFloatAttribute(entity_element, "slack", rope->slack);*/
				}
			} else
				entity_element = NULL; // Process joints on a second pass
		}
			break;
		case KindVehicle: {
			Vehicle* vehicle = static_cast<Vehicle*>(entity->kind);
			entity_element->SetName("vehicle");
			WriteTransform(entity_element, vehicle->transform);

			//xml.AddFloatAttribute(entity_element, "driven", vehicle->driven);
			xml.AddStrFloatAttribute(entity_element, "sound", vehicle->properties.sound.name, vehicle->properties.sound.pitch);
			xml.AddFloatAttribute(entity_element, "spring", vehicle->properties.spring);
			xml.AddFloatAttribute(entity_element, "damping", vehicle->properties.damping);
			xml.AddFloatAttribute(entity_element, "topspeed", 3.6 * vehicle->properties.topspeed);
			xml.AddFloatAttribute(entity_element, "acceleration", vehicle->properties.acceleration);
			xml.AddFloatAttribute(entity_element, "strength", vehicle->properties.strength);
			xml.AddFloatAttribute(entity_element, "antispin", vehicle->properties.antispin);
			xml.AddFloatAttribute(entity_element, "antiroll", vehicle->properties.antiroll);
			xml.AddFloatAttribute(entity_element, "difflock", vehicle->difflock);
			xml.AddFloatAttribute(entity_element, "steerassist", vehicle->properties.steerassist);
			xml.AddFloatAttribute(entity_element, "friction", vehicle->properties.friction);

			int exhausts_count = vehicle->exhausts.getSize();
			for (int i = 0; i < exhausts_count; i++) {
				XMLElement* exhaust = xml.CreateElement("location");
				xml.AddElement(entity_element, exhaust);
				xml.AddAttribute(exhaust, "tags", "exhaust");
				WriteTransform(exhaust, vehicle->exhausts[i].transform);
			}

			XMLElement* player = xml.CreateElement("location");
			xml.AddElement(entity_element, player);
			xml.AddAttribute(player, "tags", "player");
			xml.AddFloatNAttribute(player, "pos", vehicle->player, 3);

			if (vehicle->camera[0] != vehicle->player[0] || vehicle->camera[1] != vehicle->player[1] || vehicle->camera[2] != vehicle->player[2]) {
				XMLElement* camera = xml.CreateElement("location");
				xml.AddElement(entity_element, camera);
				xml.AddAttribute(camera, "tags", "camera");
				xml.AddFloatNAttribute(camera, "pos", vehicle->camera, 3);
			}

			if (vehicle->exit[0] != 0 || vehicle->exit[1] != 0 || vehicle->exit[2] != 0) {
				XMLElement* exit = xml.CreateElement("location");
				xml.AddElement(entity_element, exit);
				xml.AddAttribute(exit, "tags", "exit");
				xml.AddFloatNAttribute(exit, "pos", vehicle->exit, 3);
			}

			bool is_boat = false;
			for (unsigned int i = 0; i < entity->tags.getSize() && !is_boat; i++)
				is_boat = entity->tags[i].name == "boat";

			if (is_boat) {
				XMLElement* propeller = xml.CreateElement("location");
				xml.AddElement(entity_element, propeller);
				xml.AddAttribute(propeller, "tags", "propeller");
				xml.AddFloatNAttribute(propeller, "pos", vehicle->propeller, 3);
			}
		}
			break;
		case KindWheel: {
			Wheel* wheel = static_cast<Wheel*>(entity->kind);
			entity_element->SetName("wheel");

			Entity* wheel_parent = entity->parent;
			if (wheel_parent != NULL && wheel_parent->kind_byte == KindShape) {
				Shape* parent_shape = (Shape*)wheel_parent->kind;
				Transform wheel_tr = TransformToLocalTransform(parent_shape->transform, wheel->transform);
				WriteTransform(entity_element, wheel_tr);
			} else
				WriteTransform(entity_element, wheel->transform);

			xml.AddFloatAttribute(entity_element, "drive", wheel->drive);
			xml.AddFloatAttribute(entity_element, "steer", wheel->steer);
			xml.AddFloatNAttribute(entity_element, "travel", wheel->travel, 2);
		}
			break;
		case KindScreen: {
			Screen* screen = static_cast<Screen*>(entity->kind);
			entity_element->SetName("screen");

			Entity* screen_parent = entity->parent;
			if (screen_parent != NULL && screen_parent->kind_byte == KindShape) {
				Shape* parent_shape = (Shape*)screen_parent->kind;
				// Revert applied transform
				Transform parent_shape_tr = TransformToLocalTransform(parent_shape->old_transform, parent_shape->transform);
				Transform screen_tr = TransformToLocalTransform(parent_shape_tr, screen->transform);
				WriteTransform(entity_element, screen_tr);
			} else
				WriteTransform(entity_element, screen->transform);

			xml.AddFloatNAttribute(entity_element, "size", screen->size, 2);
			xml.AddFloatAttribute(entity_element, "bulge", screen->bulge);

			char resolution[32];
			int ret = snprintf(resolution, 32, "%d %d", screen->resolution[0], screen->resolution[1]);
			assert(ret >= 0 && ret < 32);
			xml.AddAttribute(entity_element, "resolution", resolution);

			string script_file = screen->script;
			string prefix = "data/level/" + level_id;
			if (script_file.find(prefix) == 0)
				script_file = "MOD" + script_file.substr(prefix.size());

			prefix = "data/";
			if (script_file.find(prefix) == 0)
				script_file = script_file.substr(prefix.size());

			xml.AddStrAttribute(entity_element, "script", script_file);
			xml.AddBoolAttribute(entity_element, "enabled", screen->enabled);
			xml.AddBoolAttribute(entity_element, "interactive", screen->interactive);
			xml.AddFloatAttribute(entity_element, "emissive", screen->emissive);
			xml.AddFloatAttribute(entity_element, "fxraster", screen->fxraster);
			xml.AddFloatAttribute(entity_element, "fxca", screen->fxca);
			xml.AddFloatAttribute(entity_element, "fxnoise", screen->fxnoise);
			//xml.AddFloatAttribute(entity_element, "fxglitch", screen->fxglitch);
		}
			break;
		case KindTrigger: {
			Trigger* trigger = static_cast<Trigger*>(entity->kind);
			entity_element->SetName("trigger");
			if (trigger->type == TrBox) {
				Vector offset = Vector(0, trigger->box_size[1], 0);
				trigger->transform.pos = trigger->transform.pos - trigger->transform.rot * offset;
			}
			WriteTransform(entity_element, trigger->transform);

			if (trigger->type == TrSphere) {
				xml.AddAttribute(entity_element, "type", "sphere");
				xml.AddFloatAttribute(entity_element, "size", trigger->sphere_size);
			} else if (trigger->type == TrBox) {
				xml.AddAttribute(entity_element, "type", "box");
				xml.AddFloat3Attribute(entity_element, "size", 2.0 * trigger->box_size[0], 2.0 * trigger->box_size[1], 2.0 * trigger->box_size[2]);
			} else if (trigger->type == TrPolygon) {
				xml.AddAttribute(entity_element, "type", "polygon");
				xml.AddFloatAttribute(entity_element, "size", trigger->polygon_size);

				int vertex_count = trigger->polygon_vertices.getSize();
				for (int i = 0; i < vertex_count; i++) {
					XMLElement* vertex = xml.CreateElement("vertex");
					xml.AddElement(entity_element, vertex);
					xml.AddFloatNAttribute(vertex, "pos", trigger->polygon_vertices[i].pos, 2);
				}
			}
			xml.AddStrFloatAttribute(entity_element, "sound", trigger->sound.path, trigger->sound.volume);
			xml.AddFloatAttribute(entity_element, "soundramp", trigger->sound.soundramp);
		}
			break;
		case KindScript:
			entity_element = NULL;
			break;
	}

	if (entity_element != NULL && entity->desc.length() > 0)
		xml.AddStrAttribute(entity_element, "desc", entity->desc);

	if (entity_element != NULL)
		xml.AddElement(parent, entity_element, entity->handle);
	else
		entity_element = parent;

	for (unsigned int i = 0; i < entity->children.getSize(); i++)
		WriteEntity(entity_element, entity->children[i]);
}

void WriteXML::WriteEntity2ndPass(Entity* entity) {
	if (entity->kind_byte == KindVehicle) {
		Vehicle* vehicle = static_cast<Vehicle*>(entity->kind);
		int vital_count = vehicle->vitals.getSize();
		for (int i = 0; i < vital_count; i++) {
			uint32_t body_handle = vehicle->vitals[i].body_handle;
			XMLElement* xml_body = xml.getNode(body_handle);
			if (xml_body != NULL) {
				XMLElement* vital = xml.CreateElement("location");
				xml.AddElement(xml_body, vital);
				xml.AddAttribute(vital, "tags", "vital");
				xml.AddFloatNAttribute(vital, "pos", vehicle->vitals[i].pos, 3);
			} else
				printf("Warning: vital body %d not found\n", body_handle);
		}
	} else if (entity->kind_byte == KindJoint) {
		Joint* joint = static_cast<Joint*>(entity->kind);
		if (joint->type != _Rope) {
			XMLElement* entity_element = xml.CreateElement("joint");
			xml.AddFloatAttribute(entity_element, "size", joint->size);
			xml.AddBoolAttribute(entity_element, "collide", joint->collide);
			xml.AddBoolAttribute(entity_element, "sound", joint->sound);

			// Add tags to joints
			string tags = "";
			for (unsigned int i = 0; i < entity->tags.getSize(); i++) {
				tags += entity->tags[i].name;
				if (entity->tags[i].value.length() > 0)
					tags += "=" + entity->tags[i].value;
				if (i != entity->tags.getSize() - 1)
					tags += " ";
			}
			if (tags.length() > 0)
				xml.AddStrAttribute(entity_element, "tags", tags);

			uint32_t shape_handle = joint->shape_handles[0];
			XMLElement* shape_parent = xml.getNode(shape_handle);
			if (shape_parent != NULL && entity_mapping.find(shape_handle) != entity_mapping.end()) {
				Entity* entity = entity_mapping[shape_handle];
				assert(entity->kind_byte == KindShape);
				Shape* shape = (Shape*)entity->kind;
				Transform shape_tr = shape->transform;
				Vector relative_pos(joint->shape_positions[0][0], joint->shape_positions[0][1], joint->shape_positions[0][2]);

				Quat relative_rot;
				if (joint->type != Ball) {
					float rx = joint->shape_axes[0][0]; // sin(a) * cos(b) * sin(c) + cos(a) * sin(b)
					float ry = joint->shape_axes[0][1]; // -sin(a) * cos(c)
					float rz = joint->shape_axes[0][2]; // -sin(a) * sin(b) * sin(c) + cos(a) * cos(b)
					xml.AddFloat3Attribute(entity_element, "name", rx, ry, rz);

					double a = -asin(ry);
					double b = asin(rx / cos(a));
					if (!isnan(a) && !isnan(b))
						relative_rot = QuatEulerRad(a, b, 0);
					else
						relative_rot = QuatEuler(0, -90, 90);
				}
				Transform joint_tr = TransformToLocalTransform(shape_tr, Transform(relative_pos, relative_rot));
				WriteTransform(entity_element, joint_tr);
			}

			if (joint->type == Ball)
				xml.AddAttribute(entity_element, "type", "ball");
			else if (joint->type == Hinge) {
				xml.AddAttribute(entity_element, "type", "hinge");
				if (joint->limits[0] != 0.0 || joint->limits[1] != 0.0)
					xml.AddFloat2Attribute(entity_element, "limits", deg(joint->limits[0]), deg(joint->limits[1]));
			} else if (joint->type == Prismatic) {
				xml.AddAttribute(entity_element, "type", "prismatic");
				xml.AddFloatNAttribute(entity_element, "limits", joint->limits, 2);
				xml.AddBoolAttribute(entity_element, "autodisable", joint->autodisable);
			}

			if (joint->type != Prismatic) {
				xml.AddFloatAttribute(entity_element, "rotstrength", joint->rotstrength);
				xml.AddFloatAttribute(entity_element, "rotspring", joint->rotspring);
			}
			if (shape_parent != NULL) // Ignore joints without an attached shape
				xml.AddElement(shape_parent, entity_element, entity->handle);
		}
	} else if (entity->kind_byte == KindScript) {
		Script* script = static_cast<Script*>(entity->kind);
		XMLElement* entity_element = xml.CreateElement("script");

		string script_file = script->file;
		string prefix = "data/script/";
		if (script_file.find(prefix) == 0)
			script_file = script_file.substr(prefix.size());

		prefix = "data/level/" + level_id;
		if (script_file.find(prefix) == 0)
			script_file = "MOD" + script_file.substr(prefix.size());

		if (script_file == "fx.lua" || script_file == "explosion.lua" || script_file == "spawn.lua")
			return;
		xml.AddElement(xml.getScene(), entity_element);

		xml.AddStrAttribute(entity_element, "file", script_file);
		assert(script->params.getSize() <= 4);
		for (unsigned int i = 0; i < script->params.getSize(); i++) {
			string param_index = "param";
			param_index += to_string(i);
			string param = script->params[i].key;
			if (script->params[i].value.length() > 0)
				param += "=" + script->params[i].value;
			if (param.length() > 0)
				xml.AddStrAttribute(entity_element, param_index.c_str(), param);
		}

		for (unsigned int j = 0; j < script->entity_handles.getSize(); j++) {
			uint32_t entity_handle = script->entity_handles[j];
			XMLElement* entity_child = xml.getNode(entity_handle);
			if (entity_child != NULL) {
				int detect_loop = 0;
				while (entity_child->Parent()->ToElement() != xml.getScene() &&
					strcmp(entity_child->Parent()->ToElement()->Name(), "group") != 0 && detect_loop < 10) {
					entity_child = entity_child->Parent()->ToElement();
					detect_loop++;
				}
				if (detect_loop > 10)
					printf("ERROR!!! trying to find the top level parent of entity %d\n", entity_handle);
				if (entity_element != entity_child)
					xml.MoveElement(entity_element, entity_child);
			}
		}
	}

	for (unsigned int i = 0; i < entity->children.getSize(); i++)
		WriteEntity2ndPass(entity->children[i]);
}
