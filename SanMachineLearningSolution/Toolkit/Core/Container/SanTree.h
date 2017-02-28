//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add tree container [_stree, _swtree, dfs_iterator, bfs_iterator]
#include"SanContainerDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDCONTAINER_TREE_H__
#define __STDCONTAINER_TREE_H__

#pragma region San weighted tree structure defination
	template<class _data>
	struct _TREENODE
	{
	public:
		_TREENODE(){};
		~_TREENODE(){};

		virtual SString iPrintNode(const _TREENODE &Node) const { return SString(); };
	};

	/*Tree defination*/
	template<class _data, class _weight = sfloat>
	struct _TREE //Need update, some function has error
	{
	public:
		typedef _TREE<_data, _weight> _type;
	private:
		uint32			ID;						// Local ID
		_data			Data;					// Data, for AI stand for current node state
		uint32			Depth;					// Current depth
		_type*			pTrunk;					// Trunk pointer, if null, root node
		vector<_type*>	OffshootPtrArray;		// Offshoot pointer array, if empty, leaf node
		vector<_weight>	OffshootWeightArray;	// Offshoot weight array
		_weight			GlobalWeight;			// Glboal weight, for build the heap
	protected:
		void _Reshape(const uint32 ID, const uint32 Depth)
		{
			this->ID = ID;
			this->Depth = Depth;

			typename vector<_type*>::iterator pOffshoot = this->OffshootPtrArray.begin();
			typename vector<_weight>::iterator pWeight = this->OffshootWeightArray.begin();
			while (pOffshoot != this->OffshootPtrArray.end())
			{
				if ((*pOffshoot) == nullptr)
				{
					pOffshoot = this->OffshootPtrArray.erase(pOffshoot);
					pWeight = this->OffshootWeightArray.erase(pWeight);
				}
				else
				{
					pOffshoot++;
					pWeight++;
				}
			}
			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->_Reshape(seek, this->Depth + 1);
			}
		}
	public:
		_TREE(const uint32 ID = 0, _type* pTrunk = nullptr)
			:ID(ID), Depth(0), pTrunk(pTrunk), GlobalWeight(0)
		{
		};

		_TREE(const uint32 ID, const _data &Data, const uint32 Depth, _type* pTrunk)
			:ID(ID), Data(Data), Depth(Depth), pTrunk(pTrunk), GlobalWeight(0)
		{
		};

		_TREE(const _type &Tree) = delete;

		~_TREE()
		{
			if (this->pTrunk != nullptr)
			{
				this->pTrunk->OffshootPtrArray[this->ID] = nullptr;
			}

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				if (this->OffshootPtrArray[seek] != nullptr)
				{
					delete this->OffshootPtrArray[seek];
					this->OffshootPtrArray[seek] = nullptr;
				}
			}
			this->ID = 0;
			this->OffshootPtrArray.clear();
			this->OffshootWeightArray.clear();

			this->pTrunk = nullptr;
		};

		_type& operator=(const _type Tree) = delete;

		/*Operator Ovarload*/
		const _type& operator[](const size_t Position) const
		{
			if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
			return *this->OffshootPtrArray[Position];
		};

		_type& operator[](const size_t Position)
		{
			if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
			return *this->OffshootPtrArray[Position];
		};

		/*For heap use*/
		bool operator()(const _type &Tree1, const _type &Tree2)
		{
			return Tree1.GlobalWeight < Tree2.GlobalWeight;
		};

		bool operator<(const _type &Tree)
		{
			return this->GlobalWeight < Tree.GlobalWeight;
		};

		bool operator==(const _type &Tree)
		{
			if (this->Data != Tree.Data){ return false; }
			if (this->GlobalWeight != Tree.GlobalWeight){ return false; }
			if (this->OffshootPtrArray.size() != Tree.OffshootPtrArray.size()){ return false; }
			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				if (this->OffshootWeightArray[seek] != Tree.OffshootWeightArray[seek]){ return false; }
				if ((*this->OffshootPtrArray[seek]) != (*Tree.OffshootPtrArray[seek])){ return false; }
			}
			return true;
		};

		bool operator!=(const _type &Tree)
		{
			return !(*this == Tree);
		};

		/*Basic tree functions*/
		void iSetID(const uint32 ID)
		{
			this->ID = ID;
		};

		void iSetData(const _data &Data)
		{
			this->Data = Data;
		};

		bool iResize(const uint32 Size, const _data &DefaultData, const _weight &DefaultWeight)
		{
			if (this->OffshootPtrArray.size() == Size)
			{
				return true;
			}

			if (this->OffshootPtrArray.size() < Size)
			{
				this->OffshootPtrArray.resize(Size);
				this->OffshootWeightArray.resize(Size);

				for (uint32 seek = this->OffshootPtrArray.size(); seek < Size; seek = seek + 1)
				{
					this->OffshootPtrArray[seek] = new _type(seek, DefaultData, this->Depth + 1, this);
					this->OffshootWeightArray[seek] = DefaultWeight;
				}
				return true;
			}
			else
			{
				for (uint32 seek = (Size - 1) > 0 ? (Size - 1) : 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						//this->OffshootPtrArray[seek] = nullptr;
					}
				}
				this->OffshootPtrArray.resize(Size);
				this->OffshootWeightArray.resize(Size);
				return true;
			}
		};

		_type* iCreate(const _data &Data, const _weight &Weight)
		{
			_type* pNode = new  _type(this->OffshootPtrArray.size(), Data, this->Depth + 1, this);

			this->OffshootPtrArray.push_back(pNode);
			this->OffshootWeightArray.push_back(Weight);

			return pNode;
		};

		_type* iCreate(const _type &Tree, const _weight &Weight)
		{
			_type* pNode = Tree.iCopy();

			pNode->pTrunk = this;

			pNode->_Reshape(this->OffshootPtrArray.size(), this->Depth + 1);

			this->OffshootPtrArray.push_back(pNode);
			this->OffshootWeightArray.push_back(Weight);

			return pNode;
		};

		_type* iInsert(const uint32 ID, const _data &Data, const _weight &Weight)
		{
			if (this->OffshootPtrArray.size() < ID)
			{
				return nullptr;
			}

			/*Search for the target node*/
			typename vector<_type*>::iterator pItem = this->OffshootPtrArray.begin();
			typename vector<_weight>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}

			/*Insert*/
			_type* pTreeNode = new _type(ID, Data, this->Depth, this);

			this->OffshootPtrArray.insert(pItem, pTreeNode);
			this->OffshootWeightArray.insert(pWeightItem, Weight);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}

			this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

			return this->OffshootPtrArray[ID];
		};

		_type* iInsert(const uint32 ID, const _type &TreeNode, const _weight &Weight)
		{
			if (this->OffshootPtrArray.size() < ID)
			{
				return nullptr;
			}

			/*Search for the target node*/
			typename vector<_type*>::iterator pItem = this->OffshootPtrArray.begin();
			typename vector<_weight>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}

			/*Insert*/
			_type* pTreeNode = TreeNode.iCopy();

			pTreeNode->pTrunk = this;

			this->OffshootPtrArray.insert(pItem, pTreeNode);
			this->OffshootWeightArray.insert(pWeightItem, Weight);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}

			this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

			return this->OffshootPtrArray[ID];
		};

		bool iDelete(const uint32 ID)
		{

			if (this->OffshootPtrArray.size() <= ID)
			{
				return false;
			}

			/*If target node exist, delete the target node*/
			if (this->OffshootPtrArray[ID] != nullptr)
			{
				delete this->OffshootPtrArray[ID];
				this->OffshootPtrArray[ID] = nullptr;
			}

			/*Erase the target node from the array*/
			typename vector<_type*>::iterator pItem = this->OffshootPtrArray.begin();
			typename vector<_weight>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}
			this->OffshootPtrArray.erase(pItem);
			this->OffshootWeightArray.erase(pWeightItem);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}
		};

		void iCopy(_type &Tree) const
		{
			Tree.Data = this->Data;
			Tree.pTrunk = nullptr;// this->pTrunk;

			for (auto &NodePtr : Tree.OffshootPtrArray)
			{
				delete NodePtr;
				NodePtr = nullptr;
			}

			Tree.OffshootPtrArray.resize(this->OffshootPtrArray.size());
			for (uint32 seek = 0; seek < Tree.OffshootPtrArray.size(); seek = seek + 1)
			{
				Tree.OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
			}

			Tree.OffshootWeightArray = this->OffshootWeightArray;
			Tree.GlobalWeight = this->GlobalWeight;

			Tree._Reshape(0, 0);
		};

		_type* iCopy() const
		{
			_type* pTree = new _type();

			pTree->Data = this->Data;
			pTree->Depth = this->Depth;

			pTree->OffshootPtrArray.resize(this->OffshootPtrArray.size());
			for (uint32 seek = 0; seek < pTree->OffshootPtrArray.size(); seek = seek + 1)
			{
				pTree->OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
			}
			pTree->OffshootWeightArray = this->OffshootWeightArray;
			pTree->GlobalWeight = this->GlobalWeight;

			pTree->_Reshape(0, 0);
			return pTree;
		};

		void iClear(bool bDeleteObject = true)
		{
			if (bDeleteObject)
			{
				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						this->OffshootPtrArray[seek] = nullptr;
					}
				}
			}
			this->OffshootPtrArray.clear();
			this->OffshootWeightArray.clear();
		};

		bool iSetOffshootWeight(const uint32 ID, const _weight Weight)
		{
			if (ID >= this->OffshootWeightArray.size())
			{
				return false;
			}
			this->OffshootWeightArray[ID] = Weight;
			return true;
		};

		_weight iSetGlobalWeight(_weight GlobalWeight)
		{
			this->GlobalWeight = GlobalWeight;
			return this->GlobalWeight;
		};

		const uint32 iGetID() const
		{
			return this->ID;
		};

		_data iGetData() const
		{
			return Data;
		};

		const _data* iGetDataPtrConst() const
		{
			return &Data;
		};

		_data* iGetDataPtr()
		{
			return &Data;
		};

		uint32 iGetDepth() const
		{
			return Depth;
		};

		const _type* iGetTrunkPtrConst() const
		{
			return this->pTrunk;
		};

		_type* iGetTrunkPtr() const
		{
			return this->pTrunk;
		};

		const uint32 iGetSize() const
		{
			return this->OffshootPtrArray.size();
		};

		const _type* iGetOffshootPtrConst(const uint32 ID) const
		{
			if (ID >= this->OffshootPtrArray.size())
			{
				return nullptr;
			}
			return this->OffshootPtrArray[ID];
		};

		_type* iGetOffshootPtr(const uint32 ID) const
		{
			if (ID >= this->OffshootPtrArray.size())
			{
				return nullptr;
			}
			return this->OffshootPtrArray[ID];
		};

		_weight iGetOffshootWeight(const uint32 ID) const
		{
			if (ID >= this->OffshootWeightArray.size())
			{
				return 0.0;
			}
			return this->OffshootWeightArray[ID];
		};

		_weight iGetGlobalWeight() const
		{
			return this->GlobalWeight;
		};

		uint32 iGetRelativeMaxDepth() const
		{
			//this->_Reshape(this->ID, this->Depth);

			uint32 MaxDepth = 0;

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				uint32 CurrentDepth = this->OffshootPtrArray[seek]->iGetRelativeMaxDepth() + 1;
				if (CurrentDepth>MaxDepth)
				{
					MaxDepth = CurrentDepth;
				}
			}

			return MaxDepth;// -this->Depth + 1;
		};

		SString iPrintTree(const SString &strBaseString = _SSTR("   ")) const
		{
			SString strOutput;

			for (uint32 seek = 0; seek < this->Depth; seek = seek + 1)
			{
				strOutput = strOutput + strBaseString;
			}

			strOutput = strOutput + this->Data.iPrintNode() + _SSTR("\r\n");

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				strOutput = strOutput + this->OffshootPtrArray[seek]->iPrintTree();
			}

			return strOutput;
		}

		bool iIsRoot() const
		{
			return (this->pTrunk == nullptr);
		};

		bool iIsLeaf() const
		{
			return this->OffshootPtrArray.empty();
		};
	};
	template<class _data, class _weight = sfloat>
	using _swtree = _TREE<_data, _weight>;
#pragma endregion
#endif
}