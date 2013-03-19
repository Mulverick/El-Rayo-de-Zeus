//---------------------------------------------------------------------------
// packet.h
//---------------------------------------------------------------------------

/**
 * @file packet.h
 *
 * Representaci�n de los datos transmitidos por red
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Octumbre, 2006
 */

#ifndef __Net_Packet_H
#define __Net_Packet_H

#include "Connection.h"

namespace Net {

	/**
	 * Los packets manejar�n bytes
	 */
	typedef unsigned char byte;

	/**
	 * Identificador de packets
	 */
    enum PacketType{
		DATA,
		CONNECT,
		DISCONNECT,
		CONTROL
	};

/**
 * Clase que representa de los datos transmitidos por red
 */
class CPacket {

public:

	/**
	 * Constructora. Crea un packet de datos.
	 * @param tipo del packet
	 * @param data es el contenido del packet
	 * @param dataLength es el tama�o de los datos (data)
	 * @param connection es la conexi�n por la que se transmiten
	 * @param canan es el canal por el que se transmiten
	 */
	CPacket(PacketType tipo, byte* data, size_t dataLength, CConnection* connection, unsigned char canal);

	/**
	 * Destructura
	 */
	~CPacket();

	/**
	 * Obtener los datos
	 */
	byte* getData();

	/**
	 * Establecer el contenido del packet
	 */
	void setData(byte* data, size_t length);

	/**
	 * Obtener el tama�o de los datos del packet
	 */ 
	size_t getDataLength();

	/**
	 * Obtener el tipo de los datos
	 */
	PacketType getTipo();

	/**
	 * Establecer el tipo de los datos
	 */
	void setTipo(PacketType tipo);

	/**
	 * Establecer la conexi�n que transmite los datos
	 */ 
	void setConexion(CConnection* connection);

	/**
	 * Obtener la conexi�n que transmite los datos
	 */ 
	CConnection* getConexion();

	/**
	 * Establecer el canal que transmite los datos
	 */ 
	void setCanal(unsigned char canal);

	/**
	 * Obtener el canal que transmite los datos
	 */ 
	unsigned char getCanal();

protected:
	unsigned char _canal;
	size_t _dataLength;
	byte* _data;
	PacketType _packetType;
	CConnection* _connection;

};


} //namespace Net

#endif