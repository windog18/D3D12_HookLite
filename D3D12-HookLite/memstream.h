#pragma once

#include <windows.h>
#include <vector>
#include <d3d12.h>
#include "stdio.h"
#include "xdefine.h"
#include <sstream>
#include <mutex>
#include <iostream>
#include <fstream>

using namespace std;


//void SaveRecordToFile(const char* name);
//void ResetStream();

class MemStream
{

private:
	//static MemStream* instance;
	
	//unsigned char* memhandle;
	std::vector<unsigned char> memhandle;
	unsigned char* streamhandle;

	INT64 streamcount;
	

	stringstream nameListCache;

public:
	std::mutex m_gMutex;


public:
	MemStream();
	~MemStream();
	void init();
	//static MemStream* getstream();
	
	void writetoFile(const char* pname);

	void writeCommandNameToFile(const char *pname);

	void readfromfile(const char* pname);

	void reset();

	template <typename T>
	inline void write(T &el);

	template <typename T>
	inline void write(T *el);

	template <typename T>
	inline void writePointerValue(T *el);

	void write(CommandEnum enu);

	void write(const void* pdata, size_t datasize);

	void write(const D3D12_INPUT_LAYOUT_DESC& desc);

	void write(const D3D12_STREAM_OUTPUT_DESC& desc);

	void write(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);

	void write(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc);

	void write(const D3D12_COMMAND_SIGNATURE_DESC& desc);

	void read(D3D12_STREAM_OUTPUT_DESC& desc);

	void read(D3D12_COMMAND_SIGNATURE_DESC& desc);

	template <typename T>
	inline void read(T &el);

	template <typename T>
	inline void read(T *el);

	template <typename T>
	inline void readPointerValue(T *el);

	inline void read(CommandEnum& enu);

	void read(D3D12_INPUT_LAYOUT_DESC& desc);

	void read(const void* pdata, size_t datasize);

	

	inline void WriteToMemStream(const void *src, size_t tSize) {
		std::lock_guard<std::mutex> locker(m_gMutex);
		if (tSize + streamcount >= memhandle.size()) {
			size_t increSize = memhandle.size() * 2;
			if (tSize > memhandle.size()) {
				increSize = (size_t)(1.5 * tSize);
			}
			if (increSize > 1024 * 1024 * 1024) {
// 				Log_Detail_1(Enum_other1, "writeBuffer: %d, bufferSize: %d", streamcount + tSize, memhandle.size());
// 				Log_Detail_1(Enum_other1, "incre large memory: %.2f", (1.5 * tSize) / (1024 * 1024 * 1024));
			}
			memhandle.resize(increSize);
			streamhandle = &memhandle[streamcount];
		}
		memcpy(streamhandle, src, tSize);
		streamcount += tSize;
		streamhandle += tSize;

	}
};


template <typename T>
inline void MemStream::write(T &el)
{
// 	std::string t = typeid(T).name();
// 	std::string prefix = "[2020]  write T: ";
// 
// 	OutputDebugStringA((prefix + t).c_str());
	size_t datasize = sizeof(T);
// 	memcpy(streamhandle, &el, datasize);
// 	streamcount += datasize;
// 	streamhandle  += datasize;

	WriteToMemStream(&el, datasize);
}

template <typename T>
inline void MemStream::write(T* el)
{
// 	std::string t = typeid(T).name();
// 	std::string prefix = "[2020]  write T*: ";
// 	
// 	OutputDebugStringA((prefix + t).c_str());
	size_t datasize = sizeof(T*);
// 	memcpy(streamhandle, &el, datasize);
// 	streamcount += datasize;
// 	streamhandle = streamhandle + datasize;
	WriteToMemStream(&el, datasize);

}

template <typename T>
inline void MemStream::writePointerValue(T *el)
{
// 	std::string t = typeid(T).name();
// 	std::string prefix = "[2020]  writePointerValue: ";
// 
// 	OutputDebugStringA((prefix + t).c_str());
	bool isnullpointer;
	if (el == nullptr)
	{
		isnullpointer = true;
		write(isnullpointer);
	}
	else
	{
		isnullpointer = false;
		write(isnullpointer);
		write(*el);
	}
}


template <typename T>
inline void MemStream::read(T &el)
{
	size_t datasize = sizeof(T);
	memcpy((void*)&el,streamhandle , datasize);
	streamcount += datasize;
	streamhandle += datasize;
}

template <typename T>
inline void MemStream::read(T *el)
{
	size_t datasize = sizeof(T*);
	memcpy(&el, streamhandle , datasize);
	streamcount += datasize;
	streamhandle = streamhandle + datasize;
}

template <typename T>
inline void MemStream::readPointerValue(T *el)
{
	
	bool isnullpointer;
	read(isnullpointer);
	if (isnullpointer)
	{
		
	}
	else
	{
		read(*el);
	}
}


inline void MemStream::read(CommandEnum& enu)
{
	memcpy(&enu,streamhandle, sizeof(CommandEnum));
	streamcount += sizeof(CommandEnum);
	streamhandle = (unsigned char*)streamhandle + sizeof(CommandEnum);

}

