//---------------------------------------------------------------------------
// Light.h
//---------------------------------------------------------------------------

/**
@file Light.h

Contiene la declaración de la clase que maneja una luz

@see Graphics::CLight

@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Graphics_Light_H
#define __Graphics_Light_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Light;
};

namespace Graphics 
{
	class CScene;
	class CServer;
};

namespace Graphics 
{

	class CLight
	{
	public:

		/**
		Constructor de la clase.
		*/
		CLight(const std::string &name, CScene* scene);

		/**
		Destructor de la luz.
		*/
		~CLight();

		/*
		*/
		void setType(const std::string &type);
		
		/**
		*/
		void setPosition(const Vector3& position);

		/**
		*/
		void setDirection(const Vector3& position);

		/**
		*/
		void setDiffuseColour(float r, float g, float b);

		/**
		*/
		void setSpecularColour(float r, float g, float b);
		
		/**
		*/
		void setVisible(bool visible);

		/**
		Aunque las luces no tengan una representacion visual, 
		hacerlas visibles/invisibles significa que no afecta
		*/
		const bool isVisible();

		/**
		*/
		Ogre::Light* getLight();

	protected:

		friend class CScene;

		/** 
		*/
		Ogre::Light* _light;

		/**
		*/
		CScene* _scene;	

		/**
		*/
		bool load();

		/**
		*/
		void unload();

		/**
		*/
		virtual void tick(float secs);

	private:

		/**
		*/
		std::string _name;

		/**
		*/
		bool _loaded;


	}; // class CLight

}; // namespace Graphics

#endif // __Graphics_Light_H
