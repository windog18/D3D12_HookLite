#pragma once
#include "Common.h"
#include "memstream.h"
#include <map>

class MemStream;

#define DECLARE_MAP(ClassName,Key,Value) \
class ClassName {\
public:\
	\
	static void SetTempMapData(Key threadID, Value ptr) {\
		std::lock_guard<std::mutex> lock(m_sMutex);\
		m_sTempMap[threadID] = ptr;\
	}\
	\
	static bool GetTempMapData(Key threadID, Value &ptr) {\
\
		bool hasvalue = false;\
		{\
			std::lock_guard<std::mutex> lock(m_sMutex);\
			if (m_sTempMap.find(threadID) != m_sTempMap.end())\
			{\
				ptr = m_sTempMap[threadID];\
				hasvalue = true;\
			}\
		}\
		return hasvalue;\
	}\
	\
	static void Reset() {\
		std::lock_guard<std::mutex> lock(m_sMutex);\
		m_sTempMap.clear();\
	}\
private:\
	static std::map<Key, Value>  m_sTempMap;\
	static std::mutex m_sMutex; \
};

#define IMPLEMENT_MAP(ClassName,Key,Value ) \
std::map<Key,Value> ClassName::m_sTempMap;\
std::mutex ClassName::m_sMutex;

DECLARE_MAP(DescriptorRemap,size_t,size_t)

struct SRVStr
{
	D3D12_SHADER_RESOURCE_VIEW_DESC* desc;	
	ID3D12Resource *pResource;
};
DECLARE_MAP(SrvMap, size_t, SRVStr)

DECLARE_MAP(SamplerMap, size_t, D3D12_SAMPLER_DESC*)

DECLARE_MAP(RootSignMap, ID3D12RootSignature*, const D3D12_ROOT_SIGNATURE_DESC*)

struct CBResstr
{
	D3D12_GPU_VIRTUAL_ADDRESS gpu;
	UINT64 bufsize;
	ID3D12Resource* pres;
};

struct ConstantBufferData
{
	ID3D12Resource* pres;
	UINT64 offset;
	UINT bufsize;
};
DECLARE_MAP(CBBufMap, size_t, ConstantBufferData)



class ResourceVectorData {
public:

	static void Add(CBResstr ptr) {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempVector.push_back(ptr);
		
	}

	
	static bool FindCst(UINT64 gpuadr,UINT bufsize, CBResstr& ptr) {
		std::lock_guard<std::mutex> lock(m_sMutex);
		bool hasvaule = false;
		for (size_t i = 0;i < m_sTempVector.size();i++)
		{
			CBResstr& str = m_sTempVector[i];
			if (gpuadr >= str.gpu && gpuadr <= str.gpu + str.bufsize)
			{
				ptr = str;
				hasvaule = true;
				cstreset.insert(str.pres);
				break;
			}
		}
		return hasvaule;
	}

	static void Reset() {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempVector.clear();
	}

	struct bufstr
	{
		UINT64 bufsize;
		void* pdata;
	};

	static void addmapres(UINT64 gpuadr, void* pdata) {

		std::lock_guard<std::mutex> lock(m_sMutex2);
		unmapres.insert(std::make_pair(gpuadr,pdata));

	}

	static void addunmapres(UINT64 gpuadr) {

		std::lock_guard<std::mutex> lock(m_sMutex2);
		unmapres.erase(gpuadr);

	}

	static void savemapres(UINT64 gpuadr, MemStream* pstream)
	{
		
		void* pdata = NULL;
		{
			std::lock_guard<std::mutex> lock(m_sMutex2);
			pdata = unmapres[gpuadr];
		}

		{
			std::lock_guard<std::mutex> lock(m_sMutex3);
			if (datamap.find(pdata) == datamap.end())
			{
				bool hasdata = true;
				pstream->write(hasdata);
				INT64 dataadr = (INT64)pdata;
				pstream->write(dataadr);
				pstream->write(pdata, 0x10000);
				datamap.insert(pdata);
			}
			else
			{
				bool hasdata = false;
				pstream->write(hasdata);
			}
		}
	}

	static void clearmapres()
	{
		std::lock_guard<std::mutex> lock(m_sMutex3);
		datamap.clear();
	}

	static size_t getCBResCount() {
		std::lock_guard<std::mutex> lock(m_sMutex);
		return cstreset.size();
	}

	static size_t getunmapresCount() {
		std::lock_guard<std::mutex> lock(m_sMutex2);
		return unmapres.size();
	}

	static void WriteDesMap(MemStream* pstream);

	static UINT* getHandleMap5()
	{
		if (desmap5 == NULL)
		{
			desmap5 = new UINT[500000];
			memset(desmap5, 0, sizeof(UINT) * 500000);
		}
		return desmap5;
	}

	static UINT* getHandleMap6()
	{
		if (desmap6 == NULL)
		{
			desmap6 = new UINT[500000];
			memset(desmap6, 0, sizeof(UINT) * 500000);
		}
		return desmap6;
	}
	
private:
	static std::vector<CBResstr>  m_sTempVector;
	static std::set<ID3D12Resource*> cstreset;
	static std::mutex m_sMutex;

	static std::set<void*> datamap;
	static std::mutex m_sMutex3;


	static std::map<UINT64, void*> unmapres;
	static std::mutex m_sMutex2;
public:
	static UINT* desmap5;
	static UINT* desmap6;
	
};

static UINT64 desgpuadr5 = 0;
static size_t descpuadr5 = 0;

static UINT64 desgpuadr6 = 0;
static size_t descpuadr6 = 0;

static size_t desrthandleptr = 0;
static size_t lidesrthandleptr = 0;

static size_t dsvdesrthandleptr = 0;


struct RTData
{
	ID3D12Resource* pres;
	D3D12_RENDER_TARGET_VIEW_DESC desc;
	LARGE_INTEGER counter;
};

struct DSVData
{
	ID3D12Resource* pres;
	D3D12_DEPTH_STENCIL_VIEW_DESC desc;
};

class RTDesData 
{
private:
	static RTData rtarray[5000];
	static DSVData dsvarray[120];
	static std::mutex m_sMutex;
public:
	static void setRTdata(size_t offset,RTData& rtdata);
	
	static RTData getRTdata(size_t offset);

	static void setDSVdata(size_t offset, DSVData& rtdata);

	static DSVData getDSVdata(size_t offset);

};



void ResetRecordState();
class TempCluster
{
public:
	static TempCluster *GetInstance();
	TempCluster();
	~TempCluster();

	inline bool IsRecordingData() {
		bool recordState = false;
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			recordState = (m_RecordState <= 1);
		}
		return recordState;
	}

	inline bool IsRecordingPresent() {
		bool recordState = false;
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			recordState = (m_RecordState == 1);
		}
		return recordState;
	}

	inline bool IsRecordingEnd() {
		bool recordState = false;
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			recordState = (m_RecordState == 2);
		}
		return recordState;
	}

	inline void SetRecordingState(int recordState) {
	{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			if (m_RecordState != recordState) {
				std::stringstream ss;
				ss << "recording state changed: ";
				switch (recordState)
				{
				case 0:
					ss << " (record all data)";
					break;
				case 1:
					ss << " (record present data)";
					break;
				case 2:
					ss << " (stop write files)";
					break;
				default:
					ss << " (unknown state): ";
					ss << recordState;
					break;
				}

				Log(ss.str().c_str());
			}
			m_RecordState = recordState;

		}
	}

	inline void ToggleRecordingState() {
		std::lock_guard<std::mutex> lock(m_recordMutex);
		m_RecordState = (m_RecordState + 1) % 4;

		std::stringstream ss;
		ss << "recording state changed: ";
		switch (m_RecordState)
		{
		case 0:
			ss << " (record all data)";
			break;
		case 1:
			ss << " (record present data)";
			break;
		case 2:
			ss << " (stop to write files)";
			break;
		case 3:
			ss << " (stop write files)";
			break;
		default:
			ss << " (unknown state): ";
			ss << m_RecordState;
			break;
		}
		Log(ss.str().c_str());
	}

	void SetFrameTagForAll(CommandEnum Tag);

	void SetEndFileForAll();

	void WriteAllBufferToResult();

	void ResetRecordState();
	MemStream *GetOrCreateMemStream(DWORD threadID);

	MemStream *GetInitMemStream(DWORD threadID);
private:
	static TempCluster *m_sSingleton;


	std::mutex m_sMutex;
	std::map<DWORD, MemStream *> m_sRecordMemStreamMap;

	std::mutex m_sMutex2;
	std::map<DWORD, MemStream *> m_sInitMemStreamMap;

	int m_RecordState;
	std::mutex m_recordMutex;
};
/*

#define RecordStart \
	if( TempCluster::GetInstance()->IsRecordingData() ) \
	 {\
*/


#define RecordStart \
	if( 0 ) \
	 {\

#define RecordEnd \
	}

/*
#define RecordSpecialStart \
if( TempCluster::GetInstance()->IsRecordingPresent() ) \
	{
*/

#define RecordSpecialStart \
if( 0 ) \
	{


#define RecordSpecialEnd \
}


#define GetStreamFromThreadID() \
TempCluster::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); 


#define GetInitStreamFromThreadID() \
TempCluster::GetInstance()->GetInitMemStream(GetCurrentThreadId()); 