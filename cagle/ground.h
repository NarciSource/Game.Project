#pragma once
#include "mvp.h"
#include "terrain.h"
#include "object.h"


namespace CAGLE {
	class Ground : public Object
	{
	private:
		Terrain* terrain;

	public:
		Ground() :Object() { refresh(); }

		void bind(Terrain* trr) { terrain = trr; }

		const float* Normals() { return terrain->Normals(); }
		const float* Vertexs() { return terrain->Vertexs(); }
		const int indices_size() { return terrain->indices_size(); }
		const int* Indices() { return terrain->Indices(); }
		

		CAGLM::Vec3<float>	Normal(const int x, const int y) {
			return terrain->Normal(x,y);
		}

		float Height(const float x, const float z) { 
			
			/**
			Match the world coordinate system to the terrain coordinate system */			
			CAGLM::Vec3<float> r(x, 0, z);

			CAGLM::Mat4 s; s.scalef(1 / Size());
			CAGLM::Mat4 t; t.translatef(-Position().X(), 0, -Position().Z());

			CAGLM::Vec3<float> r_hat = s*(t*r);


			/**
			Interpolation */
			CAGLM::Vec3<int> a, b, c, d;
			float ha, hb, hc, hd;
			float w_p, w_q, l_p, l_q;
			float answer;


			a = CAGLM::Vec3<int>(static_cast<int>(r_hat.X()), 0, static_cast<int>(r_hat.Z()));
			b = CAGLM::Vec3<int>(static_cast<int>(r_hat.X() + 1), 0, static_cast<int>(r_hat.Z()));
			c = CAGLM::Vec3<int>(static_cast<int>(r_hat.X()), 0, static_cast<int>(r_hat.Z() + 1));
			d = CAGLM::Vec3<int>(static_cast<int>(r_hat.X() + 1), 0, static_cast<int>(r_hat.Z() + 1));

			if ((r_hat.X() - static_cast<int>(r_hat.X())) + (r_hat.Z() - static_cast<int>(r_hat.Z())) <= 1.0f)
			{
				ha = terrain->Height(a.X(), a.Z());
				hb = terrain->Height(b.X(), b.Z());
				hc = terrain->Height(c.X(), c.Z());
				hd = hb + hc - ha;
			}
			else {				
				hb = terrain->Height(b.X(), b.Z());
				hc = terrain->Height(c.X(), c.Z());
				hd = terrain->Height(d.X(), d.Z());
				ha = hb + hc - hd;
			}

			w_p = r_hat.X() - static_cast<int>(r_hat.X());
			w_q = 1 - w_p;
			l_p = r_hat.Z() - static_cast<int>(r_hat.Z());
			l_q = 1 - l_p;

			answer = (ha*w_q + hb*w_p)*l_q +
				(hc*w_q + hd*w_p)*l_p;
			
			return answer*Size()+Position().Y();
		}




	};



}