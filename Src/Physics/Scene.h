/**
@file Scene.h

Contiene la declaraci�n de la clase contenedora de los elementos
de una escena.

@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#ifndef __Physics_Scene_H
#define __Physics_Scene_H

//HACK TOCHO ?

#include <vector>

#include "Physics/Actor.h"
#include "Physics/ActorTrigger.h"


namespace Physics 
{

	class CScene 
	{

	public:

		typedef std::vector<CActor*>		TColliders;
		typedef std::vector<CActorTrigger*> TTriggers;

		/************
			ACTORS
		*************/
		bool addActor(CActor *actor);
		bool addActor(CActorTrigger* actor);
		//bool addStaticActor(Physics::CStaticActor *actor);

		void removeActor(CActor* actor);
		void removeActor(CActorTrigger* actor);
		//void removeStaticActor(CStaticActor* actor);

	protected:

		/**
		Clase amiga. Solo el servidor f�sico puede crear o liberar escenas f�sicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	Constructor de la clase.	*/
		CScene(const std::string& name) : _name(name) {};

		/**
		Destructor de la aplicaci�n.
		*/
		~CScene();

		/**	Despierta la escena*/
		bool activate();

		/**	Duerme la escena*/
		bool deactivate();
		void tick(unsigned int);


		/******************
			GET's & SET's
		********************/
		/**
		Devuelve el nombre de la escena.
		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}


	private:

		/**	Nombre de la escena.*/
		std::string _name;
		TColliders	_colliders;
		TTriggers	_triggers;

		// Componentes de la simulacion
		void checkCollisions();
		void checkTriggers();


		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate();

		/**
		Corrige la posici�n de 2 actores que colisionan.
		*/
		void resolveCollision(Physics::CActor *actor1, Physics::CActor *actor2, float overlapX, float overlapY);		

		

	}; // class CScene

} // namespace Physics

#endif 