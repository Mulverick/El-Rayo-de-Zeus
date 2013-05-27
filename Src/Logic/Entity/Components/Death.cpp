/**
@file Death.cpp

Contiene la implementaci�n del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llans�
@date Octubre, 2010
*/

#include "Death.h"
//#include "Application/BaseApplication.h"
//#include "Graphics/AnimatedEntity.h"
//#include "Logic/Entity/Entity.h"
//#include "Logic/Maps/EntityFactory.h"
// FRS Prohibido depender de Application en Logic (siempre que se pueda); adem�s el gameover lo gestiona ya la FSM 

//#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
//#include "Logic/Entity/Messages/MessageString.h"
//#include "Logic/Entity/Messages/MessageBoolString.h"
//#include "Logic/Entity/Messages/MessageAudio.h"

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;
	} // spawn
	
	//---------------------------------------------------------

	bool CDeath::accept(const CMessage *message)
	{
		return	message->getType() == Message::DEAD;

	} // accept
	
	//---------------------------------------------------------

	// TODO separar en funciones / tipo
	void CDeath::process(CMessage *message)
	{
		// HACK FRS habr� que tener en cuenta todos los que tengan muerte animada...
		// (por par�metro en map.txt? animatedDeath = true o CDeathAnimated?)
			death(message);
	
	} // process


	//---------------------------------------------------------


	void CDeath::death (CMessage *message) {
		if(message->getType() == TMessageType::DEAD)		
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
	} // death

	//---------------------------------------------------------

	void CDeath::deathAnimated(CMessage *message) {

		switch(message->getType())
		{
		//CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

		//init HUD Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("RespawnPlayer");
			CMessageBoolUShort *txMsg = new CMessageBoolUShort(); // Poner la animaci�n de muerte
				txMsg->setType(TMessageType::SET_ANIMATION);
				txMsg->setUShort( Logic::DEATH );	
				txMsg->setBool(false);
				_entity->emitMessage(txMsg);
				/* Aqu� ponemos el sonido */
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
			maudio->setType(Message::AUDIO);			
			maudio->setPath(_audio);
			maudio->setId("muerte");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(false);
			_entity->emitMessage(maudio);
		} break;	
		
		// ANIMACION FINALIZADA		
		case Message::ANIMATION_FINISHED: {
			
			CMessageUShort *rxMsg = static_cast<CMessageUShort*>(message);
				if(rxMsg->getUShort() == Logic::DEATH ) { // Completada animaci�n de muerte? -> END_GAME					
				/* UNDONE FRS Esto ahora lo gestiona la FSM
					if(_entity->isPlayer() ) // PLAYER MUERTO -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver"); // HACK Player muerto -> respawn es distinto de base muerta
				
					else // Resto de entidades
				*/		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				
				}
		 }break;
		
		} // switch

	} // death


} // namespace Logic

