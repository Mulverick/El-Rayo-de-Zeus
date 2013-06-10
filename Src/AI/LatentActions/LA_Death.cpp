#include "LA_Death.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "../../Logic/Entity/Components/Combat.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "../../Logic/Entity/Components/PhysicalCharacter.h"

#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageAudio.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"
#include "Logic/Maps/EntityFactory.h"


namespace AI
{
//////////////////////////////
//	Implementaci�n de CLA_Death
//////////////////////////////

	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminar� la acci�n mediante el atributo _time

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLA_Death::OnStart()
	{
		//PT
		unsigned int currentTime = Application::CBaseApplication::getSingletonPtr()->getAppTime();
		_endingTime = currentTime + _time;

		//init Respawn Layout and functions
		if (_entity->isPlayer()){

			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideHud"); //oculto el HUD
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideShop"); //oculto el SHOP

			ScriptManager::CServer::getSingletonPtr()->loadExeScript("RespawnPlayer");
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("initRespawn");
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showRespawn");
		}

		sleepComponents();
		std::cout<<"AI::StateMachine::WTF-I am Death!!"<<std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::DEATH);
		message->setAction(Message::WALK_STOP);
		message->setBool(false);
		_entity->emitMessage(message);
		std::string _audio = "media\\audio\\fallecimiento.wav";
		Logic::CMessageAudio *maudio=new Logic::CMessageAudio();		
		maudio->setType(Message::AUDIO);			
		maudio->setPath(_audio);
		maudio->setId("muerte");
		maudio->setPosition(_entity->getPosition());
		maudio->setNotIfPlay(false);
		maudio->setIsPlayer(false);
		_entity->emitMessage(maudio);
		
		_scene=_entity->getMap()->getGraphicScene();
		if (_entity->isPlayer())
			_scene->activateCompositor("BW");

		//PT
		//return SUSPENDED;
		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_Death::OnStop()
	{

		//PT
		if (_entity->isPlayer())
		{
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideRespawn"); //escondo la pantalla de respawn
			_scene->deactivateCompositor("BW"); //desactivo el compositor blanco y negro
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showHud"); //muestro el HUD
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showShop"); //muestro el SHOP
			respawn();

		}

		if(_entity->getType()=="OtherPlayer")
			respawn();

		awakeComponents();

		finish(true);

	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En cada paso de ejecuci�n se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acci�n.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLA_Death::OnRun() 
	{
		// TODO PR�CTICA IA
		// En cada paso de ejecuci�n tendremos que comprobar si hemos
		// superado el tiempo de espera. Seg�n lo hayamos superado o no,
		// la acci�n tendr� que pasar a un estado de terminado con �xito o
		// quedarse en el mismo estado en ejecuci�n.
		/*if(Application::CBaseApplication::getSingletonPtr()->getAppTime() < _endingTime)
			return RUNNING;
		else 
			return SUCCESS;*/


		//PT
		if (_entity->isPlayer()){
			if(Application::CBaseApplication::getSingletonPtr()->getAppTime() > _endingTime)
			{
				ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateTime",0);
				return SUCCESS;
			}
			else
			{
				int timeToRevive = (int) ((_endingTime-Application::CBaseApplication::getSingletonPtr()->getAppTime())/1000);
				ScriptManager::CServer::getSingletonPtr()->executeProcedure("updateTime",timeToRevive);
				return RUNNING;
			}
		}
		else if(_entity->getType()=="OtherPlayer")
		{
			if(Application::CBaseApplication::getSingletonPtr()->getAppTime() > _endingTime)
				return SUCCESS;
			else
				return RUNNING;
		}

		return RUNNING;


	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_Death::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n no acepta mensajes de ning�n tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/
	bool CLA_Death::accept(const CMessage *message)
	{		
		return	(message->getType()==Message::ANIMATION_FINISHED);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Death::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::ANIMATION_FINISHED: //ConditionFail
			{
				CMessageUShort* maux = static_cast<CMessageUShort*>(message);
				if (maux->getUShort()==Logic::DEATH)
				{
				//		finish(true);
					//el finish es para cambiar a otro estado, pero de momento este el estado en el que quiero que permanezca. Otro posible estado ser�a desapareciendo quiza...
				}
				break;
			}
		}
	}
	
	void CLA_Death::tick(unsigned int msecs) 
	{
		CLatentAction::tick();

	}

	void CLA_Death::sleepComponents()
	{
		if (_entity->getComponent<CCombat>()!=NULL)
			_entity->getComponent<CCombat>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->sleep();
		if (_entity->getComponent<CPhysicalCharacter>() != 0)
			_entity->getComponent<CPhysicalCharacter>()->sleep();
		if (_entity->getComponent<CJump>() != 0)
			_entity->getComponent<Logic::CJump>()->sleep();
	}

	void CLA_Death::awakeComponents()
	{
		if (_entity->getComponent<CCombat>()!=NULL)
			_entity->getComponent<CCombat>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<Logic::CJump>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->awake();
		if (_entity->getComponent<CPhysicalCharacter>()!=NULL)
			_entity->getComponent<CPhysicalCharacter>()->awake();
	}

	void CLA_Death::respawn()
	{
			//PT Respawneo en la base Origen
			if (_entity->getComponent<CBaseTraveler>()!=NULL)
			{	
				_entity->getComponent<CBaseTraveler>()->respawnInBaseOrigin();
			}

			//PT mando mensaje al componente LIFE para restaurar la vida al m�ximo
			CMessageBoolUShort *message = new CMessageBoolUShort();
			message->setType(Message::LIFE_RESTORE);
			_entity->emitMessage(message);
	}

} //namespace LOGIC