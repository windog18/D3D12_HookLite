#pragma once
#include "Common.h"

#include "memstream.h"


//////////////////////////////////////////////////////////device interface
DECLARE_FUNCTIONPTR(long, D3D12DeviceQueryInterface, ID3D12Device* dDevice, REFIID riid, void **ppvObject) //0
{ 
	auto res = oD3D12DeviceQueryInterface(dDevice, riid, ppvObject);

	LOG_ONCE(__FUNCTION__);

	return res;
}

DECLARE_FUNCTIONPTR(UINT, D3D12DeviceGetNodeCount, ID3D12Device *dDevice)  //7
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12DeviceGetNodeCount(dDevice);
}

DECLARE_FUNCTIONPTR(long, D3D12DeviceCreateCommandQueue, ID3D12Device *dDevice, const D3D12_COMMAND_QUEUE_DESC *pDesc, REFIID riid, void **ppCommandQueue) //8
{
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12DeviceCreateCommandQueue(dDevice, pDesc, riid, ppCommandQueue);
// 	RecordStart
// 	MemStream* streaminstance = GetStreamFromThreadID();
// 	streaminstance->write(Device_CreateCommandQueue);
// 	streaminstance->write(dDevice);
// 	streaminstance->writePointerValue(pDesc);
// 	streaminstance->write(riid);
// 	streaminstance->write(*ppCommandQueue);
// 	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandAllocator, ID3D12Device *dDevice, D3D12_COMMAND_LIST_TYPE type, REFIID riid, void **ppCommandAllocator) //9
{
	//FUNC_DEFINE
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE(__FUNCTION__);

	auto res = oD3D12CreateCommandAllocator(dDevice, type, riid, ppCommandAllocator);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateCommandAllocator);
	streaminstance->write(dDevice);
	streaminstance->write(type);
	streaminstance->write(riid);
	streaminstance->write(*ppCommandAllocator);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateGraphicsPipelineState, ID3D12Device *dDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc, REFIID riid, void **ppPipelineState) //10
{
	LOG_ONCE(__FUNCTION__);
	HRESULT result = oD3D12CreateGraphicsPipelineState(dDevice, pDesc, riid, ppPipelineState);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateGraphicsPipelineState);
	streaminstance->write(dDevice);
	streaminstance->write(*pDesc);
	streaminstance->write(riid);
	streaminstance->write(*ppPipelineState);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateComputePipelineState, ID3D12Device *dDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc, REFIID riid, void **ppPipelineState) //11
{
	LOG_ONCE(__FUNCTION__);

	auto res = oD3D12CreateComputePipelineState(dDevice, pDesc, riid, ppPipelineState);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateComputePipelineState);
	streaminstance->write(dDevice);
	streaminstance->write(*pDesc);
	streaminstance->write(riid);
	streaminstance->write(*ppPipelineState);
	RecordEnd
		return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandList, ID3D12Device *dDevice, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator *pCommandAllocator,
ID3D12PipelineState *pInitialState, REFIID riid, void **ppCommandList) //12
{
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE(__FUNCTION__);
	return oD3D12CreateCommandList(dDevice, nodeMask, type, pCommandAllocator, pInitialState, riid, ppCommandList);
}

DECLARE_FUNCTIONPTR(long, D3D12CheckFeatureSupport, ID3D12Device *dDevice, D3D12_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize) //13
{

	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE(__FUNCTION__);
	return oD3D12CheckFeatureSupport(dDevice, Feature, pFeatureSupportData, FeatureSupportDataSize);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateDescriptorHeap, ID3D12Device *dDevice, const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc, REFIID riid, void **ppvHeap) //14
{

	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE(__FUNCTION__);
	ID3D12DescriptorHeap* pheap;
	auto res = oD3D12CreateDescriptorHeap(dDevice, pDescriptorHeapDesc, riid, (void **)&pheap);
	*ppvHeap = pheap;

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateDescriptorHeap);
	streaminstance->write(dDevice);
	streaminstance->writePointerValue(pDescriptorHeapDesc);
	streaminstance->write(riid);
	streaminstance->write(pheap);
	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(UINT, D3D12GetDescriptorHandleIncrementSize, ID3D12Device *dDevice, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType) //15
{
	LOG_ONCE(__FUNCTION__);
	//Log("[d3d12] D3D12GetDescriptorHandleIncrementSize");
	return oD3D12GetDescriptorHandleIncrementSize(dDevice, DescriptorHeapType);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateRootSignature, ID3D12Device *dDevice, UINT nodeMask, const void *pBlobWithRootSignature, SIZE_T blobLengthInBytes, 
REFIID riid, void **ppvRootSignature)                        //16
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);


	HRESULT ert = oD3D12CreateRootSignature(dDevice, nodeMask, pBlobWithRootSignature, blobLengthInBytes, riid, ppvRootSignature);
	RecordStart
		MemStream* streaminstance = GetStreamFromThreadID();
		streaminstance->write(Device_CreateRootSignature);
		streaminstance->write(dDevice);
		streaminstance->write(nodeMask);
		streaminstance->write(blobLengthInBytes);
		streaminstance->write(pBlobWithRootSignature, blobLengthInBytes);
		streaminstance->write(riid);
		streaminstance->write(*ppvRootSignature);
	RecordEnd
	return ert;
}

DECLARE_FUNCTIONPTR(void, D3D12CreateConstantBufferView, ID3D12Device *dDevice, const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //17
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateConstantBufferView");
	oD3D12CreateConstantBufferView(dDevice, pDesc, DestDescriptor);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateConstantBufferView);
	streaminstance->write(dDevice);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);

	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CreateShaderResourceView, ID3D12Device *dDevice, ID3D12Resource *pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //18
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateShaderResourceView");
	oD3D12CreateShaderResourceView(dDevice, pResource, pDesc, DestDescriptor);


	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateShaderResourceView);
	streaminstance->write(dDevice);
	streaminstance->write(pResource);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);


	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12CreateUnorderedAccessView, ID3D12Device *dDevice, ID3D12Resource *pResource, ID3D12Resource *pCounterResource,
const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //19
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateUnorderedAccessView");
	oD3D12CreateUnorderedAccessView(dDevice, pResource, pCounterResource, pDesc, DestDescriptor);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateUnorderedAccessView);
	streaminstance->write(dDevice);
	streaminstance->write(pResource);
	streaminstance->write(pCounterResource);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);
	RecordEnd

}

DECLARE_FUNCTIONPTR(void, D3D12CreateRenderTargetView, ID3D12Device *dDevice, ID3D12Resource *pResource,
const D3D12_RENDER_TARGET_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //20
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	oD3D12CreateRenderTargetView(dDevice, pResource, pDesc, DestDescriptor);
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateRenderTargetView);
	streaminstance->write(dDevice);
	streaminstance->write(pResource);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);
	RecordEnd

}

DECLARE_FUNCTIONPTR(void, D3D12CreateDepthStencilView, ID3D12Device *dDevice, ID3D12Resource *pResource, 
const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //21
{
	LOG_ONCE(__FUNCTION__);
	//GlobalGathering::GetInstance()->GatherDevice(dDevice);
	oD3D12CreateDepthStencilView(dDevice, pResource, pDesc, DestDescriptor);
	
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateDepthStencilView);
	streaminstance->write(dDevice);
	streaminstance->write(pResource);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12CreateSampler, ID3D12Device *dDevice, const D3D12_SAMPLER_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //22
{
	LOG_ONCE(__FUNCTION__);
	oD3D12CreateSampler(dDevice, pDesc, DestDescriptor);

	RecordStart
	MemStream* instancestream = GetStreamFromThreadID();
	instancestream->write(Device_CreateSampler);
	instancestream->write(dDevice);
	instancestream->write(*pDesc);
	instancestream->write(DestDescriptor);
	RecordEnd
}


DECLARE_FUNCTIONPTR(void, D3D12CopyDescriptors, ID3D12Device *dDevice, UINT NumDestDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
const UINT *pDestDescriptorRangeSizes, UINT NumSrcDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
const UINT *pSrcDescriptorRangeSizes, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) //23
{

	LOG_ONCE(__FUNCTION__);



	oD3D12CopyDescriptors(dDevice, NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
		NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes, DescriptorHeapsType);


	RecordStart
	MemStream* instancestream = GetStreamFromThreadID();
	instancestream->write(Device_CopyDescriptors);
	instancestream->write(dDevice);
	instancestream->write(NumDestDescriptorRanges);
	instancestream->write(DescriptorHeapsType);

 	D3D12_CPU_DESCRIPTOR_HANDLE handle = *pDestDescriptorRangeStarts;
 	instancestream->write(handle);

 	handle = *pSrcDescriptorRangeStarts;
 	instancestream->write(handle);
 
	for (UINT i = 0; i < NumDestDescriptorRanges; i++){
		instancestream->write(pDestDescriptorRangeSizes[i]);
	}

	instancestream->write(NumSrcDescriptorRanges);


	for (UINT i = 0; i < NumSrcDescriptorRanges; i++){
		if (pSrcDescriptorRangeSizes == NULL) {
			UINT zeroVal = 0;
			instancestream->write(zeroVal);
		}
		else {
			instancestream->write(pSrcDescriptorRangeSizes[i]);
		}
		//instancestream->write(pSrcDescriptorRangeSizes[i]);
	}

 	RecordEnd
	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CopyDescriptorsSimple, ID3D12Device *dDevice, UINT NumDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) //24
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12CopyDescriptorsSimple(dDevice, NumDescriptors, DestDescriptorRangeStart, SrcDescriptorRangeStart, DescriptorHeapsType);
}

DECLARE_FUNCTIONPTR(D3D12_RESOURCE_ALLOCATION_INFO, D3D12GetResourceAllocationInfo, ID3D12Device *dDevice, UINT visibleMask,
UINT numResourceDescs, const D3D12_RESOURCE_DESC *pResourceDescs) //25
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetResourceAllocationInfo(dDevice, visibleMask, numResourceDescs, pResourceDescs);
}

DECLARE_FUNCTIONPTR(D3D12_HEAP_PROPERTIES, D3D12GetCustomHeapProperties, ID3D12Device *dDevice, UINT nodeMask, D3D12_HEAP_TYPE heapType) //26
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetCustomHeapProperties(dDevice, nodeMask, heapType);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommittedResource, ID3D12Device *dDevice,
const D3D12_HEAP_PROPERTIES *pHeapProperties,
D3D12_HEAP_FLAGS HeapFlags,
const D3D12_RESOURCE_DESC *pDesc,
D3D12_RESOURCE_STATES InitialResourceState,
const D3D12_CLEAR_VALUE *pOptimizedClearValue,
REFIID riidResource,
void **ppvResource) //27
{

	LOG_ONCE(__FUNCTION__);
	ID3D12Resource* pres;
	HRESULT result = oD3D12CreateCommittedResource(dDevice, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, (void**)&pres);
	*ppvResource = pres;

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateCommittedResource);
	streaminstance->write(dDevice);
	streaminstance->writePointerValue(pHeapProperties);
	streaminstance->write(HeapFlags);
	streaminstance->write(*pDesc);
	streaminstance->write(InitialResourceState);
	streaminstance->writePointerValue(pOptimizedClearValue);
	streaminstance->write(riidResource);
	streaminstance->write(pres);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateHeap, ID3D12Device *dDevice, const D3D12_HEAP_DESC *pDesc, REFIID riid, void **ppvHeap) //28
{
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12CreateHeap(dDevice, pDesc, riid, ppvHeap);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateHeap);
	streaminstance->write(dDevice);
	streaminstance->write(*pDesc);
	//streaminstance->write(riid);
	streaminstance->write(*ppvHeap);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreatePlacedResource, ID3D12Device *dDevice, ID3D12Heap *pHeap, UINT64 HeapOffset, const D3D12_RESOURCE_DESC *pDesc,
D3D12_RESOURCE_STATES InitialState, const D3D12_CLEAR_VALUE *pOptimizedClearValue, REFIID riid, void **ppvResource) //29
{
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12CreatePlacedResource(dDevice, pHeap, HeapOffset, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreatePlacedResource);
	streaminstance->write(dDevice);
	streaminstance->write(pHeap);
	streaminstance->write(HeapOffset);
	streaminstance->write(*pDesc);
	streaminstance->write(InitialState);
	streaminstance->writePointerValue(pOptimizedClearValue);
	streaminstance->write(riid);
	streaminstance->write(*ppvResource);
	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateReservedResource, ID3D12Device *dDevice, const D3D12_RESOURCE_DESC *pDesc, D3D12_RESOURCE_STATES InitialState,
const D3D12_CLEAR_VALUE *pOptimizedClearValue, REFIID riid, void **ppvResource) //30
{
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12CreateReservedResource(dDevice, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateReservedResource);
	streaminstance->write(dDevice);
	streaminstance->write(*pDesc);
	streaminstance->write(InitialState);
	streaminstance->writePointerValue(pOptimizedClearValue);
	streaminstance->write(riid);
	streaminstance->write(*ppvResource);
	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateSharedHandle, ID3D12Device *dDevice, ID3D12DeviceChild *pObject, const SECURITY_ATTRIBUTES *pAttributes,
DWORD Access, LPCWSTR Name, HANDLE *pHandle) //31
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12CreateSharedHandle(dDevice, pObject, pAttributes, Access, Name, pHandle);
}

DECLARE_FUNCTIONPTR(long, D3D12OpenSharedHandle, ID3D12Device *dDevice, HANDLE NTHandle, REFIID riid, void **ppvObj) //32
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12OpenSharedHandle(dDevice, NTHandle, riid, ppvObj);
}

DECLARE_FUNCTIONPTR(long, D3D12OpenSharedHandleByName, ID3D12Device *dDevice, LPCWSTR Name, DWORD Access, HANDLE *pNTHandle) //33
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12OpenSharedHandleByName(dDevice, Name, Access, pNTHandle);
}

DECLARE_FUNCTIONPTR(long, D3D12MakeResident, ID3D12Device *dDevice, UINT NumObjects, ID3D12Pageable *const *ppObjects) //34
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12MakeResident(dDevice, NumObjects, ppObjects);
}

DECLARE_FUNCTIONPTR(long, D3D12Evict, ID3D12Device *dDevice, UINT NumObjects, ID3D12Pageable *const *ppObjects) //35
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12Evict(dDevice, NumObjects, ppObjects);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateFence, ID3D12Device *dDevice, UINT64 InitialValue, D3D12_FENCE_FLAGS Flags, REFIID riid, void **ppFence) //36
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12CreateFence(dDevice, InitialValue, Flags, riid, ppFence);
}

DECLARE_FUNCTIONPTR(long, D3D12GetDeviceRemovedReason, ID3D12Device *dDevice) //37
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetDeviceRemovedReason(dDevice);
}

DECLARE_FUNCTIONPTR(void, D3D12GetCopyableFootprints, ID3D12Device *dDevice, const D3D12_RESOURCE_DESC *pResourceDesc, UINT FirstSubresource, 
UINT NumSubresources, UINT64 BaseOffset, D3D12_PLACED_SUBRESOURCE_FOOTPRINT *pLayouts, UINT *pNumRows, UINT64 *pRowSizeInBytes, UINT64 *pTotalBytes) //38
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetCopyableFootprints(dDevice, pResourceDesc, FirstSubresource, NumSubresources, BaseOffset, pLayouts, pNumRows, pRowSizeInBytes, pTotalBytes);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateQueryHeap, ID3D12Device *dDevice, const D3D12_QUERY_HEAP_DESC *pDesc, REFIID riid, void **ppvHeap) //39
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12CreateQueryHeap(dDevice, pDesc, riid, ppvHeap);
}

DECLARE_FUNCTIONPTR(long, D3D12SetStablePowerState, ID3D12Device *dDevice, BOOL Enable) //40
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12SetStablePowerState(dDevice, Enable);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandSignature, ID3D12Device *dDevice, const D3D12_COMMAND_SIGNATURE_DESC *pDesc, ID3D12RootSignature *pRootSignature,
REFIID riid, void **ppvCommandSignature) //41
{
	LOG_ONCE(__FUNCTION__);
	auto res = oD3D12CreateCommandSignature(dDevice, pDesc, pRootSignature, riid, ppvCommandSignature);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateCommandSignature);
	streaminstance->write(dDevice);
	streaminstance->write(*pDesc);
	streaminstance->write(pRootSignature);
	streaminstance->write(riid);
	streaminstance->write(*ppvCommandSignature);
	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(void, D3D12GetResourceTiling, ID3D12Device *dDevice, ID3D12Resource *pTiledResource, UINT *pNumTilesForEntireResource, D3D12_PACKED_MIP_INFO *pPackedMipDesc,
D3D12_TILE_SHAPE *pStandardTileShapeForNonPackedMips, UINT *pNumSubresourceTilings, UINT FirstSubresourceTilingToGet, D3D12_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips) //42
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetResourceTiling(dDevice, pTiledResource, pNumTilesForEntireResource, pPackedMipDesc, pStandardTileShapeForNonPackedMips,
		pNumSubresourceTilings, FirstSubresourceTilingToGet, pSubresourceTilingsForNonPackedMips);
}

DECLARE_FUNCTIONPTR(LUID, D3D12GetAdapterLuid, ID3D12Device *dDevice) //43
{
	LOG_ONCE(__FUNCTION__);
	return oD3D12GetAdapterLuid(dDevice);
}

void CreateHookD3D12DeviceInterface(uint64_t* methodVirtualTable)
{
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[0], D3D12DeviceQueryInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[7], D3D12DeviceGetNodeCount);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[8], D3D12DeviceCreateCommandQueue);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[9], D3D12CreateCommandAllocator);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[10], D3D12CreateGraphicsPipelineState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[11], D3D12CreateComputePipelineState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[12], D3D12CreateCommandList);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[13], D3D12CheckFeatureSupport);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[14], D3D12CreateDescriptorHeap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[15], D3D12GetDescriptorHandleIncrementSize);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[16], D3D12CreateRootSignature);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[17], D3D12CreateConstantBufferView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[18], D3D12CreateShaderResourceView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[19], D3D12CreateUnorderedAccessView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[20], D3D12CreateRenderTargetView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[21], D3D12CreateDepthStencilView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[22], D3D12CreateSampler);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[23], D3D12CopyDescriptors);

	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[24], D3D12CopyDescriptorsSimple);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[25], D3D12GetResourceAllocationInfo);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[26], D3D12GetCustomHeapProperties);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[27], D3D12CreateCommittedResource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[28], D3D12CreateHeap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[29], D3D12CreatePlacedResource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[30], D3D12CreateReservedResource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[31], D3D12CreateSharedHandle);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[32], D3D12OpenSharedHandle);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[33], D3D12OpenSharedHandleByName);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[34], D3D12MakeResident);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[35], D3D12Evict);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[36], D3D12CreateFence);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[37], D3D12GetDeviceRemovedReason);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[38], D3D12GetCopyableFootprints);

	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[39], D3D12CreateQueryHeap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[40], D3D12SetStablePowerState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[41], D3D12CreateCommandSignature);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[42], D3D12GetResourceTiling);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[43], D3D12GetAdapterLuid);



	MH_EnableHook((LPVOID)methodVirtualTable[0]);
	MH_EnableHook((LPVOID)methodVirtualTable[7]);
	MH_EnableHook((LPVOID)methodVirtualTable[8]);
	MH_EnableHook((LPVOID)methodVirtualTable[9]);
	MH_EnableHook((LPVOID)methodVirtualTable[10]);
	MH_EnableHook((LPVOID)methodVirtualTable[11]);
	MH_EnableHook((LPVOID)methodVirtualTable[12]);
	MH_EnableHook((LPVOID)methodVirtualTable[13]);
	MH_EnableHook((LPVOID)methodVirtualTable[14]);
	MH_EnableHook((LPVOID)methodVirtualTable[15]);
	MH_EnableHook((LPVOID)methodVirtualTable[16]);
	MH_EnableHook((LPVOID)methodVirtualTable[17]);
	MH_EnableHook((LPVOID)methodVirtualTable[18]);
	MH_EnableHook((LPVOID)methodVirtualTable[19]);
	MH_EnableHook((LPVOID)methodVirtualTable[20]);
	MH_EnableHook((LPVOID)methodVirtualTable[21]);
	MH_EnableHook((LPVOID)methodVirtualTable[22]);
	MH_EnableHook((LPVOID)methodVirtualTable[23]);
	MH_EnableHook((LPVOID)methodVirtualTable[27]);
	MH_EnableHook((LPVOID)methodVirtualTable[28]);
	MH_EnableHook((LPVOID)methodVirtualTable[29]);
	MH_EnableHook((LPVOID)methodVirtualTable[30]);
	MH_EnableHook((LPVOID)methodVirtualTable[31]);
	MH_EnableHook((LPVOID)methodVirtualTable[32]);
	MH_EnableHook((LPVOID)methodVirtualTable[33]);
	MH_EnableHook((LPVOID)methodVirtualTable[34]);
	MH_EnableHook((LPVOID)methodVirtualTable[35]);
	MH_EnableHook((LPVOID)methodVirtualTable[36]);
	MH_EnableHook((LPVOID)methodVirtualTable[37]);
	MH_EnableHook((LPVOID)methodVirtualTable[38]);
	MH_EnableHook((LPVOID)methodVirtualTable[39]);
	MH_EnableHook((LPVOID)methodVirtualTable[40]);
	MH_EnableHook((LPVOID)methodVirtualTable[41]);

	MH_EnableHook((LPVOID)methodVirtualTable[42]);
	MH_EnableHook((LPVOID)methodVirtualTable[43]);
}