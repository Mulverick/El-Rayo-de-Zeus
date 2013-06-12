//---------------------------------------------------------------------------
// ParticleSystem.h
//---------------------------------------------------------------------------

/**
@file ParticleSystem.h

Contiene la declaraci�n de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#ifndef __Graphics_ParticleSystem_H
#define __Graphics_ParticleSystem_H

#include <HHFX/IHHFXPublic.h>

#include "SceneElement.h"


namespace Graphics 
{
	/**
	Clase de ParticleSysteme.
	
	@ingroup graphicsGroup

	@author FRS
	@date Mayo, 2013
	*/
	class CParticleSystem : public CSceneElement, public IHHFX::IFXListener
	{
	public:

		/**
		Constructor de la clase.		
		*/		
		// TODO FRS parametrizar o hacer enum de hfx
		CParticleSystem(const std::string& hfx, const std::string& parentName, const Vector3& relativePos = Vector3::ZERO);
			

		/*****************
			GET's & SET'S
		*******************/

		bool isVisible() const;
		void start(); // TODO Si solo tenemos start -> renombrar a RUN
		//void stop() const; UNDONE		


	private:		
		
		std::string _parentName;
		Vector3 _relativePos;	// Pos relativa respecto al padre (particulas son hijas de otra entidad gr�fica)
		

		//-------- SceneElement implementation --------------
		bool				load(); 
		void				unload();


		/*********************
			HELL HEAVENS FX		
		**********************/		
		Ogre::NameValuePairList _hhfxParams; // effect's params
		IHHFXOgre*				_movObj;	// Sistema de particulas (movable object)		
		IHHFXScene*				_hhfxScene; // HHFX Scene
	
		//--------- IHHFX::IFXListener implementation -------------------
		void				OnFXStarted(IHHFX *obj);
		void				OnFXStopped(IHHFX *obj);


	//	static bool					_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);

	}; // class CParticleSystem

} // namespace Graphics

#endif // __Graphics_ParticleSystem_H
