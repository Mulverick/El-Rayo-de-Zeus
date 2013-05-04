/**
@file RingTraveler.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CRingTraveler
@see Logic::IComponent

@author Jose Luis L�pez S�nchez
@date Febrero, 2013
*/
#include "Logic/Entity/Entity.h"

#include "Logic/Entity/Components/RingTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageChar.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::RING_TRAVELER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Logic 
{
	IMP_FACTORY(CRingTraveler);
	
	//---------------------------------------------------------

	CRingTraveler::~CRingTraveler() 
	{
	} // ~CGRingTraveler
	
	void CRingTraveler::doChange()
	{
		//return;
		if (_changingRing /*&& !_desencogiendo*/)
		{
			CMessageChar *m0 = new CMessageChar();	
			m0->setType(Message::AVATAR_MOVE);
			m0->setAction(Message::CHANGE_RING);		
			m0->setChar(_toUp);
			_entity->emitMessage(m0,this);	
			LOG("Change Ring: " << (int) m0->getChar() );
			
						
			_changingRing=false;
			_desencogiendo=true;
			_changingRingTime=0;
		}else
		{
			/*
			CMessageString *m3 = new CMessageString();	
			m3->setType(Message::SET_MATERIAL);
			m3->setString(_entity->getInitialMaterial());
			_entity->emitMessage(m3,this);
			*/
			_desencogiendo=false;

		
		}
	}

	//---------------------------------------------------------

	bool CRingTraveler::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
	
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		
		
		//Graphics::CServer* server=Graphics::CServer::getSingletonPtr();
		
		//_scene=_entity->getMap()->getGraphicScene();
		return true;

	} // spawn
	
	//---------------------------------------------------------
	bool CRingTraveler::activate()
	{
		_desencogiendo=false;
		return true;
	}
	
	bool CRingTraveler::accept(const CMessage *message)
	{
		if (_entity->getLogicalPosition()->getRing()==Logic::LogicalPosition::LOWER_RING && message->getAction()==Message::GO_DOWN)
			return false;//si estamos abajo no dejamos bajar mas.
		if (_entity->getLogicalPosition()->getRing()==Logic::LogicalPosition::UPPER_RING && message->getAction()==Message::GO_UP)
			return false;//si estamos arriba no dejamos subir mas.

		if (_desencogiendo) return false;
		return (!_changingRing && isAwake() && message->getType() == Message::CONTROL /*&& (
						message->getAction() == Message::GO_DOWN || 
						message->getAction() == Message::GO_UP)*/); //ANULADO, NO OLVIDARLO
	} // accept
	
	//---------------------------------------------------------

	void CRingTraveler::process(CMessage *message)
	{
		if(message->getAction() != Message::GO_DOWN && message->getAction() != Message::GO_UP ) 
			return;
		if (_changingRing || !isAwake() ) return;
		_changingRing=true; //TODO Pablo. S�lo si no esta saltandose puede realizar la accion de cambio de anillo.

		message->getAction() == Message::GO_UP ? _toUp = 1 : _toUp = -1;  // ��� GO_UP (+1) vs GO_DOWN (-1)  son las �nicas dos opciones que pasan el filtro del accept
 	} // process

	void CRingTraveler::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
	}

} // namespace Logic

