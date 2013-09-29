/**
@file Physics.h

Contiene la declaraci�n del componente encargado de representar entidades f�sicas simples.
Este componente no sirve para representar physic characters.

@see Logic::CPhysics
@see Logic::IComponent
@see Logic::CPhysicCharacter
@see Physics::IObserver

@author FRS
@date 23-02-13
*/

#ifndef __Logic_Physics_H
#define __Logic_Physics_H

#include "Logic/Entity/Component.h"
#include "Physics/IObserver.h"


// Predeclaraci�n de tipos
namespace Physics {
	class CScene;
	class CActor;
}

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representaci�n f�sica de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un �nico actor.
	Esta clase no sirve para representar physic character ni entidades compuestas como ragdolls.
	<p>
	Existen dos tipos de entidades f�sicas: est�ticas y din�micas. Las entidades 
	est�ticas no se pueden mover una vez creadas y s�lo sirven como vol�menes de colisi�n.
	Las entidades din�micas tienen masa y se mueven aplic�ndoles fuerzas (por ejemplo la gravedad). 
	Tambi�n existe un tipo especial de entidades din�micas llamadas cinem�ticas, que est�n pensadas 
	para representar objetos cuyo movimiento viene dictado desde la l�gica del juego. Las entidades 
	cinem�ticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	En cada tick, este componente actualiza la posici�n y rotaci�n de las entidades l�gicas usando
	la informaci�n que proporciona el motor de f�sica (s�lo entidades din�micas). 
	<p>
	Cuando este componente se utiliza para representar una entidad cinem�tica, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad f�sica de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author FRS
	@date 23-02-13
	*/
	class CPhysics : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysics);

	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysics::CPhysics() : 
		  IComponent(GetAltTypeIdOf(CPhysics)), _physicalActor(0), _isTrigger(false), _radius(0), _halfWidth(0), _halfHeight(0),
				_density(0), _friction(0), _restitution(0),_attackPower(0) {}
		CPhysics::CPhysics(altTypeId id) : 
		  IComponent(id),  _physicalActor(0), _isTrigger(false), _radius(0), _halfWidth(0), _halfHeight(0), 
				_density(0), _friction(0), _restitution(0) {}

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysics();
		
		/**	Inicializa el componente usando los atributos definidos en el fichero de mapa.*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual	bool spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo, CEntity* father);
		virtual bool accept(const CMessage *message) { return false; }
		virtual void process(CMessage *message) {}
		virtual void tick(unsigned int msecs) {}

		virtual void detachFromMap();
		virtual void attachToMap(CMap* map);

		void disable();
		void enable();

		virtual void disableCollisions();
		virtual void enableCollisions();

 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		virtual void onTrigger(IObserver* other, bool enter);
		virtual void onCollision(IObserver* other);

		Physics::CActor* getPhysicalActor();


	protected:

		Physics::CScene* _scene; // Servidor de f�sica
		Physics::CActor* _physicalActor; // Actor que representa la entidad f�sica

		bool _isTrigger;
		std::string _shape;

		float _radius, _halfWidth, _halfHeight;
		float _density, _friction, _restitution;

		/**
		Atributo que indica la magnitud de velocidad de la flecha.
		*/
		float _angularProjectileSpeed;

		// Crea el actor que representa la entidad f�sica a partir de la informaci�n del mapa.*/
		virtual Physics::CActor* createActor(const Map::CEntity* entityInfo);
		// TODO FRS Podr�a pasar a llamarse physicEntity (por paralelismos con CGraphics)

		CEntity* _father;

		float _attackPower;
		
		float _negativeYVelocity;
	
	}; // class CPhysics

	REG_FACTORY(CPhysics);

} // namespace Logic

#endif // __Logic_Physics_H
