#pragma once
#include "memstream.h"
#include <map>

class MemStream;
template<typename Key, typename Value, int N = 0>
class ResourceTempData {
public:

	static void SetTempMapData(Key threadID, Value ptr) {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempMap[threadID] = ptr;
	}


	static Value GetTempMapData(Key threadID) {
		Value ptr = NULL;
		{
			std::lock_guard<std::mutex> lock(m_sMutex);
			if (m_sTempMap.find(threadID) != m_sTempMap.end())
				ptr = m_sTempMap[threadID];
		}
		return ptr;
	}

	static void Reset() {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempMap.clear();
	}
private:
	static std::map<Key, Value>  m_sTempMap;

	static std::mutex m_sMutex;
};

#define IMPLEMENT_RESOURCE_DATA(Key, Value, N0) \
std::map<Key, Value> ResourceTempData<Key, Value,N0>::m_sTempMap; \
std::mutex ResourceTempData<Key, Value,N0>::m_sMutex;

void ResetRecordState();
class TempCluster
{
public:
	static TempCluster *GetInstance();
	TempCluster();
	~TempCluster();

	inline bool IsRecording() {
		bool recordState = false;
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			recordState = m_isRecording;
		}
		return recordState;
	}

	inline void SetRecording(bool recordState) {
	{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			if (m_isRecording != recordState) {
				std::stringstream ss;
				ss << "recording state changed: ";
				ss << recordState;
				OutputDebugStringA(ss.str().c_str());
				//OutputDebugStringA("start recording");
			}
			m_isRecording = recordState;

		}
	}
	void SetFrameTagForAll(CommandEnum Tag);
	void WriteAllBufferToResult();

	void ResetRecordState();
	MemStream *GetOrCreateMemStream(DWORD threadID);
private:
	static TempCluster *m_sSingleton;


	std::mutex m_sMutex;
	std::map<DWORD, MemStream *> m_sRecordMemStreamMap;

	bool m_isRecording;
	std::mutex m_recordMutex;
};

#define RecordStart \
	if( TempCluster::GetInstance()->IsRecording() ) \
	 {\

#define RecordEnd \
	}

#define GetStreamFromThreadID() \
TempCluster::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); 