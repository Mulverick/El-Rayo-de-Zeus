/**
@file Entity.cpp

Contiene la implementaci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Julio, 2010
*/

#include "Entity.h"

// Componentes
#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/CameraController.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageBoolTF.h"




namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false) // UNDONE ��� _pos(TLogicalPosition()) ya llama al ctro por defecto, por defecto, valga la rebuznancia
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "��Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo) 
	{
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();
		_logicInput=false;
		Vector3 position=Vector3::ZERO;	
		
		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");	

		if(entityInfo->hasAttribute("logicInput"))
			_logicInput = entityInfo->getBoolAttribute("logicInput");

		if(entityInfo->hasAttribute("degrees"))
			_pos._degrees = entityInfo->getFloatAttribute("degrees");
			
		if(entityInfo->hasAttribute("sense"))
			_pos._sense = static_cast<Logic::Sense>(entityInfo->getIntAttribute("sense"));
		else
			//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna por defecto direcci�n LEFT
			_pos._sense = Logic::Sense::LEFT;

		if(entityInfo->hasAttribute("base"))					
			_pos._base = entityInfo->getIntAttribute("base");

		if(entityInfo->hasAttribute("ring"))
			_pos._ring = static_cast<Logic::Ring>(entityInfo->getIntAttribute("ring"));
		else			
			//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
			//pese a todo no pete.
			_pos._ring= Logic::Ring::CENTRAL_RING;  

		// UNDONE ��� Este height ya se inicializa a 0 en el ctor por defecto de TLogicalPosition
		//_pos._height = 0;

		// UNDONE ���: Esta informaci�n de f�sica es necesaria para alguien m�s?
		//if(entityInfo->hasAttribute("angularBox"))					
		//	_angularBox = entityInfo->getFloatAttribute("angularBox");

		if (_logicInput)
		{
			position=fromLogicalToCartesian(_pos._degrees,_pos._height, _pos._base,_pos._ring);
			_transform.setTrans(position);
			float yaw = Math::fromDegreesToRadians(getDegree());
			Math::yaw(yaw,_transform);

		}
		else //logicInput=false
		{
			position=CServer::getSingletonPtr()->getRingPositions(_pos._base,_pos._ring);						
			_transform.setTrans(position);
		}

		if(entityInfo->hasAttribute("position"))
		{
			position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			setIsPlayer( entityInfo->getBoolAttribute("isPlayer") );		

		// Inicializamos los componentes
		bool correct = true;
		TComponentList::const_iterator it;		
			for( it = _components.begin(); it != _components.end() && correct; ++it )
				correct = (*it)->spawn(this,map,entityInfo) && correct;
		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		if ( this->getType() == "Camera" )
		{
			//CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getCameraController()->setControlledCamera(this);
			int i=0;
		}

		// Activamos los componentes
		TComponentList::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;
			for( it = _components.begin(); it != _components.end(); ++it )
				_activated = (*it)->activate() && _activated;
		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si �ramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
			setIsPlayer(false);

		TComponentList::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->deactivate();

		_activated = false;

	} // deactivate

	//---------------------------------------------------------
	void CEntity::setIsPlayer(bool isPlayer) 
	{ 		
		if(isPlayer == _isPlayer)
			return;

		_isPlayer = isPlayer; 
		if(_isPlayer) {
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);		
		} else {
			if(CServer::getSingletonPtr()->getPlayer() == this)
				CServer::getSingletonPtr()->setPlayer(0);
			if(GUI::CServer::getSingletonPtr()->getPlayerController()->getControlledAvatar() == this)
				GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
		}
	} // setIsPlayer

	//---------------------------------------------------------
	 const Vector3 CEntity::fromLogicalToCartesian(const float grados,const float altura, const unsigned short base, const Logic::LogicalPosition::Ring ring)
	 {		 
		float offset=0;// se trata de un offset de radio, no de altura

		if (this->getType().compare("Altar")==0)
		{
			offset=-9;
		}

		Vector3 resultado=Vector3::ZERO;
		resultado=Math::fromCylindricalToCartesian(grados, CServer::getSingletonPtr()->getRingRadio(base,ring)+offset,CServer::getSingletonPtr()->getRingPositions(base,ring).y+altura+126);
		return resultado;
	 }
	 
	  const float CEntity::getY(const unsigned short base, const Logic::LogicalPosition::Ring ring)
	  { 	
		  Vector3 position=Vector3::ZERO;
		  position=CServer::getSingletonPtr()->getRingPositions(base,ring);	
		  return position.y;
	  }

	//---------------------------------------------------------
	void CEntity::tick(unsigned int msecs) 
	{
		TComponentList::const_iterator it;

		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		_components.push_back(component);
		component->setEntity(this);

	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component)
	{
		TComponentList::const_iterator it = _components.begin();

		bool removed = false;
		// Buscamos el componente hasta el final, por si aparec�a
		// m�s de una vez... (no tendr�a mucho sentido, pero por si
		// acaso).
		while (it != _components.end()) 
		{
			if (*it == component)
			{
				it = _components.erase(it);
				removed = true;
			}
			else
				++it;
		}
		if (removed)
			component->setEntity(0);
		return removed;

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{
		IComponent* c;
		while(!_components.empty())
		{
			c = _components.back();
			_components.pop_back();
			delete c;
		}

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(CMessage *message, IComponent* emitter)
	{
		if(!_activated) // HACK Si la entidad no est� activa, no recibe mensajes
			return false;

		// Interceptamos los mensajes que adem�s de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			CMessageTF *maux = static_cast<CMessageTF*>(message);
			_transform = maux->getTransform();
		}

		TComponentList::const_iterator it;

		message->grab();
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it) )
				anyReceiver = (*it)->set(message) || anyReceiver;
		}

		message->release();
		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		//message->setBool(true); // [���] Interesante si s�lo queremos comprobar colisiones en cambios de posici�n y no en todos los SET_TRANSFORM
		emitMessage(message);

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) 
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		//message->setBool(true); // [���] Interesante si s�lo queremos comprobar colisiones en cambios de posici�n y no en todos los SET_TRANSFORM
		emitMessage(message);

	} // setPosition

	//---------------------------------------------------------

	void CEntity::setLogicalPosition(const Logic::TLogicalPosition &pos)
	{
		_pos._base=pos._base;
		_pos._degrees=pos._degrees;
		_pos._ring=pos._ring;
		_pos._sense=pos._sense;
		_pos._height=pos._height;

		const Vector3 position=fromLogicalToCartesian(_pos._degrees,_pos._height,_pos._base,_pos._ring);
		setPosition(position);


	} //setLogicalPosition
		
	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;
		
		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		emitMessage(message);

	} // setOrientation

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;

	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYaw(float yaw) 
	{
		Math::setYaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setYaw

	//---------------------------------------------------------

	void CEntity::setRoll(float roll) 
	{
		Math::setRoll(roll,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setRoll

	//---------------------------------------------------------

	void CEntity::setPitch(float pitch) 
	{
		Math::setPitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setPitch

	//---------------------------------------------------------

	void CEntity::setPitchYaw(float pitch,float yaw) 
	{
		Math::setPitchYaw(pitch,yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setPitchYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // yaw

	//---------------------------------------------------------

	void CEntity::roll(float roll) 
	{
		Math::roll(roll,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // roll

	//---------------------------------------------------------

	void CEntity::pitch(float pitch)
	{
		Math::pitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // pitch

	//---------------------------------------------------------

	void CEntity::setDegree(const float &degree)
	{
		_pos._degrees=degree;
	}

	//---------------------------------------------------------
	
	void CEntity::setRing(const LogicalPosition::Ring &ring)
	{
		_pos._ring=ring;
	}

	//---------------------------------------------------------

	const float CEntity::getRadio()
	{
		
		return CServer::getSingletonPtr()->getRingRadio(_pos._base,_pos._ring);
	}
} // namespace Logic
