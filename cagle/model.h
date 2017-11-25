#pragma once
#include <string>


namespace CAGLE {
	
	
	
	class Model {
	protected:
		int polygoncount;
		int polygoncolor;
		float* vertexdata;
		float* normaldata;
		float* uvdata;

	public:
		Model() : polygoncolor(0xFFFFFF) { }

		/** Parsing function*/
		void obj_loader(const std::string filename);
		void obj_loader2(const std::string filename);


		/** Get */
		const float* load_normal() { return normaldata; }
		const float* load_vertex() { return vertexdata; }
		const float* load_uv() { return uvdata; }
		const int	 load_polygon_num() { return polygoncount; }


		const int	 load_polygon_color() { return polygoncolor; }
		void		 set_polygon_color(const int c) { polygoncolor = c; }
	};


	class Null_Model : public Model {
	public:
		Null_Model()
		{
			polygoncount = 0;
			polygoncolor = 0x000000;
			vertexdata = new float[3];
			normaldata = new float[3];
			uvdata = new float[3];
			vertexdata[0] = 0; vertexdata[1] = 0; vertexdata[2] = 0;
			normaldata[0] = 0; normaldata[1] = 0; normaldata[2] = 0;
			uvdata[0] = 0; uvdata[1] = 0; uvdata[2] = 0;
		}
	};
}