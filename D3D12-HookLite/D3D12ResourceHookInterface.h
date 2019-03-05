#pragma once
#include "Common.h"
#include "memstream.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////D3DRESOUCRES hook interface//

/*

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceQueryInterface, ID3D12Resource * dResource, REFIID riid, void **ppvObject) //0
{
	//Log("[D3D12]D3D12Resouces QueryInterface ");
	return oD3D12ResourceQueryInterface(dResource, riid, ppvObject);
}



DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetPrivateData, ID3D12Resource *dResource, REFGUID guid, UINT DataSize, const void *pData) //4
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12ResourceSetPrivateData(dResource, guid, DataSize, pData);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetPrivateDataInterface, ID3D12Resource *dResource, REFGUID guid, const IUnknown *pData) //5
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12ResourceSetPrivateDataInterface(dResource, guid, pData);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetName, ID3D12Resource *dResource, LPCWSTR Name) //6
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12ResourceSetName(dResource, Name);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceGetDevice, ID3D12Resource *dResource, REFIID riid, void **ppvDevice) //7 
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12ResourceGetDevice(dResource, riid, ppvDevice);
}*/

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceMap, ID3D12Resource *dResource, UINT subresource, const D3D12_RANGE *pReadRange, void **ppData) //8
{

	LOG_ONCE(__FUNCTION__);

	HRESULT result = oD3D12ResourceMap(dResource, subresource, pReadRange, ppData);
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Resource_Map);
	streaminstance->write(dResource);
 	streaminstance->write(subresource);
 	streaminstance->writePointerValue(pReadRange);
	streaminstance->write(*ppData);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(void, D3D12ResourceUnmap, ID3D12Resource *dResource, UINT subresource, const D3D12_RANGE *pWrittenRange) //9
{
	LOG_ONCE(__FUNCTION__);

	oD3D12ResourceUnmap(dResource, subresource, pWrittenRange);
	

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();

	D3D12_RESOURCE_DESC desc = dResource->GetDesc();

	streaminstance->write(Resource_Unmap);
	streaminstance->write(dResource);
	streaminstance->write(subresource);
	streaminstance->writePointerValue(pWrittenRange);


	//write the real data
	streaminstance->write(desc.Dimension);
	if (desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		size_t buffersize = desc.Width;	
		void *tempPtr = NULL;
		
			
		oD3D12ResourceMap(dResource, subresource, pWrittenRange,&tempPtr);
		streaminstance->write(buffersize);
		streaminstance->write(tempPtr, buffersize);
		oD3D12ResourceUnmap(dResource, subresource, pWrittenRange);
			
		
		
	}
	else if (desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		size_t texsize = desc.Width;
		//size_t texsize = desc.Width*desc.Height * 4;
		//streaminstance->write(pTemp, texsize);
	}
	RecordEnd
}

/*
DECLARE_FUNCTIONPTR(D3D12_RESOURCE_DESC, D3D12ResourceGetDesc, ID3D12Resource *dResource) //10
{
	LOG_ONCE(__FUNCTION__);
	Log_Detail_2(Enum_Device, __FUNCTION__);
	//Log("[D3D12]D3D12Resources GetDesc");
	return oD3D12ResourceGetDesc(dResource);
}*/


DECLARE_FUNCTIONPTR(D3D12_GPU_VIRTUAL_ADDRESS,D3D12GetGPUVirtualAddress, ID3D12Resource * dResource) //11
{
	LOG_ONCE(__FUNCTION__);
	
	auto res = oD3D12GetGPUVirtualAddress(dResource);

#if DECLARE_STATIC
	ResourceTempData<UINT64, ID3D12Resource *>::SetTempMapData(res, dResource);
#endif
	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12WriteToSubresource, ID3D12Resource *dResource, UINT DstSubresource, const D3D12_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)  //12
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12WriteToSubresource(dResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

DECLARE_FUNCTIONPTR(long, D3D12ReadFromSubresource, ID3D12Resource *dResource, void *pDstData, UINT DstRowPitch, UINT DstDepthPitch, UINT SrcSubresource, const D3D12_BOX *pSrcBox) //13
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12ReadFromSubresource(dResource, pDstData, DstRowPitch, DstDepthPitch, SrcSubresource, pSrcBox);
}

DECLARE_FUNCTIONPTR(long, D3D12GetHeapProperties, ID3D12Resource *dResource, D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS *pHeapFlags) //14
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetHeapProperties(dResource, pHeapProperties, pHeapFlags);
}

DECLARE_FUNCTIONPTR(D3D12_DESCRIPTOR_HEAP_DESC, D3D12DescriptorHeapGesDesc, ID3D12DescriptorHeap *desHeap) //15
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12DescriptorHeapGesDesc(desHeap);
}

DECLARE_FUNCTIONPTR(D3D12_CPU_DESCRIPTOR_HANDLE, D3D12GetCPUDescriptorHandleForHeapStart, ID3D12DescriptorHeap *desHeap) //16
{ 
	LOG_ONCE(__FUNCTION__);

	auto res = oD3D12GetCPUDescriptorHandleForHeapStart(desHeap);


	ResourceTempData<UINT64, ID3D12DescriptorHeap *, 0>::SetTempMapData(res.ptr, desHeap);
	return res;
}

DECLARE_FUNCTIONPTR(D3D12_GPU_DESCRIPTOR_HANDLE, D3D12GetGPUDescriptorHandleForHeapStart, ID3D12DescriptorHeap *desHeap) //17
{
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12GetGPUDescriptorHandleForHeapStart(desHeap);

	ResourceTempData<UINT64, ID3D12DescriptorHeap *, 1>::SetTempMapData(res.ptr, desHeap);
	return res;
}

void CreateHookD3D12ResourceInterface(uint64_t* methodVirtualTable)
{
	/*CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 0], D3D12ResourceQueryInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 4], D3D12ResourceSetPrivateData);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 5], D3D12ResourceSetPrivateDataInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 6], D3D12ResourceSetName);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 7], D3D12ResourceGetDevice);*/
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 8], D3D12ResourceMap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 9], D3D12ResourceUnmap);
	//CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 10], D3D12ResourceGetDesc); ??? will crash 
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 11], D3D12GetGPUVirtualAddress);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 12], D3D12WriteToSubresource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 13], D3D12ReadFromSubresource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 14], D3D12GetHeapProperties);


	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 16], D3D12GetCPUDescriptorHandleForHeapStart);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 17], D3D12GetGPUDescriptorHandleForHeapStart);

	/*MH_EnableHook((LPVOID)methodVirtualTable[150 +  0]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  4]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  5]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  6]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  7]);*/
 	MH_EnableHook((LPVOID)methodVirtualTable[150 +  8]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  9]);
	//MH_EnableHook((LPVOID)methodVirtualTable[150 + 10]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 11]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 12]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 13]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 14]);

	MH_EnableHook((LPVOID)methodVirtualTable[150 + 16]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 17]);
}