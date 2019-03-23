#pragma once

/*
typedef struct ID3D12CommandQueueVtbl
{
	BEGIN_INTERFACE

		HRESULT(STDMETHODCALLTYPE *QueryInterface)( 0
			ID3D12CommandQueue * This,
			REFIID riid,
			_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)( 1
		ID3D12CommandQueue * This);

	ULONG(STDMETHODCALLTYPE *Release)( 2
		ID3D12CommandQueue * This);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)( 3
		ID3D12CommandQueue * This,
		_In_  REFGUID guid,
		_Inout_  UINT *pDataSize,
		_Out_writes_bytes_opt_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)( 4 
		ID3D12CommandQueue * This,
		_In_  REFGUID guid,
		_In_  UINT DataSize,
		_In_reads_bytes_opt_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)( 5 
		ID3D12CommandQueue * This,
		_In_  REFGUID guid,
		_In_opt_  const IUnknown *pData);

	HRESULT(STDMETHODCALLTYPE *SetName)( 6
		ID3D12CommandQueue * This,
		_In_z_  LPCWSTR Name);

	HRESULT(STDMETHODCALLTYPE *GetDevice)( 7 
		ID3D12CommandQueue * This,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvDevice);

	void (STDMETHODCALLTYPE *UpdateTileMappings)( 8
		ID3D12CommandQueue * This,
		_In_  ID3D12Resource *pResource,
		UINT NumResourceRegions,
		_In_reads_opt_(NumResourceRegions)  const D3D12_TILED_RESOURCE_COORDINATE *pResourceRegionStartCoordinates,
		_In_reads_opt_(NumResourceRegions)  const D3D12_TILE_REGION_SIZE *pResourceRegionSizes,
		_In_opt_  ID3D12Heap *pHeap,
		UINT NumRanges,
		_In_reads_opt_(NumRanges)  const D3D12_TILE_RANGE_FLAGS *pRangeFlags,
		_In_reads_opt_(NumRanges)  const UINT *pHeapRangeStartOffsets,
		_In_reads_opt_(NumRanges)  const UINT *pRangeTileCounts,
		D3D12_TILE_MAPPING_FLAGS Flags);

	void (STDMETHODCALLTYPE *CopyTileMappings)( 9 
		ID3D12CommandQueue * This,
		_In_  ID3D12Resource *pDstResource,
		_In_  const D3D12_TILED_RESOURCE_COORDINATE *pDstRegionStartCoordinate,
		_In_  ID3D12Resource *pSrcResource,
		_In_  const D3D12_TILED_RESOURCE_COORDINATE *pSrcRegionStartCoordinate,
		_In_  const D3D12_TILE_REGION_SIZE *pRegionSize,
		D3D12_TILE_MAPPING_FLAGS Flags);

	void (STDMETHODCALLTYPE *ExecuteCommandLists)( 10
		ID3D12CommandQueue * This,
		_In_  UINT NumCommandLists,
		_In_reads_(NumCommandLists)  ID3D12CommandList *const *ppCommandLists);

	void (STDMETHODCALLTYPE *SetMarker)( 11
		ID3D12CommandQueue * This,
		UINT Metadata,
		_In_reads_bytes_opt_(Size)  const void *pData,
		UINT Size);

	void (STDMETHODCALLTYPE *BeginEvent)(
		ID3D12CommandQueue * This,
		UINT Metadata,
		_In_reads_bytes_opt_(Size)  const void *pData,
		UINT Size);

	void (STDMETHODCALLTYPE *EndEvent)(
		ID3D12CommandQueue * This);

	HRESULT(STDMETHODCALLTYPE *Signal)(
		ID3D12CommandQueue * This,
		ID3D12Fence *pFence,
		UINT64 Value);

	HRESULT(STDMETHODCALLTYPE *Wait)(
		ID3D12CommandQueue * This,
		ID3D12Fence *pFence,
		UINT64 Value);

	HRESULT(STDMETHODCALLTYPE *GetTimestampFrequency)(
		ID3D12CommandQueue * This,
		_Out_  UINT64 *pFrequency);

	HRESULT(STDMETHODCALLTYPE *GetClockCalibration)(
		ID3D12CommandQueue * This,
		_Out_  UINT64 *pGpuTimestamp,
		_Out_  UINT64 *pCpuTimestamp);

	D3D12_COMMAND_QUEUE_DESC(STDMETHODCALLTYPE *GetDesc)(
		ID3D12CommandQueue * This);

	END_INTERFACE
} ID3D12CommandQueueVtbl;*/

DECLARE_FUNCTIONPTR(void, D3D12CommandQueueExecuteCommandLists, ID3D12CommandQueue * dCommandQueue,UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists) //10
{
	LOG_ONCE(__FUNCTION__);
	oD3D12CommandQueueExecuteCommandLists(dCommandQueue, NumCommandLists, ppCommandLists);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandQueue_ExecuteCommandLists);
	streamInstance->write(dCommandQueue);
	streamInstance->write(NumCommandLists);
	streamInstance->write(ppCommandLists, sizeof(ID3D12CommandList *)*NumCommandLists);
	RecordEnd
	
}

void CreateHookD3D12CommandQueueInterface(uint64_t* methodVirtualTable)
{
	LOG_ONCE(__FUNCTION__);
	
	CREATE_HOOKPAIR( (LPVOID)methodVirtualTable[44 + 10], D3D12CommandQueueExecuteCommandLists );
	
	MH_EnableHook( (LPVOID)methodVirtualTable[44 + 10] );
	
}