#include "ground.h"

namespace CAGLE {

	float Ground::Height(const float x, const float z) {

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
			ha = static_cast<Terrain*>(model)->Height(a.X(), a.Z());
			hb = static_cast<Terrain*>(model)->Height(b.X(), b.Z());
			hc = static_cast<Terrain*>(model)->Height(c.X(), c.Z());
			hd = hb + hc - ha;
		}
		else {
			hb = static_cast<Terrain*>(model)->Height(b.X(), b.Z());
			hc = static_cast<Terrain*>(model)->Height(c.X(), c.Z());
			hd = static_cast<Terrain*>(model)->Height(d.X(), d.Z());
			ha = hb + hc - hd;
		}

		w_p = r_hat.X() - static_cast<int>(r_hat.X());
		w_q = 1 - w_p;
		l_p = r_hat.Z() - static_cast<int>(r_hat.Z());
		l_q = 1 - l_p;

		answer = (ha*w_q + hb*w_p)*l_q +
			(hc*w_q + hd*w_p)*l_p;

		return answer*Size() + Position().Y();
	}


	const float* Ground::Color() {
		static float color[4] = {
			1.f*(colorv / 0x10000) / 0xFF,
			1.f*(colorv / 0x100 % 0x100) / 0xFF,
			1.f*(colorv % 0x100) / 0xFF,
			0.f
		};
		return color;
	}
}