#include "shape/model/mesh_triangle.h"
#include "utility/tiny_obj_loader.h"

#include <unordered_map>
#include <functional>

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

}

namespace shape::model
{
	mesh_triangle::mesh_triangle(const std::string& filename, shared_ptr<material> ptr)
	{
		load_obj(filename);
	}

	bool mesh_triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
	{
		return false;
	}

	bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const
	{
		return false;
	}

	double mesh_triangle::pdf_value(const point3 &o, const vec3 &v) const
	{
		return hittable::pdf_value(o, v);
	}

	vec3 mesh_triangle::random(const vec3 &o) const
	{
		return hittable::random(o);
	}

	mesh_triangle::~mesh_triangle()
	{
	}

	void mesh_triangle::load_obj(const std::string &path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		auto hash = [](const vertex& v){
			return hash_val(v.pos, v.normal, v.tex_coord);
		};

		auto eq = [](const vertex& v1, const vertex& v2){
			return v1.pos == v2.pos &&
				   v1.normal == v2.normal &&
				   v1.tex_coord == v2.tex_coord;
		};

		std::unordered_map<vertex, uint32_t, decltype(hash), decltype(eq)> uniqueVertices(1, hash, eq);

		for (const auto &shape : shapes)
		{
			size_t j = 0;
			size_t lastMaterialId = shape.mesh.material_ids[0];

		}
	}
}

