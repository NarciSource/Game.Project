/*************************************************/
/** @CAGLE Mannager								**/
/**   ==Chung Ang university GL Engine			**/
/**												**/
/** @Version : v1.2								**/
/** @Author : Jeong Won Cheol					**/
/** @Contact : jwch11@gmail.com					**/
/**												**/
/** @Create : 30th May 2017						**/
/** @Last Modified: 5th June 2017				**/
/**												**/
/**	@Class : ResourceManager							**/
/*************************************************/



#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <utility>

#include "model.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "ground.h"
#include "sky.h"
#include "terrain.h"
#include "texture.h"

namespace CAGLE {
	/** forward Declaration */
	class Object;	
	class Camera;
	class Light;
	class Player;
	class Texture;

	class ResourceManager {
	public:
		static ResourceManager& getInstance()
		{
			static ResourceManager instance;
			return instance;
		}
		
		enum class Type {
			Object, Ground, Sky
		};
	public:
		/***************** Object manage *******************************************************/

		Object& newObject(const std::string name, const Type type);

		Object* getObject(const std::string name);

		bool copyObject(const std::string dst, const std::string src, const int num=1);

		void deleteObject(const std::string name);
		/***************************************************************************************/

		Model* newModel(const std::string filename, const Type type);

		Texture* newTexture(const std::string filename);
		
		bool loadModel(const std::string name, const std::string filename);



		/***************** Player manage *******************************************************/

		Player& newPlayer(const std::string name);

		Player* getPlayer(const std::string name);
		/***************************************************************************************/






		/** Collision Test **/
		bool isCollision(const std::string name1, const std::string name2);
		bool isCollision(const std::string name1, const std::string name2, const int pivot);


		/** Call All */
		const std::map<std::string, Object*> get_all_objects();

		
		/***************************************************************************************/







		/***************** Camera manage *******************************************************/
		/** Hire */
		Camera& newCamera(const std::string name);


		/** Call */
		Camera* getCamera(const std::string name);

		const float* getCameraPosition(const std::string name);
		/***************************************************************************************/







		/***************** Light manage ********************************************************/
		/** Hire */
		Light* iWannaLight(void);


		/** Call */
		Light* getLight(void);

		const float* getLightPosition(void);
		/***************************************************************************************/



		
		void refresh(void);





	private:
		ResourceManager() {}
		~ResourceManager() {}

		/** The intern Objects means base object
		* It do not draw to world.
		* Programmer hire object to base intern object
		*/

		std::map<std::string, Object*> objects;

		std::map<std::string, Model*> models;

		std::map<std::string, Texture*> textures;


		std::map<std::string, Player*> players;

		std::map<std::string, Camera*> cameras;
		
		Light* onlyLight;


		inline bool const hasObjects(const std::string name);
	};

}
