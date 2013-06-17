/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/

#ifndef H_WJ_MaterialGenerator
#define H_WJ_MaterialGenerator

#include "OgreMaterial.h"
#include "OgreStringVector.h"

/** Caching, on-the-fly material generator. This is a class that automatically 
	generates and stores different permutations of a material, and its shaders. 
	It can be used if you have a material that has lots of slightly different 
	variations, like whether to use a specular light, skinning, normal mapping 
	and other options. Writing all these out is a tedious job. Of course it is 
	possible to always use the material with all features, but that might result 
	in large, slow shader programs. This class provides an efficient solution
	to that.
 */
class MaterialGenerator
{
public:
	/** Bitfield used to signify a material permutations */
	typedef Ogre::uint32 Perm;
	virtual ~MaterialGenerator();

	const Ogre::MaterialPtr &getMaterial(Perm permutation);

	/** Implementation class that takes care of actual generation or lookup
		of the various constituent parts (template material, fragment shader
		and vertex shader). These methods are only called once for every permutation,
		after which the result is stored and re-used.
	*/
	class Impl
	{
	public:
		virtual ~Impl();
		virtual Ogre::GpuProgramPtr generateVertexShader(Perm permutation) = 0;
		virtual Ogre::GpuProgramPtr generateFragmentShader(Perm permutation) = 0;
		virtual Ogre::MaterialPtr generateTemplateMaterial(Perm permutation) = 0;
	};
protected:
	/** The constructor is protected as this base class should never be constructed
		as-is. It is meant to be subclassed so that values can be assigned to
		the various fields controlling material generator, and most importantly, the
		mImpl field.
	*/
	MaterialGenerator();

	const Ogre::GpuProgramPtr &getVertexShader(Perm permutation);
	const Ogre::GpuProgramPtr &getFragmentShader(Perm permutation);
	const Ogre::MaterialPtr &getTemplateMaterial(Perm permutation);
	
	/// Base name of materials generated by this
	Ogre::String materialBaseName;
	/// Mask of permutation bits that influence vertex shader choice
	Perm vsMask;
	/// Mask of permutation bits that influence fragment shader choice
	Perm fsMask;
	/// Mask of permutation bits that influence template material choice
	Perm matMask;
	/// Generator
	Impl *mImpl;

	typedef Ogre::map<Perm, Ogre::GpuProgramPtr>::type ProgramMap;
	typedef Ogre::map<Perm, Ogre::MaterialPtr>::type MaterialMap;

	ProgramMap mVs, mFs;
	MaterialMap mTemplateMat, mMaterials;
};

#endif
