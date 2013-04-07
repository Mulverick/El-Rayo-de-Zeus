/**
@file Life.cpp

Contiene la implementaci�n del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llans�
@date Octubre, 2010
*/

#include "Death.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageAudio.h"

namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_audio = "media\\audio\\fallecimiento.wav";
		return true;
	} // spawn
	
	//---------------------------------------------------------

	bool CDeath::accept(const CMessage *message)
	{
		return	message->getType() == Message::DEAD ||
				message->getType() == Message::ANIMATION_FINISHED;

	} // accept
	
	//---------------------------------------------------------

	// TODO separar en funciones / tipo
	void CDeath::process(CMessage *message)
	{
		// HACK FRS habr� que tener en cuenta todos los que tengan muerte animada...
		// (por par�metro en map.txt? animatedDeath = true o CDeathAnimated?)
		if(_entity->getType() == "Player" || _entity->getType() == "OtherPlayer")
			deathAnimated(message);
		else
			death(message);
	
	} // process


	//---------------------------------------------------------


	void CDeath::death (CMessage *message) {
		if(message->getType() == Logic::Message::DEAD)		
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
	} // death

	//---------------------------------------------------------

	void CDeath::deathAnimated(CMessage *message) {

		switch(message->getType())
		{
		// MUERTO
		case Message::DEAD:		{				
			CMessageBoolString *txMsg = new CMessageBoolString(); // Poner la animaci�n de muerte
				txMsg->setType(Logic::Message::SET_ANIMATION);
				txMsg->setString("Death");	
				txMsg->setBool(false);
				_entity->emitMessage(txMsg);
				/* Aqu� ponemos el sonido */
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
			maudio->setType(Message::AUDIO);			
			maudio->setPath(_audio);
			maudio->setId("muerte");
			maudio->setPosition(_entity->getPosition());
			_entity->emitMessage(maudio);
		} break;	
		
		// ANIMACION FINALIZADA		
		case Message::ANIMATION_FINISHED: {
			
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
				if(rxMsg->getString() == "Death") { // Completada animaci�n de muerte? -> END_GAME					
					if(_entity->isPlayer() ) // PLAYER MUERTO -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver"); // HACK Player muerto -> respawn es distinto de base muerta
					else // Resto de entidades
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				}
		 }break;
		
		} // switch

	} // death


} // namespace Logic

