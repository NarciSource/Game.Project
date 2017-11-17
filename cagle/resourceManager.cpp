/*************************************************/
/** @CAGLE Mannager								**/
/**   ==Chung Ang university GL Engine			**/
/*************************************************/

#include <iostream>
#include <fstream>

#include "resourceManager.h"

namespace CAGLE {

	Object& ResourceManager::newObject(const std::string name)
	{
		auto ret = objects.insert({ name,nullptr });
		if (ret.second)
		{
			ret.first->second = new Object();
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *objects[name];
	}

	Model* ResourceManager::newModel(const std::string filename)
	{
		std::ifstream in(filename);
		if (!in.good())
		{
			std::cerr << filename << " doesn't exist" << std::endl;
			in.close();
			return &Null_Model();
		}
		in.close();

		/** model make */
		auto ret = models.insert({ filename,nullptr });
		if (ret.second)
		{
			ret.first->second = new Model();
			ret.first->second->obj_loader(filename);
		}
		return models[filename];
	}

	Object* ResourceManager::getObject(const std::string name)
	{
		if (objects.find(name) != objects.end())
		{
			return objects[name];
		}
		else {
			return nullptr;
		}
	}

	bool ResourceManager::copyObject(const std::string dst, const std::string src, const int num)
	{
		Object* src_object = getObject(src);
		if (src_object == nullptr)
		{
			std::cout << "Copy error, src isn't exist" << std::endl;
			return false;
		}

		for (int i = 0; i < num; i++)
		{
			std::string each_name;
			if (num == 1)
			{
				each_name = dst;
			}
			else {
				each_name = dst + std::to_string(i);
			}

			auto ret = objects.insert({ each_name,nullptr });
			if (ret.second)
			{
				ret.first->second = new Object(*src_object);
			}
			else {
				std::cout << "Copy error, dst is exist" << std::endl;
				return false;
			}
		}
		return true;
	}
	
	void ResourceManager::deleteObject(const std::string name)
	{
		auto it = objects.find(name);
		if (it != objects.end())
		{
			delete it->second;
			objects.erase(it);
		}
	}

	bool const ResourceManager::hasObjects(const std::string name)
	{
		return objects.find(name) != objects.end();
	}


	bool ResourceManager::loadModel(const std::string name, const std::string filename)
	{
		/** chk */
		if (!hasObjects(name))
		{
			std::cerr << "Objects hasn't "<< name << std::endl;
			return false;
		}

		std::ifstream in(filename);
		if (!in.good())
		{
			std::cerr << filename << " doesn't exist" << std::endl;
			in.close();
			return false;
		}
		in.close();


		/** model make */
		auto ret = models.insert({ filename,nullptr });
		if (ret.second)
		{
			ret.first->second = new Model();
			ret.first->second->obj_loader(filename);
		}

		/** set */
		objects[name]->bind(models[filename]);

		return true;
	}






	bool ResourceManager::getPlayer()
	{
		if (player == nullptr)
		{
			player = new Player();
		}

		return player;
	}


	bool ResourceManager::isCollision(const std::string name1, const std::string name2)
	{
		Object* object1 = getObject(name1);
		Object* object2 = getObject(name2);

		if( (object1->Position() - object2->Position()).length() <10 )
			return true;
		else return false;
	}

	bool ResourceManager::isCollision(const std::string name1, const std::string name2, const int pivot)
	{
		Object* object1 = getObject(name1);
		Object* object2 = getObject(name2);

		if ((object1->Position() - object2->Position()).length() <pivot)
			return true;
		else return false;
	}

	std::map<std::string, Object*> ResourceManager::get_all_objects()
	{
		return objects;
	}


	

	Camera& ResourceManager::newCamera(const std::string name)
	{
		auto ret = cameras.insert({ name,nullptr });
		if (ret.second)
		{
			ret.first->second = new Camera();
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *cameras[name];
	}


	Camera* ResourceManager::getCamera(const std::string name)
	{
		if (cameras.find(name) != cameras.end())
		{
			return cameras[name];
		}
		else {
			return nullptr;
		}
	}




	Light* ResourceManager::iWannaLight(void)
	{
		onlyLight = new Light;
		return onlyLight;
	}


	Light* ResourceManager::getLight(void)
	{
		return onlyLight;
	}



	Terrain& ResourceManager::newTerrain(const std::string filename)
	{
		terrain = Terrain::load_terrain(filename);
		return *terrain;
	}

	Terrain* ResourceManager::getTerrain()
	{
		return terrain;
	}

	void ResourceManager::deleteTerrain()
	{
		delete terrain;
	}


	void ResourceManager::refresh(void)
	{
		for (auto const& each : objects)
		{
			each.second->refresh();
		}
		getCamera("camera1")->shutter();
	
	}
	
}