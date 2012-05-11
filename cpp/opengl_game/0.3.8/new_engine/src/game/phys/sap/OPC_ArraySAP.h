///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains an array-based version of the sweep-and-prune algorithm
 *	\file		OPC_ArraySAP.h
 *	\author		Pierre Terdiman
 *	\date		December, 2, 2007
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPC_ARRAYSAP_H
#define OPC_ARRAYSAP_H

#include "SweepAndPrune.h"


#pragma pack(1)
	struct OPCODE_API ASAP_Pair
	{
		uword		id0;
		uword		id1;
		const void*	object0;
		const void*	object1;
//#ifdef PAIR_USER_DATA
		void*		userData;
//#endif
		inline_		const void*	GetObject0()		const	{ return (const void*)(size_t(object0) & ~3);		}
		inline_		const void*	GetObject1()		const	{ return (const void*)(size_t(object1) & ~3);		}
		inline_		size_t		IsInArray()			const	{ return size_t(object0) & 1;						}
		inline_		size_t		IsRemoved()			const	{ return size_t(object1) & 1;						}
		inline_		size_t		IsNew()				const	{ return size_t(object0) & 2;						}
		private:
		inline_		void		SetInArray()				{ object0 = (const void*)(size_t(object0) | 1);		}
		inline_		void		SetRemoved()				{ object1 = (const void*)(size_t(object1) | 1);		}
		inline_		void		SetNew()					{ object0 = (const void*)(size_t(object0) | 2);		}
		inline_		void		ClearInArray()				{ object0 = (const void*)(size_t(object0) & ~1);	}
		inline_		void		ClearRemoved()				{ object1 = (const void*)(size_t(object1) & ~1);	}
		inline_		void		ClearNew()					{ object0 = (const void*)(size_t(object0) & ~2);	}

		friend class ArraySAP;
	};
#pragma pack()

	class OPCODE_API ASAP_PairManager
	{
		public:
									ASAP_PairManager();
									~ASAP_PairManager();

				void				Purge();
				void				ShrinkMemory();

				const ASAP_Pair*	AddPair		(uword id0, uword id1, const void* object0, const void* object1);
				bool				RemovePair	(uword id0, uword id1);
				bool				RemovePairs	(const IceCore::BitArray& array);
				const ASAP_Pair*	FindPair	(uword id0, uword id1)	const;
		inline_	udword				GetPairIndex(const ASAP_Pair* pair)	const
									{
										return ((udword)((size_t(pair) - size_t(mActivePairs)))/sizeof(ASAP_Pair));
									}

				udword				mHashSize;
				udword				mMask;
				udword				mNbActivePairs;
				udword*				mHashTable;
				udword*				mNext;
				ASAP_Pair*			mActivePairs;
		inline_	ASAP_Pair*			FindPair(uword id0, uword id1, udword hash_value) const;
				void				RemovePair(uword id0, uword id1, udword hash_value, udword pair_index);
				void				ReallocPairs();
	};

	typedef void* (*SAP_CreatePair)(const void* object0, const void* object1, void* user_data);
	typedef void  (*SAP_DeletePair)(const void* object0, const void* object1, void* user_data, void* pair_user_data);

	// Forward declarations
	class ASAP_EndPoint;
	class ASAP_Box;
	struct IAABB;
	struct CreateData;

	class OPCODE_API ArraySAP : public IceCore::Allocateable
	{
		public:
									ArraySAP();
									~ArraySAP();

				udword				AddObject(void* object, uword guid, const ASAP_AABB& box);
				bool				RemoveObject(udword handle);
				bool				UpdateObject(udword handle, const ASAP_AABB& box);

				udword				DumpPairs(SAP_CreatePair create_cb, SAP_DeletePair delete_cb, void* cb_user_data, ASAP_Pair** pairs=null);

						const	ASAP_EndPoint*		GetEndPoints(udword axis) { return mEndPoints[axis]; }
		const	ASAP_Box*			GetBoxes() { return mBoxes; }
		private:
				IceCore::Container			mData;
				ASAP_PairManager	mPairs;

		inline_	void				AddPair(const void* object0, const void* object1, uword id0, uword id1);
		//							{
		//								ASSERT(object0);
		//								ASAP_Pair* UP = (ASAP_Pair*)mPairs.AddPair(id0, id1, null, null);
		//								ASSERT(UP);

		//								if(UP->object0)
		//								{
		//									// Persistent pair
		//								}
		//								else
		//								{
		//									// New pair
		//									ASSERT(!(int(object0)&1));
		//									ASSERT(!(int(object1)&1));
		//									UP->object0 = object0;
		//									UP->object1 = object1;
		//									UP->SetInArray();
		//									mData.Add(mPairs.GetPairIndex(UP));
		//									UP->SetNew();
		//									//TRACEMSG("Добавлена пара:", id0, id1)
		//								}
		//								UP->ClearRemoved();
		//							}

		inline_	void				RemovePair(const void* object0, const void* object1, uword id0, uword id1);
		//							{
		//								ASAP_Pair* UP = (ASAP_Pair*)mPairs.FindPair(id0, id1);
		//								if(UP)
		//								{
		//									if(!UP->IsInArray())
		//									{
		//										UP->SetInArray();
		//										mData.Add(mPairs.GetPairIndex(UP));
		//									}
		//									UP->SetRemoved();
		//								}
		//							}

				udword				mNbBoxes;
				udword				mMaxNbBoxes;
				ASAP_Box*			mBoxes;
				ASAP_EndPoint*		mEndPoints[3];
				udword				mFirstFree;

				void				ResizeBoxArray();
			// For batch creation
				IceCore::Container			mCreated;
				void				BatchCreate();
				void				InsertEndPoints(udword axis, const ASAP_EndPoint* end_points, udword nb_endpoints);
				bool				CompleteBoxPruning2(udword nb, const IAABB* array, const Axes& axes, const CreateData* batched);
				bool				BipartiteBoxPruning2(udword nb0, const IAABB* array0, udword nb1, const IAABB* array1, const Axes& axes, const CreateData* batched, const udword* box_indices);
			// For batch removal
				IceCore::Container			mRemoved;
				void				BatchRemove();


	};

#ifdef USE_WORDS
	typedef uword			IndexType;
#define	INVALID_INDEX	0xffff
#else
	typedef udword			IndexType;
#define	INVALID_INDEX	0xffffffff
#endif

#ifdef USE_INTEGERS
	typedef udword	ValType;
	typedef IAABB	SAP_AABB;
#else
	typedef float	ValType;
	typedef ASAP_AABB	SAP_AABB;
#endif


	class Opcode::ASAP_EndPoint : public IceCore::Allocateable
	{
	public:
		inline_					ASAP_EndPoint()		{}
		inline_					~ASAP_EndPoint()	{}

		ValType			mValue;		// Min or Max value
		udword			mData;		// Parent box | MinMax flag
	public:

		inline_	bool			IsSentinel()	const	{ return (mData&~3)==0xfffffffc;	}

		inline_	void			SetData(ValType v, udword owner_box_id, BOOL is_max)
		{
			mValue = v;
			mData = owner_box_id<<2;
			if(is_max)	mData |= 3;
		}
		inline_	BOOL			IsMax()		const	{ return mData & 3;		}
		inline_	udword			GetOwner()	const	{ return mData>>2;		}
	};

	class Opcode::ASAP_Box : public IceCore::Allocateable
	{
	public:
		inline_					ASAP_Box()	{}
		inline_					~ASAP_Box()	{}

		IndexType		mMin[3];
		IndexType		mMax[3];
		void*			mObject;
		udword			mGUID;

		inline_	ValType			GetMaxValue(udword i, const ASAP_EndPoint* base)	const
		{
			return base[mMax[i]].mValue;
		}

		inline_	ValType			GetMinValue(udword i, const ASAP_EndPoint* base)	const
		{
			return base[mMin[i]].mValue;
		}
#ifdef _DEBUG
		bool			HasBeenInserted()	const
		{
			assert(mMin[0]!=INVALID_INDEX);
			assert(mMax[0]!=INVALID_INDEX);
			assert(mMin[1]!=INVALID_INDEX);
			assert(mMax[1]!=INVALID_INDEX);
			assert(mMin[2]!=INVALID_INDEX);
			assert(mMax[2]!=INVALID_INDEX);
			return true;
		}
#endif
	};


	inline_ BOOL Intersect2D(const ASAP_Box& c, const ASAP_Box& b, udword axis1, udword axis2)
	{
		if(		b.mMax[axis1] < c.mMin[axis1] || c.mMax[axis1] < b.mMin[axis1]
		||	b.mMax[axis2] < c.mMin[axis2] || c.mMax[axis2] < b.mMin[axis2])	return FALSE;
		return TRUE;
	}

	inline_ BOOL Intersect1D_Min(const SAP_AABB& a, const ASAP_Box& b, const ASAP_EndPoint* const base, udword axis)
	{
		if(b.GetMaxValue(axis, base) < a.GetMin(axis))
			return FALSE;
		return TRUE;
	}



#endif // OPC_ARRAYSAP_H
