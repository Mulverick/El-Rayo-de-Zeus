#include "LA_Beaten.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"

#include "../../Logic/Entity/Components/Combat.h"
#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageUShort.h"


namespace AI
{
//////////////////////////////
//	Implementaci�n de CLA_Beaten
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
	CLatentAction::LAStatus CLA_Beaten::OnStart()
	{
		std::cout<<"AI::StateMachine::WTF-I am beated!! - "<< _entity->getName() << " - " << std::endl;
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::DAMAGE);
		message->setAction(Message::WALK_STOP);
		message->setBool(false);
		_entity->emitMessage(message);
		// TODO PR�CTICA IA
		// Al iniciar el wait, calculamos el instante de tiempo en 
		// el que se tiene que finalizar la acci�n. 
		// El tiempo de espera se guarda en _time.
		// Para obtener el instante actual podemos usar 
		// Application::CBaseApplication::getSingletonPtr()->getAppTime()
		//
		//_endingTime = Application::CBaseApplication::getSingletonPtr()->getAppTime() + _time; 
		// Otra opci�n es usar
		// BaseSubsystems::CServer::getSingletonPtr()->getAppTimer()
		// para sacar el timer de la aplicaci�n y usar su m�todo 
		// getMilliseconds.

		return SUSPENDED;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_Beaten::OnStop()
	{
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
	CLatentAction::LAStatus CLA_Beaten::OnRun() 
	{
		std::cout<<"CLA_Beaten::OnRun - "<< _entity->getName() << " - " << std::endl;
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
		//return RUNNING;
		return SUCCESS;
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_Beaten::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		std::cout<<"CLA_Beaten::OnAbort - "<< _entity->getName() << " - " << std::endl;
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
	bool CLA_Beaten::accept(const CMessage *message)
	{		
		return	(message->getType()==Message::ANIMATION_FINISHED);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Beaten::process(CMessage *message)
	{
		std::cout<<"CLA_Beaten::process - "<< _entity->getName() << " - " << std::endl;
		switch(message->getType())
		{
		case Message::ANIMATION_FINISHED: //ConditionFail
			{
				CMessageUShort* maux = static_cast<CMessageUShort*>(message);
				if (maux->getUShort()==Logic::DAMAGE)
				{
					finish(true);
				}
				break;
			}
		}
	}

	
	void CLA_Beaten::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Beaten::sleepComponents(){}

	void CLA_Beaten::awakeComponents(){}
} //namespace LOGIC