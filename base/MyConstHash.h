/*
 * =====================================================================================
 *
 *       Filename:  MyConnHash.h
 *
 *    Description:  ConstHash
 *
 *        Version:  1.0
 *        Created:  09/28/2015 06:01:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huangyun (hy), 895175589@qq.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MYCONSTHASH_H_
#define __MYCONSTHASH_H_

#include "MyHashFunction.h"
#include <memory>
#include <map>
#include <set>
#include <string>
#include <stdint.h>
#include <iostream>
#include <sstream>

namespace MyNameSpace
{

	template <typename T>
		class RealNode
		{
			public:
				RealNode(const std::string & tags, 
						uint32_t vCnt,
						std::shared_ptr<T> data)
					:mNodeTags(tags), mVirtualCnt(vCnt), mNodeData(data)
				{

				}
				std::string GetNodeTags() { return mNodeTags; }
				uint32_t GetVirtualCount() { return mVirtualCnt; }
				std::shared_ptr<T> GetData() { return mNodeData;}
			private:
				std::string mNodeTags;
				uint32_t mVirtualCnt;
				std::shared_ptr<T> mNodeData;
		};

	template <typename T>
		class VirtualNode
		{
			public:
				VirtualNode(std::shared_ptr<RealNode<T>> node, uint32_t hash) : mRealNode(node), mHash(hash)
			{
			}

				std::shared_ptr<RealNode<T>> GetNode()
				{
					return mRealNode;
				}
				uint32_t GetHash()
				{
					return mHash;
				}
			private:
				std::shared_ptr<RealNode<T>> mRealNode;
				uint32_t mHash;
		};

	template <typename T>
		class MyConstHash
		{
			public:
				MyConstHash()
				{
					mHashFun = FNVHash;
				}
				~MyConstHash()
				{
					clearNode();
				}
				void SetHashFunc(HashFunction func)
				{
					mHashFun = func;
				}
				bool addNode(const std::string & nodeTags, uint32_t virtualNodeCount, std::shared_ptr<T>nodeData);
				void removeNode(const std::string & nodeTags);
				std::shared_ptr<T> searchNode(const std::string & str) const;
				uint32_t GetNodeCount() { return mRealNodeMap.size(); }
				uint32_t GetVirtualNodeCount() { return mVirtualNodeMap.size(); }
				void clearNode()
				{
					mRealNodeMap.clear();
					mVirtualNodeMap.clear();
				}

			public:
				static const uint32_t VIRTUAL_NODE_COUNT = 50;
			private:
				/*
				typedef std::map<uint32_t, std::shared_ptr<VirtualNode<T>>> VirtualNodeType;
				typedef std::map<std::string, std::shared_ptr<RealNode<T>>> RealNodeType;
				*/

				using VirtualNodeType = std::map<uint32_t, std::shared_ptr<VirtualNode<T>>>;
				using RealNodeType = std::map<std::string, std::shared_ptr<RealNode<T>>>;
				RealNodeType mRealNodeMap;
				VirtualNodeType mVirtualNodeMap;
				HashFunction mHashFun;
		};

	template <typename T>
	bool MyConstHash<T>::addNode(const std::string & nodeTags, uint32_t virtualNodeCount, std::shared_ptr<T>nodeData)
	{
		if (nodeTags.empty() || virtualNodeCount == 0)
		{
			std::cerr<<"addNode nodeTags: " << nodeTags << "virtualNodeCount: "<< virtualNodeCount <<std::endl;
			return false;
		}
		std::shared_ptr<RealNode<T>> pNode(new RealNode<T>(nodeTags, virtualNodeCount, nodeData));			
		if (!pNode)
		{
			std::cerr<<"addNode std::shared_ptr<RealNode<T>> return NULL"<<std::endl;
			return false;
		}
		std::string tmp = nodeTags;
		std::pair<typename RealNodeType::iterator, bool> realRes = mRealNodeMap.insert(std::make_pair(nodeTags, pNode));
		if (!(realRes.second))
		{
			std::cerr<<"addNode real node insert failure"<<std::endl;
			return false;
		}
		for (uint32_t i = 0; i < virtualNodeCount; ++i)
		{
			std::ostringstream os;
			os<<nodeTags<<"#"<<i;
			std::string str = os.str();
			uint32_t vhash = mHashFun(str);
			std::shared_ptr<VirtualNode<T>> pVirNode(new VirtualNode<T>(pNode, vhash));
			if (!pVirNode)
			{
				std::cerr<<"addNode std::shared_ptr<VirtualNode<T>> return NULL"<<std::endl;
				continue;
			}

			std::pair<typename VirtualNodeType::iterator, bool> virRes = mVirtualNodeMap.insert(std::make_pair(vhash, pVirNode));
			if (!(virRes.second))
			{
				std::cerr<<"addNode insert failure maybe hash conflict vhash:"<<vhash<<std::endl;
			}
		}
		return true;
	}

	template <typename T>
	void MyConstHash<T>::removeNode(const std::string & nodeTags)
	{
		typename RealNodeType::iterator iter = mRealNodeMap.find(nodeTags);	
		if (iter == mRealNodeMap.end())
		{
			std::cerr<<"removeNode not found tags:"<<nodeTags<<std::endl;
			return;	
		}
		std::shared_ptr<RealNode<T>> realNode = iter->second;
		mRealNodeMap.erase(iter);
		typename VirtualNodeType::iterator virIter = mVirtualNodeMap.begin();
		for ( ; virIter != mVirtualNodeMap.end(); )
		{
			if (virIter->second->GetNode() == realNode)
			{
				mVirtualNodeMap.erase(virIter++);
			}
			else
			{
				++virIter;
			}
		}
	}

	template <typename T>
	std::shared_ptr<T> MyConstHash<T>::searchNode(const std::string & str) const
	{
		if (mVirtualNodeMap.empty())
		{
			return std::shared_ptr<T>(NULL);
		}
		uint32_t hash = mHashFun(str);
		typename VirtualNodeType::const_iterator iter = mVirtualNodeMap.lower_bound(hash);
		if (iter == mVirtualNodeMap.end())
		{
			iter = mVirtualNodeMap.begin();
		}
		return iter->second->GetNode()->GetData();
	}
}
#endif
