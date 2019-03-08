#pragma once
#include "Common.h"
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

	MemStream *GetOrCreateTempStream(DWORD threadID);
private:
	static TempCluster *m_sSingleton;


	std::mutex m_sMutex;
	std::map<DWORD, MemStream *> m_sRecordMemStreamMap;

	std::map<DWORD, MemStream *> m_sTempWriteMemStreamMap;

	int m_RecordState;
	std::mutex m_recordMutex;
};

#define RecordStart \
	if( TempCluster::GetInstance()->IsRecordingData() ) \
	 {\

#define RecordEnd \
	}

#define RecordSpecialStart \
if( TempCluster::GetInstance()->IsRecordingPresent() ) \
	{


#define RecordSpecialEnd \
}


#define GetStreamFromThreadID() \
TempCluster::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); 