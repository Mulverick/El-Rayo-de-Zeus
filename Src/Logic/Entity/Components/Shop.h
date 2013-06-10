/***

@see Logic::CShop
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#ifndef __Logic_Shop_H
#define __Logic_Shop_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"

#include "../../../../dependencies/include/cegui/CEGUIRect.h"

namespace Logic
{
	class CGameStatus;
	class CMessageFloat;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
	class WindowManager;
	class FrameWindow;
	class Rect;
}

namespace ScriptManager
{
	class Server;
}


//declaraci�n de la clase
namespace Logic 
{
/***
	Este componente controla el Shop mediante LUA
    @ingroup logicGroup

	@author Pablo Terrado
	@date Febrero, 2013
*/
	class CShop : public IComponent
	{
		DEC_FACTORY(CShop);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CShop() : IComponent(GetAltTypeIdOf(CShop)), _time(0), numBase(0) { }
		
		/** Destructor */
		CShop::~CShop();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool accept(const CMessage *message);

		void process(CMessage *message);	

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		*/
		void deactivate();

		void tick(unsigned int msecs);

		void displayShop();

		//bool handleMouseUp(const CEGUI::EventArgs& e);
		//bool handleMouseDown(const CEGUI::EventArgs& e);
		//bool handleDrag(const CEGUI::EventArgs& e);

		bool handleClose(const CEGUI::EventArgs&e);

		void deactivateControl();
		bool activateControl();
		

	protected:

		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		/**
		Puntero al gamestatus global
		*/
		Logic::CGameStatus* _gameStatus;

		unsigned short numBase, player;

		//area for limiting mouse movements
		CEGUI::Rect _area;

		CEGUI::FrameWindow* _winShop;

		 
	}; // class CShop

	REG_FACTORY(CShop);

} // namespace Logic

#endif // __Logic_Shop_H
