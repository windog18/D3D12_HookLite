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
				break;
			}
		}
		return hasvaule;
	}

	static void Reset() {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempVector.clear();
	}
private:
	static std::vector<CBResstr>  m_sTempVector;

	static std::mutex m_sMutex;
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
					ss << " (stop to write files)";
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
		m_RecordState = (m_RecordState + 1) % 3;

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

		default:
			ss << " (unknown state): ";
			ss << m_RecordState;
			break;
		}
		Log(ss.str().c_str());
	}

	void SetFrameTagForAll(CommandEnum Tag);
	void WriteAllBufferToResult();

	void ResetRecordState();
	MemStream *GetOrCreateMemStream(DWORD threadID);
private:
	static TempCluster *m_sSingleton;


	std::mutex m_sMutex;
	std::map<DWORD, MemStream *> m_sRecordMemStreamMap;

	int m_RecordState;
	std::mutex m_recordMutex;
};

#define RecordStart \
	if( TempCluster::GetInstance()->IsRecordingData() ) \
	 {\

/*
#define RecordStart \
	if( 0 ) \
	 {\*/

#define RecordEnd \
	}

#define RecordSpecialStart \
if( TempCluster::GetInstance()->IsRecordingPresent() ) \
	{


#define RecordSpecialEnd \
}


#define GetStreamFromThreadID() \
TempCluster::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); 