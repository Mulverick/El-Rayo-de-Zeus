/**
@file Graphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicalEntity)
		{
			if(_isStatic)
				_scene->remove( 
					static_cast<Graphics::CStaticEntity*>(_graphicalEntity) 
				);
			else
				_scene->remove(_graphicalEntity);				

			delete _graphicalEntity;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gr�fica es NULL");
		assert(entityInfo->hasAttribute("model"));
			_model = entityInfo->getStringAttribute("model");

		_graphicalEntity = createGraphicalEntity(entityInfo);
			if(!_graphicalEntity)
				return false;
		
		Vector3 scale = Vector3::UNIT_SCALE;

		// HACK FRS Esto lo suyo es que el modelo ya lo traiga , no?
		// o meter la escala como vector en el map
		if(_entity->getType() == "World"
			&& _entity->getLogicalPosition()->getRing() == LogicalPosition::CENTRAL_RING)
			scale = Vector3(1.3f,1.0f,1.3f);
		//
		else if(entityInfo->hasAttribute("scaleFactor") )
			scale *=  entityInfo->getFloatAttribute("scaleFactor");

		_graphicalEntity->setTransform(_entity->getTransform());
		_graphicalEntity->setScale(scale);	

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CGraphics::accept(const CMessage *message)
	{
		return	 message->getType() == Message::SET_TRANSFORM ||
				 message->getType() == Message::SET_TRANSFORM_QUAT ||
				 message->getType() == Message::SET_MATERIAL ||
				 message->getType() == Message::SET_SUBENTITY_MATERIAL ;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			{
			CMessageTF *maux = static_cast<CMessageTF*>(message);
			_graphicalEntity->setTransform(maux->getTransform());
			}
			break;
		case Message::SET_TRANSFORM_QUAT:
			//graphicalEntity->setTransform(message._quat);
			break;
		case Message::SET_MATERIAL:
			{
			CMessageString *maux2 = static_cast<CMessageString*>(message);
			_graphicalEntity->setMaterial(maux2->getString());
			}
			break;
		case Message::SET_SUBENTITY_MATERIAL:
			{
			CMessageUIntString *maux3 = static_cast<CMessageUIntString*>(message);
			_graphicalEntity->setSubEntityMaterial(maux3->getString(), maux3->getUInt());
			}
		}

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{		
		assert( _scene && "LOGIC::GRAPHICS>> No existe escena gr�fica!");
		assert( _model.length() > 0  && "LOGIC::GRAPHICS>> No existe modelo!");	
		
		if(entityInfo->hasAttribute("static"))
			_isStatic = entityInfo->getBoolAttribute("static");

		// CREATE STATIC
		if(_isStatic){
			Graphics::CStaticEntity* staticEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if( _scene->add(staticEntity) )
				return staticEntity;
			else 
				return 0;

		// CREATE NO STATIC
		} else {
			Graphics::CEntity* dynamicEntity = new Graphics::CEntity(_entity->getName(),_model);
			if( _scene->add(dynamicEntity) )		
				return dynamicEntity;
			else
				return 0;
		}

	} // createGraphicalEntity
	
	

} // namespace Logic

