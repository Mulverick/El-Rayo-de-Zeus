/**
@file AltarStateSwitcher.h

Contiene la declaración del componente que controla la
capacidad de un Character de activar/desactivar altares

@see Logic::CAltarStateSwitcher


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_AltarStateSwitcher_H
#define __Logic_AltarStateSwitcher_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
	class CAltarStateSwitcher : public IComponent
	{
		DEC_FACTORY(CAltarStateSwitcher);
	public:


		CAltarStateSwitcher() : IComponent(GetAltTypeIdOf(CAltarStateSwitcher)), _switchingState(false), 
			_switchingAllowed(false), _target(0), _targetSense(Logic::LogicalPosition::UNDEFINED), 
			_acumRotation(0.0f), _turnSpeedFactor(0.0f) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		virtual void process(CMessage *message);

		/************************
			UTILITY FUNCTIONS
		*************************/

		void rotate(float yawAmount, unsigned int msecs);
		void startSwitchingState();
		void stopSwitchingState();

		CEntity* getTarget(){return _target;};

		void checkTriggerMessage(CMessage *message);
		void checkControlMessage(CMessage *message);
		void processAltarSwitched();

	protected:

		/**
		*/
		bool _switchingState;

		/**
		*/
		bool _switchingAllowed;

		/**
		*/
		float _turnSpeedFactor;

		/**
		*/
		CEntity* _target;

		/*
		**/
		Logic::Sense _targetSense;

		/*
		**/

		float _acumRotation;
		

	}; // class CAltarStateSwitcher

	REG_FACTORY(CAltarStateSwitcher);

} // namespace Logic

#endif // __Logic_AltarStateSwitcher_H */

