#include "shape/model/mesh_triangle.h"
#include "utility/tiny_obj_loader.h"
#include "utility/OBJ_Loader.hpp"

#include <functional>
#include <unordered_map>
#include <cassert>
#include <array>

namespace
{
	//from boost (functional/hash):
	//see http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html
	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& val){
		seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

	//auxiliary generic functions to create a hash value using a seed
	template <typename T>
	inline void hash_val(std::size_t& seed, const T& val){
		hash_combine(seed, val);
	}
	template <typename T, typename... Types>
	inline void hash_val(std::size_t& seed, const T& val, const Types&... args){
		hash_combine(seed, val);
		hash_val(seed, args...);
	}

	//auxialiary generic function to create a has value out of a heterogeneous list of arguments
	template <typename... Types>
	inline std::size_t hash_val(const Types&... args){
		std::size_t seed = 0;
		hash_val(seed, args...);
		return seed;
	}

	class Hasher {
	public:
		size_t operator()(const vertex& v) const {
			//calculate hash here.
			return hash_val(
				v.pos.x(), v.pos.y(), v.pos.z(),
				v.normal.x(), v.normal.y(), v.normal.z(),
				v.tex_coord.x(), v.tex_coord.y());
		}
	};
	class Equal
	{
	public:
		bool operator()(const vertex &v1, const vertex &v2) const
		{
			bool rtval = false;
			for (int i = 0; i < 3; ++i)
			{
				rtval |= (v1.pos[i] == v2.pos[i]);
				rtval |= (v1.normal[i] == v2.normal[i]);
				if (i < 2)
					rtval |= (v1.tex_coord[i] == v2.tex_coord[i]);
			}
			return rtval;
		}
	};
}

namespace shape::model
{
	mesh_triangle::mesh_triangle(const std::string& filename, shared_ptr<material> ptr, vec3 translate, vec3 scale)
	{
		load_obj(filename, ptr, translate, scale);
	}

	bool mesh_triangle::hit(const ray &r_in, double t_min, double t_max, hit_record &rec) const
	{
		bool intersect = false;
		for (auto &tri : triangles)
		{
			if (tri.hit(r_in, t_min, t_max, rec))
			{
				intersect = true;
			}
		}
		return intersect;
	}

	bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		output_box = mesh_box;
		return false;
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

				vert.e[0] *= model[0];
				vert.e[1] *= model[5];
				vert.e[2] *= model[10];

				vert.e[0] += model[3];
				vert.e[1] += model[7];
				vert.e[2] += model[11];

				face_vertices[j] = vert;
				min_vert = vec3(std::min(min_vert.x(), vert.x()),
								std::min(min_vert.y(), vert.y()),
								std::min(min_vert.z(), vert.z()));
				max_vert = vec3(std::max(max_vert.x(), vert.x()),
								std::max(max_vert.y(), vert.y()),
								std::max(max_vert.z(), vert.z()));

				triangles.emplace_back(triangle(face_vertices[0], face_vertices[1],
												face_vertices[2], mt));
			}
		}

		mesh_box = aabb(min_vert, max_vert);

		for (auto &tri : triangles)
			area += tri.get_area();
	}
}

