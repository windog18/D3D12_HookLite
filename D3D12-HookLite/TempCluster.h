#pragma once
#include "memstream.h"
#include <map>
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