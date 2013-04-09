/**
@file BaseTraveler.h

Contiene la declaraci�n del componente que controla la capacidad de cambiar de anillo
de los elementos del juego

@see Logic::CBaseTraveler
@see Logic::IComponent

@author Jose Luis L�pez S�nchez
@date Febrero, 2013
*/
#ifndef __Logic_BaseTraveler_H
#define __Logic_BaseTraveler_H

#include "Logic/Entity/Component.h"
#include "RingTraveler.h"
#include "../../../Application/Clock.h"


namespace Logic
{
	class CMessage;	
	class CGameStatus;
}
//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de los cambios de anillo. Hereda de RingTraveler, lo amplia, hace que no solo
	pueda cambiar de anillo sino tambi�n a un anillo de otra base.
	
    @ingroup logicGroup

	@author Jose Luis L�pez S�nchez
	@date Febrero, 2013
*/
	class CBaseTraveler :  public CRingTraveler
	{
		DEC_FACTORY(CBaseTraveler);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CBaseTraveler() : CRingTraveler(GetAltTypeIdOf(CBaseTraveler)),_changingBase(false),_changingBaseTime(0),
			_maxChangingBaseTime(5000), _changeAllowed(false), _baseToGo(0) {}
		
		
		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CBaseTraveler();
		
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CHANGE_PLANE.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);


		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);
		
		void resetChangingBase(){_changingBase=false;}

		bool isChangingBase(){return _changingBase;}


		/**
		M�todo que ser� invocado siempre que se termine una el reloj 
		llegue al evento solicitado desde la m�quina de estados.		
		*/
		void timeArrived();
	
	protected:

		bool _changingBase;	

		float _changingBaseTime;

		float _maxChangingBaseTime;

		unsigned short _destiny;

		
		Logic::CGameStatus* _gameStatus;
		bool _changeAllowed;
		unsigned short _baseToGo;

		/**
		*/
		void showBase(unsigned short base);

		/**
		*/
		void jumpToBase();

		/**
		*/
		void returnToPlayerBase();


	}; // class CBaseTraveler

	REG_FACTORY(CBaseTraveler);

} // namespace Logic

#endif // __Logic_CBaseTraveler_H
