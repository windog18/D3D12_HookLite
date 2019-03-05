#include "TempCluster.h"
#include "UWP.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

std::map<std::pair<ID3D12Resource *, UINT>, void *> ResourceTempData<std::pair<ID3D12Resource *, UINT>, void *>::m_sTempMap;
std::mutex ResourceTempData<std::pair<ID3D12Resource *, UINT>, void *>::m_sMutex;

//IMPLEMENT_RESOURCE_DATA(std::pair<ID3D12Resource *, UINT>, void *,0)  :????????FIXED ME!
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12DescriptorHeap*, 0)
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12DescriptorHeap*, 1)
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12Resource*, 0)


TempCluster * TempCluster::m_sSingleton = nullptr;

inline std::string narrow(std::wstring const& text)
{
	std::locale const loc("");
	wchar_t const* from = text.c_str();
	std::size_t const len = text.size();
	std::vector<char> buffer(len + 1);
	std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
	return std::string(&buffer[0], &buffer[len]);
}


TempCluster * TempCluster::GetInstance()
{
	if (m_sSingleton == nullptr) {
		m_sSingleton = new TempCluster();
	}
	return m_sSingleton;
}

TempCluster::TempCluster()
{
	m_RecordState = 0;
}


TempCluster::~TempCluster()
{

}

void TempCluster::SetFrameTagForAll(CommandEnum Tag)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	{
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++) {
			it->second->write(Tag);
		}
	}
}

void TempCluster::WriteAllBufferToResult()
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	size_t count = m_sRecordMemStreamMap.size();
	//Log_Detail_0(Enum_other1, "total dump files count: %d", count);
	fs::path basePath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP";
	for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++)
	{
		wstringstream wss;

		wss << "RecordData_";
		wss << it->first;

		if (GetCurrentThreadId() == it->first)
		{
			wss << "_mainThread";
		}
		std::error_code ErrorCode;

		fs::path writePath = basePath / wss.str() / L"recordData.bin";
		fs::path writeNamePath = basePath / wss.str() / L"recordData_name.txt";

		//OutputDebugStringW(writePath.c_str());
		if (fs::create_directories(writePath.parent_path(), ErrorCode) == false && ErrorCode) {
			OutputDebugStringA((std::string("could not create path at: ") + narrow(writePath.c_str())).c_str());
		}
		else {
			std::string charStrs = narrow(writePath.c_str());
			it->second->writetoFile(charStrs.c_str());

			std::string nameCharStrs = narrow(writeNamePath.c_str());
			it->second->writeCommandNameToFile(nameCharStrs.c_str());
		}
	}
}

MemStream * TempCluster::GetOrCreateMemStream(DWORD threadID)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (m_sRecordMemStreamMap.find(threadID) == m_sRecordMemStreamMap.end()) {
		//og_Detail_0(Enum_other1, "alloc new memStream for newThread %d", threadID);
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sRecordMemStreamMap.insert(std::make_pair(threadID, memStream));
		//Log_Detail_0(Enum_other1, "total record memStreamSize: %d", m_sRecordMemStreamMap[m_curRecordMemMapIdx].size());

	}
	return m_sRecordMemStreamMap[threadID];
}


void TempCluster::ResetRecordState()
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (m_sRecordMemStreamMap.size() > 0)
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++) {
			delete it->second;
		}
	m_sRecordMemStreamMap.clear();
}



inline void ResetRecordState()
{
	///ugly way to do so,try to optimize me!!
	ResourceTempData<std::pair<ID3D12Resource *, UINT>, void *>::Reset();
	ResourceTempData<UINT64, ID3D12DescriptorHeap*, 0>::Reset();
	ResourceTempData<UINT64, ID3D12DescriptorHeap*, 1>::Reset();
	ResourceTempData<UINT64, ID3D12Resource*>::Reset();


	TempCluster::GetInstance()->ResetRecordState();
}
