/**
@file CLA_Run.h

En este fichero se implementan algunas acciones 
latentes b�sicas.

@author Jose Luis L�pez
@date Marzo 2013

*/
#pragma once

#ifndef __AI_RUNLatentActions_H
#define __AI_RUNLatentActions_H

#include "LatentAction.h"

#include "Logic/Entity/Entity.h"

using namespace Logic;

namespace AI 
{

	/**
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLA_Run : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLA_Run(CEntity* entity, Sense sentido) : CLatentAction() {this->setEntity(entity); _sense=sentido;};
		/**
		Destructor
		*/
		~CLA_Run() {};

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
		virtual LAStatus OnStart();

		/**
		M�todo invocado al final de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al final (y no durante toda la vida de la acci�n).

		En la mayor�a de los casos este m�todo no hace nada.
		*/
		virtual void OnStop();

		/**
		M�todo invocado c�clicamente para que se contin�e con la
		ejecuci�n de la acci�n.
		<p>
		En cada paso de ejecuci�n se obtiene el tiempo actual 
		y se comprueba si se debe acabar la acci�n.

		@return Estado de la acci�n tras la ejecuci�n del m�todo;
		permite indicar si la acci�n ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun() ;

		/**
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier raz�n).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;
		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		<p>
		Esta acci�n no acepta mensajes de ning�n tipo.

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/

		virtual bool accept(const CMessage *message);
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void process( CMessage *message);

		/**
		He a�adido un tick dependiente del tiempo, no sustituye al anterior tick(). se ejecutan ambos.
		He decidido conservar el anterior para que solo est� implementado en CLatentAction y no en todos los heredados.
		*/
		virtual void tick(unsigned int msecs);
		
		virtual void sleepComponents();

		virtual void awakeComponents();


	protected:
		Sense _sense;
	};

} //namespace AI 

#endif // __LOGIC_IDLELatentActions_H