/**
@file Altar.cpp

Contiene la implementación del componente que controla la 
capacidad de un Character de activar/desactivar altares
 
@see Logic::CAltar
@see Logic::IComponent

@author Emilio Santalla
@date Emilio Santalla, 2013
*/

#include "Altar.h"

#include "Logic/Entity/Entity.h"
#include "Map/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageUIntString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUInt.h"


/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/AltarInfo.h"
#include "Logic/PlayerInfo.h"
#include "Application/BaseApplication.h"
#include "../../../Application/GameState.h"

#define DEBUG 0

#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::ALTAR>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Logic 
{
	IMP_FACTORY(CAltar);
	
	//---------------------------------------------------------

	bool CAltar::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				

		//creamos un puntero al gamestatus global (que es única estancia)
		//_gameStatus=Application::CBaseApplication::getSingletonPtr()->getGameState()->getGameStatus();
		_gameStatus=CGameStatus::getSingletonPtr();
		//creamos un altar pasandole la entidad propietaria del presente compontente.
		_altarInfo = _gameStatus->getBase(entity->getLogicalPosition()->
			getBase())->getRing(entity->getLogicalPosition()->getRing())->createAltar(entity);

		if(entityInfo->hasAttribute("activatedMaterial"))
			_activatedMaterial = entityInfo->getStringAttribute("activatedMaterial");
		
		if(entityInfo->hasAttribute("unactivatedMaterial"))
			_unactivatedMaterial = entityInfo->getStringAttribute("unactivatedMaterial");

		_on=true;
		if (_entity->getName()=="Altar1" || _entity->getName()=="Altar2"|| _entity->getName()=="Altar3") _on=false;	
	
	return true;
	} // spawn
	
	//---------------------------------------------------------

	bool CAltar::activate()
	{
		//poner el submaterial de los altares desactivados de inicio
		_gameStatus->getBase(_entity->getLogicalPosition()->getBase())->updateAllAltarsInfo();
		
		if (_on)
		{
			LOG(_entity->getName() << ": activado")
				CMessageUIntString *m = new CMessageUIntString();	
				m->setType(Message::SET_MATERIAL);
				m->setString(_activatedMaterial);			
				_entity->emitMessage(m,this);

			CMessage *txMsg = new CMessage();	
				txMsg->setType(Message::FX_START);
				_entity->emitMessage(txMsg,this);

			_gameStatus->getPlayer(_entity->getLogicalPosition()->getBase())->increaseAltarsActivated();
		}
		else 
		{
			LOG(_entity->getName() << ": desactivado")
			CMessageUIntString *m = new CMessageUIntString();	
			m->setType(Message::SET_MATERIAL);
			m->setString(_unactivatedMaterial);	
			_entity->emitMessage(m,this);

		}

		_gameStatus->getBase(_entity->getLogicalPosition()->getBase())->updateAllAltarsInfo();
		_acumTime = _switchingTime;

		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAltar::deactivate()
	{	
	} // deactivate
	
	//---------------------------------------------------------

	bool CAltar::accept(const CMessage *message)
	{
		return (message->getType() == Message::CONTROL);
	} // accept
	
	//---------------------------------------------------------

	void CAltar::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::SWITCH_ALTAR)
			{
				CMessageUInt *maux = static_cast<CMessageUInt*>(message);
				_player=_entity->getMap()->getEntityByID( maux->getUInt() );
				startSwitchingState();
			}
			else if(message->getAction() == Message::STOP_SWITCH)
			{	
				stopSwitchingState();
			}
		}
	} // process
	
	//---------------------------------------------------------

	void CAltar::startSwitchingState()
	{
		LOG(_entity->getName() << ": cambiando de estado")	
		_switchingState = true;
		_revertingState = false;
	}

	//---------------------------------------------------------
	void CAltar::stopSwitchingState()
	{
		
		if (_switchingState)
		{
			LOG(_entity->getName() << ": volviendo al estado original")
			_switchingState = false;
			_revertingState = true;
		}
	}
	
	//---------------------------------------------------------

	void CAltar::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_switchingState)
		{
			_acumTime -= msecs;
			if (_acumTime <= 0 )
			{
				if (_player)
				{

					CMessageString *m = new CMessageString();	
					m->setType(Message::ALTAR_SWITCHED);
					m->setString(_entity->getName());
					_player->emitMessage(m);		
				}

				_on = !_on;
					
				if (_on)
				{
					LOG(_entity->getName() << ": activado")
					_gameStatus->getPlayer(_entity->getOriginBase())->increaseAltarsActivated();
					CMessageUIntString *m = new CMessageUIntString();	
					m->setType(Message::SET_MATERIAL);
					m->setString(_activatedMaterial);			
					_entity->emitMessage(m,this);		

/////////////////////////////////// HACK TEST FRS Para probar FX
					CMessage *txMsg = new CMessage();	
						txMsg->setType(Message::FX_START);		
						_entity->emitMessage(txMsg,this);
//////////////////////////////////////////////////////////////////
				}
				else 
				{
					LOG(_entity->getName() << ": desactivado")
					_gameStatus->getPlayer(_entity->getOriginBase())->decreaseAltarsActivated();
					CMessageUIntString *m = new CMessageUIntString();	
					m->setType(Message::SET_MATERIAL);
					m->setString(_unactivatedMaterial);				
					_entity->emitMessage(m,this);


///////////// HACK TEST FRS Para probar FX -> Aunque poco se puede probar ya que no tenemos esa funcionalidad impl. :S
		CMessage *txMsg = new CMessage();	
			txMsg->setType(Message::FX_STOP);
			_entity->emitMessage(txMsg,this);
////////////////////
				}

				_switchingState = false;
				_gameStatus->getBase(_entity->getLogicalPosition()->getBase())->updateAllAltarsInfo();
				_acumTime = _switchingTime;
			}
		}

		if (_revertingState)
		{
			_acumTime += msecs;
			if (_acumTime >= _switchingTime)
			{
				if (_on)
					LOG(_entity->getName() << ": activado");
				else 
					LOG(_entity->getName() << ": desactivado");

				_revertingState = false;
				_acumTime = _switchingTime;
			}
		}
	
	} // tick
} // namespace Logic