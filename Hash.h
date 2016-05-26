#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;

enum Status
{
	EXIST,
	DELETE,
	EMPTY,

};
template<class K,class V>
struct KeyValue
{
	K _key;
	V _value;

	KeyValue(const K&key = K(), const V&value = V())
		:_key(key)
		,_value(value)
	{}
	
};
template <class K>
struct DefaultHashFuncer
{
	size_t operator()(const K&key)
	{
		return key;
	}
};
struct HashFuncerString
{
	size_t operator()(const string&str)
	{
		size_t value = 0;
		for (size_t i = 0; i < str.size(); ++i)
		{
			value += str[i];
		}
		return value;
	}
};
template<class K,class V,class HashFuncer=DefaultHashFuncer<K>>
class HashTable
{
	typedef KeyValue<K,V>  KV;
public:
	HashTable()
		:_tables(NULL)
		, _size(0)
		, _capacity(0)
		, _Status(NULL)
	{}

	HashTable(size_t size)
		:_tables(new KV[size])
		, _size(0)
		, _capacity(size)
		, _Status(new Status[size])
	{
		for (int i = 0; i <_capacity; ++i)
		{
			_Status[i] = EMPTY;
		}
	}
	
	HashTable(HashTable<K,V,HashFuncer>&ht)
	{
		HashTable<K,V, HashFuncer>tmp(ht._capacity);
		for (size_t i = 0; i < ht._capacity;++i)
		{
			if (ht._Status[i] == EXIST)
			{
				tmp.Insert(ht._tables[i]);
			}
		}
		tmp._capacity = ht._capacity;
		tmp._size = ht._size;
		this->Swap(tmp);
	}

	HashTable<K,V,HashFuncer>& operator=(HashTable<K,V,HashFuncer> &ht)
	{
		HashTable<K,V, HashFuncer>tmp(ht);
		this->Swap(tmp);
		return *this;
	}

	~HashTable()
	{
		if (_tables)
		{
			delete[]_tables;
			delete[]_Status;
			_size = 0;
			_capacity = 0;
		}
	}

	size_t _HashFunc(const K&key)
	{
		HashFuncer(hf);
		return hf(key)%_capacity;
	}

	bool Insert(const K&key,const V&value)
	{
		/*if (_size == _capacity)
		{
			cout << "Full" << endl;
			return false;
		}*/
		_CheckCapacity();
		size_t index = _HashFunc(key);
		while (_Status[index] == EXIST)
		{
			if (_tables[index]._key == key)
			{
				return false;
			}
			++index;
			if (index == _capacity)
			{
				index = 0;
			}
		}
		_Status[index] = EXIST;
		_tables[index]._key = key;
		_tables[index]._value = value;
		++_size;
		return true;
	}

	bool Find(const K&key)
	{
		size_t index = _HashFunc(key);
		while (_Status[index] != EMPTY)
		{
			if (_tables[index]._key == key&&_Status[index] != DELETE)
			{
				return true;
			}
			++index;
			if (index == _capacity)
			{
				index = 0;
			}
		}
		return false;
	}

	bool Remove(const K&key)
	{
		int index = Find(key); 
		if (index != -1)
		{
			_Status[index] = DELETE;
			return true;
		}
		return false;
	}

	
	void Swap(HashTable<K,V, HashFuncer>&ht)
	{
		swap(_tables, ht._tables);
		swap(_Status, ht._Status);
		swap(_size, ht._size);
		swap(_capacity, ht._capacity);
	}

	void PrintTablesD()
	{
		for (size_t i = 0; i < _capacity; ++i)
		{
			if (_Status[i] == EXIST)
			{
				printf("[%d]:E->[%d,%d]\n", i, _tables[i]._key, _tables[i]._value);
			}
			else if (_Status[i] == DELETE)
			{
				printf("[%d]:D->NULL\n", i);
			}
			else
			{
				printf("[%d]:N->NULL\n", i);
			}
		}
	}

	void PrintTablesS()
	{
		for (size_t i = 0; i < _capacity; ++i)
		{
			if (_Status[i] == EXIST)
			{
				printf("[%d]:E->[%s,%s]\n", i, _tables[i]._key.c_str(), _tables[i]._value.c_str());
			}
			else if (_Status[i] == DELETE)
			{
				printf("[%d]:D->NULL\n", i);
			}
			else
			{
				printf("[%d]:N->NULL\n", i);
			}
		}
	}
protected:
	void _CheckCapacity()
	{
		if (_size * 10 >= _capacity * 7)
		{
			HashTable<K,V, HashFuncer>tmp(2 * _capacity);
			for (size_t i = 0; i < _capacity; ++i)
			{
				if (_Status[i] == EXIST)
				{
					tmp.Insert(_tables[i]._key, _tables[i]._value);
				}
			}
			this->Swap(tmp);
		}
		
	}

protected:
	KV* _tables;
	size_t _size;
	size_t _capacity;
	Status* _Status;
};
