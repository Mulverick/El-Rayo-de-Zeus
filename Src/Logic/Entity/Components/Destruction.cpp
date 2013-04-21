/**
@file Destruction.cpp

Contiene la implementaci�n del componente que controla el movimiento 
de la entidad.
 
@see Logic::CDestruction
@see Logic::IComponent

@author 
@date
*/

#include "Destruction.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "PhysicalCharacter.h"

#include "AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Application/BaseApplication.h"


/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"   
namespace Logic 
{
	IMP_FACTORY(CDestruction);
	
	//---------------------------------------------------------

	bool CDestruction::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
		_gameStatus=CGameStatus::getSingletonPtr();
	
		return true;
	} // spaw
	
	//---------------------------------------------------------

	bool CDestruction::activate()
	{
		_destroyingSpeed=0.13f;
		if (_entity->getLogicalPosition()->getBase()==1) // si es en la que estoy probando
		{
			if (_gameStatus->getBase(1)->getAllAltarsActivated())
			{
			_reloj->addTimeObserver(_entity->getEntityID(),this,5000);
			_reloj->addTimeObserver(-_entity->getEntityID(),this,5500);
			if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
					_entity->getComponent<CPhysicalCharacter>()->sleep();
				/*if (_entity->getComponent<CAvatarController>()!=NULL)
					_entity->getComponent<CAvatarController>()->sleep();*/
			}
		
		}
		_step=0;
		//_position=
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CDestruction::deactivate()
	{
	} // deactivate
	
	//---------------------------------------------------------

	bool CDestruction::accept(const CMessage *message)
	{
		return (isAwake() && message->getType() == Message::CONTROL && 
					message->getAction() == Message::DESTROY);
	}
	
	//---------------------------------------------------------

	void CDestruction::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::DESTROY)
					destroy();
		}
	} // process
	
	//---------------------------------------------------------

	void CDestruction::destroy() 
	{
		if (!_destroying)
		{
			//_step=1;
			_destroying = true;
	
		}
		else
		{
			_step++;
		}
	} 
	
	//---------------------------------------------------------

	void CDestruction::timeArrived()
	{
		destroy();

	}
		
	void CDestruction::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		if (_entity->getLogicalPosition()->getBase()==1/* && _entity->getLogicalPosition()->getBase()->isDestroying()*/) // si es en la que estoy probando
		{
			if (_gameStatus->getBase(1)->getAllAltarsActivated() && !_destroying)
			{
				destroy();
				_reloj->addTimeObserver(_entity->getEntityID(),this,5000);
				_reloj->addTimeObserver(-_entity->getEntityID(),this,5500);
				
					/*if (_entity->getComponent<CAvatarController>()!=NULL)
						_entity->getComponent<CAvatarController>()->sleep();*/
			}
		}

	
		if (_destroying)
		{
			if (_step>=1)
			{
				
			if (msecs>100) msecs=80;

			_destroyingSpeed-= 0.0002f * msecs;   //gravedad 0.0003f
			float tickHeight = _destroyingSpeed * msecs;
			
			/*Logic::CMessageFloat *m = new Logic::CMessageFloat();
			m->setType(Logic::Message::AVATAR_MOVE);
			m->setAction(Logic::Message::JUMP);
			m->setFloat(tickHeight);
			_entity->emitMessage(m);*/
			if (_entity->getType()!="World"){
				Vector3 position=_entity->getPosition();
				position.y+=tickHeight;				
				_entity->setPosition(position);
				if (_step==2)
				{
					if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
						_entity->getComponent<CPhysicalCharacter>()->sleep();

					_entity->roll(msecs*0.002);			
				}
			}
			else
			{
				if (_entity->getType()=="World")
				{
					Vector3 position=_entity->getPosition();
					position.y+=tickHeight;				
					_entity->setPosition(position);
					if (_step==2)
					{
						_entity->roll(msecs*0.0001);
						_entity->pitch(msecs*0.0001);
					}	
				}
			}
		}
		

	}
	 // tick
	}
} // namespace Logic