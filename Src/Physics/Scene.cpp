//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp


@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#include "Scene.h"
#include "Logic\Server.h"
#include "Server.h"
#include "Actor.h"
#include "BaseSubsystems/Server.h"

#include <algorithm>
#include <assert.h>



namespace Physics
{

	CScene::CScene() {}


	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();

	} // ~CScene

	//--------------------------------------------------------

	void CScene::addActor(CActor* actor)
	{

		_actors.push_back(*actor);
	

	} // addEntity

	//--------------------------------------------------------


	void CScene::removeActor(CActor* actor)
	{
		std::vector<CActor>::iterator position = std::find(_actors.begin(), _actors.end(), actor);
		if (position != _actors.end()) 
			_actors.erase(position);

	} // addEntity

	//--------------------------------------------------------

	void CScene::activate()
	{

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::simulate(int msecs)
	{	
		//WTF!!
		for (int i = 0; i < _actors.size() - 1; ++i)
			for (int j = i + 1; j < _actors.size(); ++j)
				if(_actors[i].intersects(_actors[j]))
				{
					// tratar colision
				}
	} // tick	

	//--------------------------------------------------------


} // namespace Physics