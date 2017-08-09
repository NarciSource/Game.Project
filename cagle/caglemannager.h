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
/**	@Class : Management							**/
/*************************************************/



#pragma once

#include <vector>
#include <algorithm>

#include "cagle.h"

namespace CAGLE {
	/** forward Declaration */
	class Object;	
	class Camera;
	class Light;

	class Management {

		
	public:
		/***************** Object manage *******************************************************/
		/** Hire */
		Object* iWannaInternObject(void);


		Object* iWannaObject(void);


		Object* iWannaObject(const std::string internName);


		/** Call */
		Object* getObject(const std::string name);


		Object* getInternObject(const std::string name);


		/** Fire */
		void fireObject(const std::string name);

		void fireInternObject(const std::string name);




		/** Collision Test **/
		bool isCollision(const std::string name1, const std::string name2);
		bool isCollision(const std::string name1, const std::string name2, const int pivot);


		/** Call All */
		std::vector<Object*> getAllobjects();
		/***************************************************************************************/







		/***************** Camera manage *******************************************************/
		/** Hire */
		Camera* iWannaCamera(void);


		/** Call */
		Camera* getCamera(void);
		/***************************************************************************************/







		/***************** Light manage ********************************************************/
		/** Hire */
		Light* iWannaLight(void);


		/** Call */
		Light* getLight(void);
		/***************************************************************************************/



		
		


		void refresh(void);





	private:
		/** The intern Objects means base object
		* It do not draw to world.
		* Programmer hire object to base intern object
		*/
		std::vector<Object*> internObjects;


		std::vector<Object*> objects;


		Camera* onlyCamera;

		Light* onlyLight;
	};

}
