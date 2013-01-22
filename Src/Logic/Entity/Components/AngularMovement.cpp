/**
@file AngularMovement.h

Contiene la declaraci�n del componente que controla el movimiento 
angular de entidades.

@see Logic::CAngularMovement
@see Logic::IComponent

@author Jos� Luis L�pez
@date Diciembre, 2012
*/

#include "AngularMovement.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
 #include "Application/BaseApplication.h"

//declaraci�n de la clase
namespace Logic 
{
	IMP_FACTORY(CAngularMovement);

	bool CAngularMovement::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");

		// Pablo. Si la entidad tiene el atributo jumpSpeed la capturamos y la guardamos en _jumpSpeed
		// En ppio solo la va a tener el player
		if(entityInfo->hasAttribute("initialJumpSpeed"))
			jumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");

		// Pablo. Inicializo la gravedad
		if(entityInfo->hasAttribute("gravity"))
			gravity = entityInfo->getFloatAttribute("gravity");
		//gravity = 9.8; //expresada en metros /seg

		// velocidad inicial. es 28 m / seg

		//Pablo. Calculo la altura maxima del salto
		//Hmax = (jumpSpeed*jumpSpeed) / (2*gravity); // 40 m es la altura maxima del salto ( en nuestro caso son pixeles)
		if(entityInfo->hasAttribute("Hmax"))
			Hmax = entityInfo->getFloatAttribute("Hmax");
		//Hmax = 40;

		//Pablo. Tiempo hasta alcanzar la m�xima altura
		// Vi - Vf / g (Velocidad inicial - Velocidad final / aceleracion). La Vf es 0 en altura max.
		Tmaxaltura = jumpSpeed / gravity; // 2,857142 segundos --> 2857,142 milisegundos
		Tmax = Tmaxaltura * 2; // 5,71428571 seg. --> 5714,28571 milisegundos.

		return true;
		}

	bool CAngularMovement::activate()
	{
		_sentidoDerecha=_entity->getSense();
		
		if (_entity->getType().compare("Player")!=0)
		if (_sentidoDerecha)
		{
			_walkingLeft=false;
			_walkingRight=true;
			_jumping = false; // Pablo
		}
		else
		{
			_walkingLeft=true;
			_walkingRight=false;
			_jumping = false; // Pablo
		}
		 //_actualRadius=
		 //if (_entity->getType().compare("Player")==0)
		 	//		std::cout<<"PlayerDegree: "<<_entity->getDegree()<<std::endl;
			return true;
	}
		

	void CAngularMovement::deactivate(){}

	
	bool CAngularMovement::accept(const TMessage &message)
	{
		if (_entity->getType().compare("Player")==0)
			return message._type == Message::CONTROL;
		 if (_entity->getType().compare("AnimatedEntity")==0)
			return message._type == Message::NPC_CONTROL;
	}

		
	 void CAngularMovement::process(const TMessage &message)
		 {
		switch(message._type)
		{
		case Message::CONTROL:
			{
			if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			else if(!message._string.compare("walkLeft"))
				walkLeft();
			else if(!message._string.compare("walkRight"))
				walkRight();
			else if(!message._string.compare("walkStop"))
				stopMovement();
			else if(!message._string.compare("jump")) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			else if(!message._string.compare("walkBack"))
				 {	
					_sentidoColision=message._bool;
					walkBack();
				 }
			else if(!message._string.compare("changeDirection"))
				 {	
					 _sentidoColision=message._bool;
					changeDirection(_sentidoColision);
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
			}
		case Message::NPC_CONTROL:
			{
			if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			else if(!message._string.compare("walkLeft"))
				walkLeft();
			else if(!message._string.compare("walkRight"))
				walkRight();
			else if(!message._string.compare("walkStop"))
				stopMovement();
			else if(!message._string.compare("jump")) // Pablo. Mensaje que viene de GUI::PlayerController::keyPressed
					jump();
			else if(!message._string.compare("walkBack"))
				 {	
					_sentidoColision=message._bool;
					walkBack();
				 }
			else if(!message._string.compare("changeDirection"))
				 {	
					 _sentidoColision=message._bool;
					changeDirection(_sentidoColision);
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
			}
		}
	}

		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animaci�n
			_entity->setSense(LogicalPosition::DERECHA);
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "RunKatana";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			
			// Cambiamos la animaci�n
			_entity->setSense(LogicalPosition::IZQUIERDA);
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "RunKatana";
			message._bool = true;
			_entity->emitMessage(message,this);

		}

		// Pablo
		void CAngularMovement::jump()
		{
			_initialJump = true;

		}
	
		void CAngularMovement::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
			// Cambiamos la animaci�n
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "IdleKatana";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		
		
		void CAngularMovement::goDown()
		{
			_changingRing=true;_entity->setRing(Ring::ANILLO_INFERIOR);
		}
		
		void CAngularMovement::goUp()
		{
			_changingRing=true;	
			_entity->setRing(Ring::ANILLO_SUPERIOR);
		}
		
		void CAngularMovement::stopMovement()
		{
			_walkingLeft = _walkingRight = false;

		// Cambiamos la animaci�n si no seguimos desplaz�ndonos
		// lateralmente
		
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "IdleKatana";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		
		
	void CAngularMovement::changeDirection(bool newDirection)
	{		
				Logic::TMessage m;
				m._string="marine";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
				if (_entity->getType().compare("Player")==0)
				return;
	
			//	if (_entity->getType().compare("AnimatedEntity")==0)
				//{
				if (!_walkingLeft && !_walkingRight)
				{
					if (newDirection)
						walkLeft();
					else
						walkRight();
				}
					//	}
			}
	


	void CAngularMovement::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);


			// Si nos estamos desplazando calculamos la pr�xima posici�n
			// Calculamos si hay vectores de direcci�n de avance y strafe,
			// hayamos la direcci�n de la suma y escalamos seg�n la
			// velocidad y el tiempo transcurrido.
	
			bool cierre=false;
			unsigned int currentTime;
			unsigned int endingTime;
			/*while(_changingRing){
				currentTime=Application::CBaseApplication::getSingletonPtr()->getAppTime();
			
			if (!cierre)
			{
				cierre=true;
				endingTime=currentTime+3000;		
			}
			if (currentTime>endingTime)
				_changingRing=false;
			_entity->setRing(LogicalPosition::ANILLO_SUPERIOR);
				IComponent::tick(msecs);
			}*/

			Vector3 direction(Vector3::ZERO);


			if(_walkingLeft || _walkingRight)
			{
				if(_walkingLeft || _walkingRight)
				{
					direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
					//Matrix4 orientacion = _entity->getOrientation();
					//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
					if(_walkingRight){
						if(_sentidoDerecha==true)
						{
							_sentidoDerecha=false;
							_entity->yaw(Math::PI);						
						}
						if (!_walkBack)
						{
							_entity->setDegree(_entity->getDegree()-_angularSpeed); 
							_entity->yaw(Math::fromDegreesToRadians(_angularSpeed));
						}
					}
					else
					{
						if(_sentidoDerecha==false)
						{
							_entity->yaw(Math::PI);					
							_sentidoDerecha=true;
						}
						if (!_walkBack)
						{
							_entity->setDegree(_entity->getDegree()+_angularSpeed);
							_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed));
						}			
					
					}

					if (_walkBack)
						{
							stopMovement();   
							_walkBack=false;

							if (_sentidoColision) // la direccion
							{
								_entity->setDegree(_entity->getDegree()+(_angularSpeed+10)); 
								_entity->yaw(Math::fromDegreesToRadians(-(_angularSpeed)));
							}
							else
							{
								_entity->setDegree(_entity->getDegree()-(_angularSpeed+10)); 
								_entity->yaw(Math::fromDegreesToRadians((_angularSpeed)));
							}							
				
							_sentidoColision=false;
						
						}
					if(_walkingLeft)
						direction *= -1;
				}

				direction.normalise();
				_entity->getPosition();

				Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
			
				//newPosition.y=_entity->getY();
				_entity->setPosition(newPosition);
				direction.normalise();
			
			}


			// Pablo. Salto



			if (_entity->getType().compare("Player")==0)
			{
					if(_initialJump==true && _jumping==false)
					{
						// Pablo. Inicializo a false _initialJump para que solo lo tenga en cuenta una vez.
						_initialJump = false;
						_jumping = true;
						_timeJumping = 0;
						Vector3 newPositionInitial=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						//inicialY = _entity->getY(_entity->getBase(),_entity->getRing());
						inicialY = newPositionInitial.y;
						Vector3 newPosition2=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());
						posicionSalto = newPosition2;
			
					}

					//Pablo
					if(_jumping)
					{
						//std::cout << "inicialY: " << inicialY << "\n";
						//std::cout << "posicionSalto.y: " << posicionSalto.y << "\n";

						//_timeJumping se incrementa en cada tick
						//_timeJumping+= msecs;

						//Vector3 newPosition2=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getBase(),_entity->getRing());

						//float altura = jumpSpeed*_timeJumping -(gravity*(_timeJumping*_timeJumping)/2);

						//newPosition.y = inicialY + altura;

						//si esta saltando, y esta bajando decrementamos la y en 1 unidad cada tick de reloj
						if(_jumpingDown){
							posicionSalto.y-= 1;
						}
						else //si esta saltando y esta subiendo incrementamos la y en 1 unidad cada tick de reloj
						{
							posicionSalto.y+= 1;
						}


						//control para que no suba m�s arriba que su inicialY + la altura maxima del salto
						if(posicionSalto.y>=inicialY+Hmax)
						{
							_jumpingDown=true;
						}
						//control para que no baje m�s que su inicialY
						if(posicionSalto.y<inicialY)
						{
							posicionSalto.y=inicialY;
							_jumpingDown=false;
							_jumping=false;
						}


						_entity->setPosition(posicionSalto);
						direction.normalise();

						/*
						if(_timeJumping >= Tmax ) {
							_jumping = false;
						}
						*/

					} //fin del if (_jumping)
			}// FIN if (_entity->getType().compare("Player")!=0)

			// Fin Pablo. Salto
		
		} //fin de CAngularMovement:tick



		void CAngularMovement::turn(float amount)
		{
				_entity->yaw(amount);		
		}

} // namespace Logic