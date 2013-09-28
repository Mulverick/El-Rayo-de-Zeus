/***
@file Hud.cpp

Contiene la implementaci�n del componente que controla el HUD mediante CEGUI.
 
@see Logic::CHud
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#include "Hud.h"

//Inclusion de los mensajes
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Logic/GameStatus.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"

#include "GUI/Server.h" //PT
#include "GUI/HudController.h" //PT

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"


namespace Logic 
{
	IMP_FACTORY(CHud);
	
	//---------------------------------------------------------
		
	CHud::~CHud() {	 	
	} // destructor

	//---------------------------------------------------------
	
	bool CHud::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		numBase = entity->getOriginBase();

		player = numBase;

		//Get CGameStatus Singleton to recover info to show in the HUD
		_gameStatus = Logic::CGameStatus::getSingletonPtr();

		//recover the maximun number of altars of the base. Its better than make the call
		// in tick method
		numMaxAltares = _gameStatus->getBase(numBase)->_numAltars;

		//Inicializo su nickname en el gamestatus con el nombre de la entidad del player.
		_gameStatus->getPlayer(player)->setNickName(entity->getName());
		// TODO FRS Habr�a que tener cuidado y considerar si realmente queremos que entityName = nickName...
		// Podr�a darse el caso de necesitar acceder a las entidades por nombres explicitos (PlayerRed,
		// PlayerGreen, etc...) y que el nick fuera un atributo aparte...

		//init HUD Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Hud");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initHud");

		//Inicializamos el nombre del jugador en el HUD, con el nickname que contenga ese jugador en su info del player de game status
		ScriptManager::CServer::getSingletonPtr()->executeProcedureString("initNickName",_gameStatus->getPlayer(player)->getNickName());

		return true;

	} // spawn


	//---------------------------------------------

	bool CHud::activate()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showHud");
		return true;
	} //activate

	//---------------------------------------------

	void CHud::deactivate()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideHud");

	}//deactivate
	
	//---------------------------------------------------------

	bool CHud::accept(const CMessage *message)
	{
		return message->getType() == Message::HUD;
	} // accept
	
	//---------------------------------------------------------

	void CHud::process(CMessage *message)
	{

			switch(message->getType())
			{
				case Message::HUD:

					if(message->getAction() == Message::DISPLAY_HUD)
						displayHud();

					if(message->getAction() == Message::UPDATE_HUD_LIFE)
					{
						CMessageFloat *maux = static_cast<CMessageFloat*>(message);
						_ratioLife = maux->getFloat();
						updateHudLife(_ratioLife);
					}

					break;
			}

	} // process

	void CHud::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		_time += msecs;

		//se recupera la informacion de Logic::CGameStatus cada medio segundo para actualizar el HUD
		if(_time >= 500)
		{
			_time = 0;
			//Activated Altars
			//parametros: 
			//primero: la cadena con el procedimiento de LUA a ejecutar : showAltarsActivated
			//segundo: el numero de altares activados		//tercero: el numero maximo de altares
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showAltarsActivated",_gameStatus->getBase(numBase)->getNumActivatedAltars(),numMaxAltares);

			//Life of the base
			//parametros: 
			//primero: la cadena con el procedimiento de LUA a ejecutar : showBaseLife
			//segundo: las vidas de la base
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showBaseLife",_gameStatus->getBase(numBase)->getBaseLife());

			//Enemies number / players that are not me in my origin base
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showEnemiesInBase",_gameStatus->getEnemiesInBase(numBase));

			//Merite Points
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showPM",_gameStatus->getPlayer(player)->getMeritPoints());
		}
		
	}//tick

	void CHud::displayHud()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeHudVisibility");
	}

	void CHud::updateHudLife(float ratioLife)
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateHudLife",ratioLife);
	}

} // namespace Logic



