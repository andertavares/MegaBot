#pragma once

#include <vector>

template <class Key, class Val>
class Heap
{
public:
	Heap(bool isMinHeap = false) : mIsMinHeap(isMinHeap) {}

	void push(const std::pair<Key, Val> &val)
	{
		int index = mData.size();
		if(mMapping.insert(std::make_pair(val.first, index)).second)
		{
			mData.push_back(val);
			percolate_up(index);
		}
	}

	void pop()
	{
		if(mData.empty())
			return;

		mMapping.erase(mData.front().first);
		mData.front() = mData.back();
		mData.pop_back();

		if(mData.empty())
			return;

		std::map<Key, int>::iterator it = mMapping.find(mData.front().first);
		if(it != mMapping.end())
		{
			it->second = 0;
			percolate_down(0);
		}
	}

	const std::pair<Key, Val> &top() const
	{
		return mData.front();
	}

	bool empty() const
	{
		return mData.empty();
	}

	bool set(const Key &key, const Val &val)
	{
		std::map<Key, int>::iterator it = mMapping.find(key);
		if(it == mMapping.end())
		{
			push(std::make_pair(key, val));
			return true;
		}

		int index = it->second;
		mData[index].second = val;
		index = percolate_up(index);

		if(index >= 0 && index < (int)mData.size())
		{
			percolate_down(index);
			return true;
		}

		return false;
	}

	const Val &get(const Key &key) const
	{
		std::map<Key, int>::const_iterator it = mMapping.find(key);
		int index = it->second;
		return mData[index].second;
	}

	bool contains(const Key &key) const
	{
		std::map<Key, int>::const_iterator it = mMapping.find(key);
		return (it != mMapping.end());
	}

	int size() const
	{
		return mData.size();
	}

	void clear()
	{
		mData.clear();
		mMapping.clear();
	}

	bool erase(const Key &key)
	{
		std::map<Key, int>::iterator it = mMapping.find(key);
		if(it == mMapping.end())
			return false;

		if(mData.size() == 1)
			clear();
		else
		{
			int index = it->second;
			mData[index] = mData.back();
			mData.pop_back();
			mMapping.erase(it);
			percolate_down(index);
		}

		return true;
	}

private:
	std::vector<std::pair<Key, Val>> mData;
	std::map<Key, int> mMapping;
	bool mIsMinHeap;

	int percolate_up(int index)
	{
		if(index < 0 || index >= (int)mData.size())
			return -1;

		unsigned int parent = (index - 1) / 2;
		int m = mIsMinHeap ? -1 : 1;

		while(index > 0 && m * mData[parent].second < m * mData[index].second)
		{
			std::swap(mData[parent], mData[index]);
			mMapping.find(mData[index].first)->second = index;
			index = parent;
			parent = (index - 1) / 2;
		}
		mMapping.find(mData[index].first)->second = index;

		return index;
	}

	int percolate_down(int index)
	{
		if(index < 0 || index >= (int)mData.size())
			return -1;

		unsigned int lchild = index * 2 + 1;
		unsigned int rchild = index * 2 + 2;
		unsigned int mchild;
		int m = mIsMinHeap ? -1 : 1;

		while((mData.size() > lchild && m * mData[index].second < m * mData[lchild].second) || (mData.size() > rchild && m * mData[index].second < m * mData[rchild].second))
		{
			mchild = lchild;
			if(mData.size() > rchild && m * mData[rchild].second > m * mData[lchild].second)
				mchild = rchild;

			std::swap(mData[mchild], mData[index]);
			mMapping.find(mData[index].first)->second = index;
			index = mchild;
			lchild = index * 2 + 1;
			rchild = index * 2 + 2;
		}

		mMapping.find(mData[index].first)->second = index;

		return index;
	}
};