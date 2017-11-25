#include <regex>
#include <vector>
#include <iostream>
#include <fstream>

#include "mat4.h"
#include "model.h"


namespace CAGLE {

	void Model::obj_loader(const std::string filename)
	{
		std::ifstream fin(filename);
		if (!fin.is_open()) {
			std::cerr << "no file: " << filename << std::endl;
			return;
		}
		else {
			std::clog << "file open: " << filename << std::endl;
		}

		std::vector<CAGLM::Vec3<float>> localVertices;
		std::vector<CAGLM::Vec3<float>> localUvs;
		std::vector<CAGLM::Vec3<float>> localNormals;
		std::vector<unsigned int> localVertexIndices;
		std::vector<unsigned int> localUvIndices;
		std::vector<unsigned int> localNormalIndices;

		std::string line;
		const static std::regex vPattern{ R"(v\s+(-?\d+.?\d*)\s(-?\d+.?\d*)\s(-?\d+.?\d*))" }; // (x y z)
		const static std::regex vtPattern{ R"(vt\s(-?\d+.?\d*)\s(-?\d+.?\d*)(?:\s(-?\d+.?\d*)){0,1})" }; // (x y z) or (x y)
		const static std::regex vnPattern{ R"(vn\s(-?\d+.?\d*)\s(-?\d+.?\d*)\s(-?\d+.?\d*))" }; // (x y z)
		const static std::regex fPattern{ R"(f\s(\d+)/(\d+)*/(\d+)\s(\d+)/(\d+)*/(\d+)\s(\d+)/(\d+)*/(\d+))" }; //(x1/y1/z1 x2/y2/z2 x3/y3/z3) or (x1//z1 x2//z2 x3//z3)
		std::smatch m;

		polygoncount = 0;
		while (std::getline(fin, line))
		{
			std::regex_search(line, m, vPattern);
			if (m[0].matched)
			{
				localVertices.push_back(CAGLM::Vec3<float>(std::stof(m[1]), std::stof(m[2]), std::stof(m[3])));
				continue;
			}

			std::regex_search(line, m, vtPattern);
			if (m[0].matched)
			{
				if (m[3].matched)
					localUvs.push_back(CAGLM::Vec3<float>(std::stof(m[1]), std::stof(m[2]), std::stof(m[3])));
				else
					localUvs.push_back(CAGLM::Vec3<float>(std::stof(m[1]), std::stof(m[2]), 0.f));
				continue;
			}

			std::regex_search(line, m, vnPattern);
			if (m[0].matched)
			{
				localNormals.push_back(CAGLM::Vec3<float>(std::stof(m[1]), std::stof(m[2]), std::stof(m[3])));
				continue;
			}

			std::regex_search(line, m, fPattern);
			if (m[0].matched)
			{
				localVertexIndices.push_back(std::stoi(m[1]) - 1);
				localVertexIndices.push_back(std::stoi(m[4]) - 1);
				localVertexIndices.push_back(std::stoi(m[7]) - 1);

				if (m[2].matched)
				{
					localUvIndices.push_back(std::stoi(m[2]) - 1);
					localUvIndices.push_back(std::stoi(m[5]) - 1);
					localUvIndices.push_back(std::stoi(m[8]) - 1);
				}
				else {
					localUvIndices.push_back(0);
					localUvIndices.push_back(0);
					localUvIndices.push_back(0);
				}
				localNormalIndices.push_back(std::stoi(m[3]) - 1);
				localNormalIndices.push_back(std::stoi(m[6]) - 1);
				localNormalIndices.push_back(std::stoi(m[9]) - 1);

				polygoncount++;
				continue;
			}
		}

		vertexdata = new float[polygoncount * 3 * 3];
		normaldata = new float[polygoncount * 3 * 3];
		if (!localUvs.empty())
			uvdata = new float[polygoncount * 3 * 2];
		for (int i = 0; i < this->polygoncount * 3; i++)
		{
			vertexdata[i * 3 + 0] = localVertices[localVertexIndices[i]].X();
			vertexdata[i * 3 + 1] = localVertices[localVertexIndices[i]].Y();
			vertexdata[i * 3 + 2] = localVertices[localVertexIndices[i]].Z();

			normaldata[i * 3 + 0] = localNormals[localNormalIndices[i]].X();
			normaldata[i * 3 + 1] = localNormals[localNormalIndices[i]].Y();
			normaldata[i * 3 + 2] = localNormals[localNormalIndices[i]].Z();

			if (!localUvs.empty())
			{
				uvdata[i * 2 + 0] = localUvs[localUvIndices[i]].X();
				uvdata[i * 2 + 1] = localUvs[localUvIndices[i]].Y();
			}
		}
	}

	void Model::obj_loader2(const std::string filename)
	{	/** Data parsing */
		std::ifstream fin(filename);
		if (!fin.is_open()) {
			std::cerr << "no file: " << filename << std::endl;
			return;
		}
		else {
			std::clog << "file open: " << filename << std::endl;
		}

		std::vector<CAGLM::Vec3<float>> localVertices;
		std::vector<CAGLM::Vec3<float>> localNormals;
		std::vector<CAGLM::Vec3<int>> localIndices;

		std::string token;
		int vertexcount;


		fin >> token >> token >> vertexcount;

		localVertices.assign(vertexcount, CAGLM::Vec3<float>(0, 0, 0));
		for (int i = 0; i < vertexcount; i++)
		{
			float x, y, z;
			fin >> x >> y >> z;
			localVertices[i] = CAGLM::Vec3<float>(x / 100, y / 100, z / 100); // size of magic number <-want to modification
		}

		fin >> token >> token >> this->polygoncount;

		localIndices.assign(polygoncount, CAGLM::Vec3<int>(0, 0, 0));
		for (int i = 0; i < polygoncount; i++)
		{
			int x, y, z;
			fin >> x >> y >> z;
			localIndices[i] = CAGLM::Vec3<int>(x, y, z);
		}




		/** !!
		****** Calcurate Normal Vector    ******************
		* It uses cross product to calcurate a normal vector.
		* The normal vector of each vertex is calculated
		* by weighting the angle of the surrounding plane.
		*****************************************************/
		localNormals.assign(vertexcount, CAGLM::Vec3<float>(0, 0, 0));
		for (int i = 0; i < polygoncount; i++)
		{
			CAGLM::Vec3<float> p1, p2, p3;
			CAGLM::Vec3<float> v1, v2, normalv;
			float angle;


			/** Three points on which to calcurate normal vector */
			p1 = localVertices[localIndices[i].X()];
			p2 = localVertices[localIndices[i].Y()];
			p3 = localVertices[localIndices[i].Z()];



			/** Normal Vector of first point
			* This multiplies theta by weight.	*/
			v1 = p2 - p1;	v2 = p3 - p1;
			normalv = CAGLM::Vec3<float>::Cross(v1, v2);
			angle = CAGLM::Vec3<float>::Angle(v1, v2);
			localNormals[localIndices[i].X()] += normalv * angle;

			/* Second point */
			v1 = p3 - p2;	v2 = p1 - p2;
			normalv = CAGLM::Vec3<float>::Cross(v1, v2);
			angle = CAGLM::Vec3<float>::Angle(v1, v2);
			localNormals[localIndices[i].Y()] += normalv * angle;

			/* Third point */
			v1 = p1 - p3;	v2 = p2 - p3;
			normalv = CAGLM::Vec3<float>::Cross(v1, v2);
			angle = CAGLM::Vec3<float>::Angle(v1, v2);
			localNormals[localIndices[i].Z()] += normalv * angle;
		}
		for (auto &localNormal : localNormals)
		{
			localNormal = CAGLM::Vec3<float>::Normalize(localNormal);
		}





		vertexdata = new float[polygoncount * 3 * 3];
		normaldata = new float[polygoncount * 3 * 3];
		for (int i = 0; i < this->polygoncount; i++)
		{
			vertexdata[i * 9 + 0] = localVertices[localIndices[i].X()].X();
			vertexdata[i * 9 + 1] = localVertices[localIndices[i].X()].Y();
			vertexdata[i * 9 + 2] = localVertices[localIndices[i].X()].Z();

			vertexdata[i * 9 + 3] = localVertices[localIndices[i].Y()].X();
			vertexdata[i * 9 + 4] = localVertices[localIndices[i].Y()].Y();
			vertexdata[i * 9 + 5] = localVertices[localIndices[i].Y()].Z();

			vertexdata[i * 9 + 6] = localVertices[localIndices[i].Z()].X();
			vertexdata[i * 9 + 7] = localVertices[localIndices[i].Z()].Y();
			vertexdata[i * 9 + 8] = localVertices[localIndices[i].Z()].Z();


			normaldata[i * 9 + 0] = localNormals[localIndices[i].X()].X();
			normaldata[i * 9 + 1] = localNormals[localIndices[i].X()].Y();
			normaldata[i * 9 + 2] = localNormals[localIndices[i].X()].Z();

			normaldata[i * 9 + 3] = localNormals[localIndices[i].Y()].X();
			normaldata[i * 9 + 4] = localNormals[localIndices[i].Y()].Y();
			normaldata[i * 9 + 5] = localNormals[localIndices[i].Y()].Z();

			normaldata[i * 9 + 6] = localNormals[localIndices[i].Z()].X();
			normaldata[i * 9 + 7] = localNormals[localIndices[i].Z()].Y();
			normaldata[i * 9 + 8] = localNormals[localIndices[i].Z()].Z();
		}
	}
}