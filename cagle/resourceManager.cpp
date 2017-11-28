/*************************************************/
/** @CAGLE Mannager								**/
/**   ==Chung Ang university GL Engine			**/
/*************************************************/

#include <iostream>
#include <fstream>

#include "resourceManager.h"

namespace CAGLE {

	Object& ResourceManager::newObject(const std::string name, const Type type)
	{
		auto ret = objects.insert({ name,nullptr });
		if (ret.second)
		{
			switch (type)
			{
			case Type::Ground:
				ret.first->second = new Ground(name);
				break;
			case Type::Sky:
				ret.first->second = new Sky(name);
				break;
			default:
				ret.first->second = new Object(name);
			}
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *objects[name];
	}

	Player& ResourceManager::newPlayer(const std::string name)
	{
		auto ret = players.insert({ name,nullptr });
		if (ret.second)
		{
			ret.first->second = new Player();
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *players[name];
	}

	void ResourceManager::adjustment()
	{
		Ground* ground = getGround("ground1");
		Player* player = getPlayer("player1");

		CAGLM::Vec3<float> position = player->Position();

		float height = ground->Height(position.X(), position.Z()) + player->Tall();

		position.Y(height);

		player->Position(position);
	}

	Controller& ResourceManager::newController(const std::string name, Player* player)
	{
		auto ret = hids.insert({ name,nullptr });
		if (ret.second)
		{
			ret.first->second = new Controller(hids.size(), player);
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *(static_cast<Controller*>(hids[name]));
	}
	Controller* ResourceManager::getController(const std::string name)
	{
		return static_cast<Controller*>(hids[name]);
	}

	Keyboard& ResourceManager::newKeyboard(const std::string name, Player* player)
	{
		auto ret = hids.insert({ name,nullptr });
		if (ret.second)
		{
			ret.first->second = new Keyboard(player);
		}
		else { // overlap			
			std::cerr << "Already exist " << name << std::endl;
		}
		return *(static_cast<Keyboard*>(hids[name]));
	}
	Keyboard* ResourceManager::getKeyboard(const std::string name)
	{
		return static_cast<Keyboard*>(hids[name]);
	}



	Model* ResourceManager::newModel(const std::string filename, const Type type)
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
			switch (type)
			{
			case Type::Ground:
				ret.first->second = new Terrain();
				ret.first->second->loader(filename);
				break;
			default:
				ret.first->second = new Model();
				ret.first->second->loader(filename);
			}
		}
		return models[filename];
	}

	Texture* ResourceManager::newTexture(const std::string filename)
	{
		std::ifstream in(filename);
		if (!in.good())
		{
			std::cerr << filename << " doesn't exist" << std::endl;
			in.close();
		//	return &Null_Model();
		}
		in.close();

		/** model make */
		auto ret = textures.insert({ filename,nullptr });
		if (ret.second)
		{
			ret.first->second = new Texture();
			ret.first->second->texture_loader(filename);
		}
		return textures[filename];
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

	Ground* ResourceManager::getGround(const std::string name)
	{
		return static_cast<Ground*>(getObject(name));
	}

	Player* ResourceManager::getPlayer(const std::string name)
	{
		if (players.find(name) != players.end())
		{
			return players[name];
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
			ret.first->second->loader(filename);
		}

		/** set */
		objects[name]->bind(models[filename]);

		return true;
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

	const std::map<std::string, Object*> ResourceManager::get_all_objects()
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

	const float* ResourceManager::getCameraPosition(const std::string name)
	{
		return getCamera(name)->Position().get_area();
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

	const float* ResourceManager::getLightPosition(void)
	{
		return getLight()->Position().get_area();
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