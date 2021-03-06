//---------------------------------------------------------------------------
// GameNetMsgManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.cpp

Contiene la implementaci�n del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llans�
@date Febrero, 2011
*/

#include "GameNetMsgManager.h"

#include "Application/BaseApplication.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

#include "NET/Manager.h"
#include "NET/packet.h"
#include "NET/buffer.h"


#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::NET_MANAGER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	CGameNetMsgManager* CGameNetMsgManager::_instance = 0;

	//--------------------------------------------------------

	CGameNetMsgManager::CGameNetMsgManager()
	{
		_instance = this;
		_netManager = Net::CManager::getSingletonPtr();
	} // CServer

	//--------------------------------------------------------

	CGameNetMsgManager::~CGameNetMsgManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CGameNetMsgManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CGameNetMsgManager no permitida!");

		new CGameNetMsgManager();

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameNetMsgManager::Release()
	{
		assert(_instance && "Logic::CGameNetMsgManager no est� inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//--------------------------------------------------------

	void CGameNetMsgManager::activate() 
	{
		// TODO Escuchamos los mensajes de red. Engancharnos a Net::CManager
		_netManager->addObserver(this);
	} // activate

	//--------------------------------------------------------

	void CGameNetMsgManager::deactivate() 
	{	
		// TODO Dejamos de escuchar los mensajes de red. 
		// Desengancharnos de Net::CManager
		_netManager->removeObserver(this);
	} // deactivate

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendEntityMessage(const CMessage *txMsg, TEntityID destID)
	{
		// [f��] Serializar el mensaje l�gico. Para la serializaci�n
		// apoyarse en Net::CBuffer. Se debe guardar primero el tipo
		// de mensaje de red (Net::ENTITY_MSG) y luego el id de la
		// entidad y el mensaje serializado. La funci�n de serializaci�n
		// de un mensaje l�gico est� en Message. Al final se debe hacer
		// el env�o usando el gestor de red.
		// Es un mensaje para enviar por el tubo.
		// Lo serializamos y enviamos por la red...
		Net::NetMessageType msgType = Net::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;		
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			//Message::Serialize(txMsg, serialMsg);   // Serializamos el mensaje a continuaci�n en el buffer
			txMsg->serialize(serialMsg);
		_netManager->send(serialMsg.getbuffer(), serialMsg.getSize());
		//std::cout << "Enviado mensaje tipo " << txMsg->getType() << " para la entidad " << destID << " de tama�o " << serialMsg.getSize() << std::endl;
		//LOG("TX ENTITY_MSG " << txMsg._type << " to EntityID " << destID);

	} // sendEntityMessage

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processEntityMessage(Net::CPacket* packet)
	{
		// TODO M�todo que debe de ser invocado desde el m�todo que
		// recibe todos los packets de red cuando el tipo de mensaje
		// de red es Net::ENTITY_MSG. Se debe sacar el ID de la entidad,
		// recuperarla, deserializar el mensaje y envi�rselo

		// Creamos un buffer con los datos para leer de manera m�s c�moda
		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset(); // Reiniciamos el puntero de lectura a la posici�n 0

		// Extraemos, pero ignoramos el tipo de mensaje de red. Ya lo hemos procesado		
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		TEntityID destID; // Escribimos el id de la entidad
			serialMsg.read(&destID, sizeof(destID));
		altTypeId classId;
			serialMsg.read(&classId, sizeof(classId));

		//std::cout << "Mensaje recibido de tipo " << classId<< " para la entidad " << destID << " de tama�o " << serialMsg.getSize() << std::endl;

		CMessage * rxMsg = Logic::CMessageFactory::getSingletonPtr()->create(classId);
			rxMsg->deserialize(serialMsg);


		// PROPAGACI�N INMEDIATA: Msgs que deben propagarse directamente desde 1 cliente al resto (p.e. CONTROL)
		if(rxMsg->getType() == Message::CONTROL) // HACK: Si hay m�s mensajes de propagaci�n inmediata -> config en map.txt
			_netManager->send(serialMsg.getbuffer(), serialMsg.getSize(), packet->getConnection() );

		// RTX DESERIALIZADO

		/*
		CEntity* destEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
			if(destEntity != 0)
				destEntity->emitMessage(rxMsg);
		*/
		if(packet->getConnection()){}
			//LOG("RX ENTITY_MSG " << rxMsg._type << " from NetID " << packet->getConnection()->getId() << " for EntityID " << destID);
	} // processEntityMessage


	/*******************
		NET: IObserver
	*********************/
	// Aqu� es donde debemos recibir los mensajes de red
	void CGameNetMsgManager::dataPacketReceived(Net::CPacket* packet)
	{
		// TODO para solo sacar el NetID no sale rentable cargarlo en un buffer
		// O extraemos con el memcpy el netID solo
		// O adaptamos el processEntityMessage para que ya use ese buffer y se deje de hostias... :P

		Net::CBuffer rxSerialMsg; // Packet: "NetMessageType | extraData"
			rxSerialMsg.write(packet->getData(),packet->getDataLength());
			rxSerialMsg.reset();

		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );			
		switch (rxMsgType)
		{

		case Net::ENTITY_MSG:	
			processEntityMessage(packet);
			break;	

		// UNDONE FRS Todo esto ahora se gestiona en el CDeath
		//case Net::NetMessageType::END_GAME:	

		//	TEntityID entityID; 
		//		rxSerialMsg.read(&entityID, sizeof(entityID) );  //	Packet: "NetMessageType | extraData(NetID)"	
		//	
		//	CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID);
		//		if( player->isLocalPlayer() )						// GameOver si el END_GAME es para nuestro player
		//			Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
		//		else											// Si no, eliminamos ese player del mapa
		//			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(player);

		//	break;
		}
	} // dataPacketReceived

	//--------------------------------------------------------

} // namespace Logic
