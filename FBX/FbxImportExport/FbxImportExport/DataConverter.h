#pragma once
#ifndef DATACONVERTER_H

#include <fbxsdk.h>
#include <vector>
#include <string>

using namespace std;

class DataConverter {
private:
	int addCount = 0;
	//char* currPointer = NULL;

	vector<char*> pointers;
	vector<size_t> pointerSizes;

	size_t currSize = 0;
public:
	DataConverter()
	{
		addCount = 0;
	}

    void Clear()
    {
        addCount = 0;

        for (int i = 0; i < pointers.size(); i++)
        {
            free (pointers[i]);
        }

        pointers.clear();
        pointerSizes.clear();
    }

	template <class T>
	void Add(T data)
	{
		size_t size = sizeof(data);
		char* newData = reinterpret_cast<char *>(&data);

		if (this->addCount == 0)
		{
			currSize = size;
		}
		else
		{
			currSize = size + currSize;
		}

		char* tmpPointer = new char[size];
		memcpy(tmpPointer, newData, size);

		pointers.push_back(tmpPointer);
		pointerSizes.push_back(size);

		this->addCount++;
	}

	void Add(string data)
	{
		size_t size = sizeof(char) * data.length();
		char* newData = new char[size];

		if (this->addCount == 0)
		{
			currSize = size;
		}
		else
		{
			currSize = size + currSize;
		}

		memcpy(newData, data.c_str(), size);
		pointers.push_back(newData);
		pointerSizes.push_back(size);

		this->addCount++;
	}

	void Add(char* dataPointer, size_t dataSize)
	{
		if (this->addCount == 0)
		{
			currSize = dataSize;
		}
		else
		{
			currSize = dataSize + currSize;
		}

		pointers.push_back(dataPointer);
		pointerSizes.push_back(dataSize);

		this->addCount++;
	}

	char* Get()
	{
		char* allPointers = new char[currSize];

		if (pointers.size() != 0)
		{
			memcpy(allPointers, pointers[0], pointerSizes[0]);

			size_t accumOffset = pointerSizes[0];
			for (int i = 1; i < pointers.size(); i++)
			{
				memcpy(allPointers + accumOffset, pointers[i], pointerSizes[i]);
				accumOffset += pointerSizes[i];
			}

			return allPointers;
		}
		else
		{
			return NULL;
		}
	}

	size_t Size()
	{
		return currSize;
	}
};

#endif