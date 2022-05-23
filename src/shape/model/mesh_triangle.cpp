#include "shape/model/mesh_triangle.h"
#include "utility/tiny_obj_loader.h"
#include "utility/OBJ_Loader.hpp"

#include <functional>
#include <unordered_map>
#include <cassert>
#include <array>

namespace shape::model
{
	mesh_triangle::mesh_triangle(const std::string& filename, shared_ptr<material> ptr, vec3 translate, vec3 scale)
	{
		//load_obj(filename, ptr, translate, scale);
		load_tiny_obj(filename, ptr, translate, scale);
	}

	bool mesh_triangle::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		return bvh_tree.hit(r_in, t_min, t_max, rec);
	}

	// todo: check this
	bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		aabb temp;
		for (int i = 0 ; i < triangles.size() ; ++i)
		{
			triangles[i]->bounding_box(0, 0, temp);
			output_box = surrounding_box(output_box, temp);
		}

		return true;
	}

	mesh_triangle::~mesh_triangle()
	{
	}

	void mesh_triangle::load_obj(const std::string &path, shared_ptr<material> mt, vec3& translate, vec3& scale)
	{
		objl::Loader loader;
		if (!loader.LoadFile(path))
		{
			std::cout << "Cannot load obj :" << path.c_str() << "\n";
			return;
		}

		assert(loader.LoadedMeshes.size() == 1);
		auto mesh = loader.LoadedMeshes[0];

		std::cout << "load obj :" << path.c_str() << "\n";

		// model
		/*  scale_x 0       0       x
			0       scale_y 0       y
			0       0       scale_z z
			0       0       0       1
		*/

		std::vector<shared_ptr<object>> list;

		std::array<double, 12> model;
		model[0] = scale[0];
		model[5] = scale[1];
		model[10] = scale[2];

		model[3] = translate[0];
		model[7] = translate[1];
		model[11] = translate[2];

		vec3 min_vert = vec3(infinity, infinity, infinity);
		vec3 max_vert = vec3(-infinity, -infinity, -infinity);

		for (int i = 0 ; i < mesh.Vertices.size() ; i += 3)
		{
			std::array<vec3, 3> face_vertices;
			for (int j = 0 ; j < 3 ; ++j)
			{
				auto vert = vec3(mesh.Vertices[i + j].Position.X,
								 mesh.Vertices[i + j].Position.Y,
								 mesh.Vertices[i + j].Position.Z);

				vert[0] *= model[0];
				vert[1] *= model[5];
				vert[2] *= model[10];

				vert[0] += model[3];
				vert[1] += model[7];
				vert[2] += model[11];

				face_vertices[j] = vert;
				min_vert = vec3(std::min(min_vert.x(), vert.x()),
								std::min(min_vert.y(), vert.y()),
								std::min(min_vert.z(), vert.z()));
				max_vert = vec3(std::max(max_vert.x(), vert.x()),
								std::max(max_vert.y(), vert.y()),
								std::max(max_vert.z(), vert.z()));

				shared_ptr<triangle> tt = std::make_shared<triangle>(triangle(face_vertices[0], face_vertices[1],
															  face_vertices[2], mt));
				triangles.emplace_back(tt);
				list.push_back(tt);
			}
		}

//		mesh_box = aabb(min_vert, max_vert);

//		for (auto &tri : triangles)
//			area += tri.get_area();

		bvh_tree = bvh_node(list, 0, 0, bvh_node::SplitMethod::NAIVE);
	}

	void mesh_triangle::load_tiny_obj(const std::string &path, shared_ptr<material> mt, vec3 translate, vec3 scale)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn, err;
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
		{
			std::cout << "Cannot load Mesh " <<  path.c_str() << "\n";
			throw std::runtime_error(warn + err);
		}

//		if (!warn.empty())
//			std::cout << "WARN: " << warn << std::endl;
//
		std::vector<shared_ptr<object>> list;

		for (size_t i = 0; i < shapes.size(); ++i)
		{
			size_t index_offset = 0;

			assert(shapes[i].mesh.num_face_vertices.size() == shapes[i].mesh.material_ids.size());
			assert(shapes[i].mesh.num_face_vertices.size() == shapes[i].mesh.smoothing_group_ids.size());

			for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++)
			{
				size_t fnum = shapes[i].mesh.num_face_vertices[f];

				vertex vertices[3];

				for (size_t v = 0; v < fnum; v++)
				{
					tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];

					vec3 pos = vec3(static_cast<double>(attrib.vertices[idx.vertex_index * 3]),
									static_cast<double>(attrib.vertices[idx.vertex_index * 3 + 1]),
									static_cast<double>(attrib.vertices[idx.vertex_index * 3 + 2]));


					pos[0] *= scale[0];
					pos[1] *= scale[1];
					pos[2] *= scale[2];

					pos[0] += translate[0];
					pos[1] += translate[1];
					pos[2] += translate[2];

					vertices[v].pos = pos;
				}

				for (size_t v = 0; v < fnum; v++)
				{
					tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
					// todo: consider the cast of non-conforming scaling
					if (idx.normal_index >= 0)
					{
						vertices[v].normal = normalize(vec3(static_cast<double>(attrib.normals[idx.normal_index * 3]),
														 static_cast<double>(attrib.normals[idx.normal_index * 3 + 1]),
														 static_cast<double>(attrib.normals[idx.normal_index * 3 + 2])));
					}
					else
					{
						vertices[v].normal = normalize(cross(vertices[1].pos - vertices[0].pos,
															vertices[2].pos - vertices[0].pos));
					}

					if (idx.normal_index >= 0)
					{
						// u
						vertices[v].tex_coord[0] = static_cast<double>(attrib.texcoords[idx.texcoord_index * 2 + 0]);
						// v
						vertices[v].tex_coord[1] = static_cast<double>(attrib.texcoords[idx.texcoord_index * 2 + 1]);
					}
					for (int i = 0; i < 3; i++)
					{
						if (vertices[v].normal[i] == -0.0)
							vertices[v].normal[i] = 0.0;
					}
				}

				shared_ptr<triangle> tt = std::make_shared<triangle>(triangle(vertices[0], vertices[1], vertices[2], mt));

				triangles.emplace_back(tt);
				list.push_back(tt);

				index_offset += fnum;
			}
		}

		std::cout << "success load obj : " << path.c_str() << "\n";

		bvh_tree = bvh_node(list, 0, 0, bvh_node::SplitMethod::NAIVE);
	}
}

