#include "TempCluster.h"
#include "UWP.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;




IMPLEMENT_MAP(DescriptorRemap,size_t,size_t)
IMPLEMENT_MAP( SrvMap, size_t, SRVStr)
IMPLEMENT_MAP(CBBufMap, size_t, ConstantBufferData)
IMPLEMENT_MAP(SamplerMap, size_t, D3D12_SAMPLER_DESC*)

IMPLEMENT_MAP(RootSignMap, ID3D12RootSignature*, const D3D12_ROOT_SIGNATURE_DESC*)

std::vector<CBResstr>  ResourceVectorData::m_sTempVector;
std::mutex ResourceVectorData::m_sMutex;
std::set<ID3D12Resource*> ResourceVectorData::cstreset;

std::map<ID3D12Resource*, ResourceVectorData::bufstr> ResourceVectorData::unmapres;
std::mutex ResourceVectorData::m_sMutex2;


UINT* ResourceVectorData::desmap5 = NULL;
UINT* ResourceVectorData::desmap6 = NULL;


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

void TempCluster::SetEndFileForAll()
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	{
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++) {
			it->second->prepareendfile = true;
		}
	}
}

void TempCluster::WriteAllBufferToResult()
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	size_t count = m_sRecordMemStreamMap.size();
	
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
		
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sRecordMemStreamMap.insert(std::make_pair(threadID, memStream));
		

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
	

	DescriptorRemap::Reset();
	


	TempCluster::GetInstance()->ResetRecordState();
}


UINT ResourceVectorData::WriteStream(MemStream* pstream)
{
	UINT count = 0;
	{
		std::lock_guard<std::mutex> lock(m_sMutex2);
		std::map<ID3D12Resource*, bufstr>::iterator iter;
		std::vector<bufstr> bufarray;
		std::vector<ID3D12Resource*> resarray;
		for ( iter = unmapres.begin(); iter != unmapres.end(); iter++ )
		{
			if ( cstreset.find(iter->first) != cstreset.end() )
			{
				bufarray.push_back(iter->second);
				resarray.push_back(iter->first);
				count++;
			}
		}
		pstream->write(copy_cstbuf);
		pstream->write(count);

		for (size_t i = 0; i < bufarray.size(); i++)
		{
			bufstr& bstr = bufarray[i];
			pstream->write(resarray[i]);
			pstream->write(bstr.bufsize);
			pstream->write(bstr.pdata, bstr.bufsize);
		}
	}
	return count;
}

void ResourceVectorData::WriteDesMap(MemStream* pstream)
{
	pstream->write(copy_cstmap);

	pstream->write(desmap5, sizeof(UINT) * 500000);
	pstream->write(desmap6, sizeof(UINT) * 500000);

}