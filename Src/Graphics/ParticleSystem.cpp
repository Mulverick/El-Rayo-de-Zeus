//---------------------------------------------------------------------------
// ParticleSystem.cpp
//---------------------------------------------------------------------------

/**
@file ParticleSystem.cpp

Contiene la implementación de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#include "ParticleSystem.h"


#include <OgreCommon.h>
#include <OgreMovableObject.h>
#include <OgreSceneManager.h>

#include "Camera.h"
#include "Scene.h"
#include "Server.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::PARTICLE_SYSTEM>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{	

	CParticleSystem::CParticleSystem(const std::string& hfx, const std::string& parentName, const Vector3& relativePos) :
			_parentName(parentName), _relativePos(relativePos), _movObj(0), _hhfxScene(0) 
	{
		_hhfxParams["pack"] =  Graphics::CServer::getSingletonPtr()->getHHFXBase()->GetHHFXPackExplorer().GetPack();
		_hhfxParams["fx"]	=  Graphics::CServer::getSingletonPtr()->getHFXLongName(hfx);
		_hhfxParams["run"]	=  "yes";	// Por defecto: ejecución inmediata al crear el MO
	}

	//--------------------------------------------------------
	
	void CParticleSystem::start()
	{			
		// Orientacion initial rotada 180 hacia -X (cara frontal de nuestras entidades)
		static const Quaternion ORIENTATION(Ogre::Radian( Math::PI ), Vector3::UNIT_Y);

		if(_movObj) { // MO tiene que ser NULL; lo contrario significa que ya hay un FX en curso
			_movObj->RunFX(); // TODO Verificar que se liberan bien... queremos acumular N veces el mismo efecto?

		} else {
			// spawn a new effect at this location
			Ogre::MovableObject	*mo = getSceneMgr()->createMovableObject("HHFX", &_hhfxParams);
				assert(mo && "Error al crear ParticleSystem");	
			
			_node = getSceneMgr()->getSceneNode( _parentName + "_node") 
				 ->createChildSceneNode(_relativePos, ORIENTATION); 

			// set this class to listen to the ps, to be notified when it is destroyed.
			_movObj = static_cast<IHHFXOgre*>(mo);
				_movObj->SetFXListener(this);
#ifdef _DEBUG
				_movObj->setDebugDisplayEnabled(true);
#endif
				_node->attachObject(_movObj);
		}
	}

	//--------------------------------------------------------

	// UNDONE FRS: No se puede usar el método StopFX => Inconsistencias y pescaillas que se muerden la cola con evento StoppedFX
	void CParticleSystem::stop()
	{
		if(_movObj) {

			// TODO FRS

			//_movObj->setVisible(false);
			//_movObj->detachFromParent();
			//LOG("isAttached = "			<< _movObj->isAttached()			)
			//LOG("IsFXActive = "			<< _movObj->IsFXActive()			)
			//LOG("isInScene= "			<< _movObj->isInScene()			)
			//LOG("isParentTagPoint = "	<< _movObj->isParentTagPoint()	)
			//LOG("isVisible = "			<< _movObj->isVisible()			)

			//_movObj->SetFXListener(0);

			//_movObj->StopFX();
			//static_cast<Ogre::MovableObject*>(_movObj)->detachFromParent();		
			//getSceneMgr()->destroyMovableObject( static_cast<Ogre::MovableObject*>(_movObj) ); 
			//getSceneMgr()->destroySceneNode(_node);		
			//_movObj = 0;	
			//_node = 0;
		}

			// FRS Opciones fallutas
			//	_node->setVisible(false); 
			//	_movObj->StopFX();		
		
	
	}
		
	




	/**********************
		HHFX LISTENER
	***********************/

	// TODO revisar
	void CParticleSystem::OnFXStarted(IHHFX* obj)
	{
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");

		// create a light under the ElectricOrb effect
		// UNDONE FRS
		 if ( strstr(obj->GetPath(), "ElectricOrb.hfx") )
		{
			IHHFXOgre* fx = static_cast<IHHFXOgre*>(obj);	
			Ogre::Light* pointLight = getSceneMgr()->createLight("pointLight" + Ogre::StringConverter::toString((unsigned int)(obj)));
			pointLight->setType(Ogre::Light::LT_POINT);
			pointLight->setPosition(fx->getParentSceneNode()->getPosition() + Vector3::UNIT_Y * 5.0f);
			pointLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
			pointLight->setSpecularColour(0.8f, 0.8f, 1.0f);
			//pointLight->setAttenuation(32.0f, 1.0f, 0.14f, 0.07f);
		}
	}

	//--------------------------------------------------------

	// called when an effect stopped by itself or when the hhfx scene is cleared
	void CParticleSystem::OnFXStopped(IHHFX* obj)
	{		
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");
		
		// destroy the light created under ElectricOrb
		// UNDONE FRS
		if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
			getSceneMgr()->destroyLight("pointLight" + Ogre::StringConverter::toString((unsigned int)(obj)));

		if(_movObj) {
			_movObj->detachFromParent();
			getSceneMgr()->destroyMovableObject(_movObj); 
			getSceneMgr()->destroySceneNode(_node);		
			_movObj = 0;	
			_node = 0;
		}
	}

	


	/**********************
		SCENE ELEMENT
	***********************/

	bool CParticleSystem::load()
	{
		assert( getSceneMgr()->hasSceneNode( _parentName + "_node") );

		try{		
			_hhfxScene = _scene->getHHFXScene();
			_loaded = true;
			
		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;		
	} // load
	
	//--------------------------------------------------------
		
	void CParticleSystem::unload()
	{	
		CSceneElement::unload();	

		if(_movObj)	{		
			getSceneMgr()->destroyMovableObject(_movObj); 
			_movObj = 0;
		}
	} // unload

	


	
	
	/**********************
		GET's & SET's
	***********************/

	bool CParticleSystem::isVisible() const {
		assert(_loaded && "Imprescindible haber cargado el sistema de particulas en escena primero");
		return _movObj->isVisible();
	}

	
} // namespace Graphics

