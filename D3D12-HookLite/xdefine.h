#pragma once

enum InputNameEnu
{
	SEM_BINORMAL = 0,
	SEM_BLENDINDICES,
	SEM_BLENDWEIGHT,
	SEM_COLOR,
	SEM_NORMAL,
	SEM_POSITION,
	SEM_POSITIONT,
	SEM_PSIZE,
	SEM_TANGENT,
	SEM_TEXCOORD,
	SEM_FOG,
	SEM_TESSFACTOR,
	SEM_inputnum,
	SEM_ERROR,
	SEM_forcedw = 0xffffffff
};


static const char* sem_name[SEM_inputnum] =
{

	"BINORMAL",
	"BLENDINDICES",
	"BLENDWEIGHT",
	"COLOR",
	"NORMAL",
	"POSITION",
	"POSITIONT",
	"PSIZE",
	"TANGENT",
	"TEXCOORD",
	"FOG",
	"TESSFACTOR"
};



enum CommandEnum
{
	Device_QueryInterface = 1,
	Device_AddRef,
	Device_Release,
	Device_GetPrivateData,
	Device_SetPrivateData,
	Device_SetPrivateDataInterface,
	Device_SetName,
	Device_GetNodeCount,
	Device_CreateCommandQueue,
	Device_CreateCommandAllocator,
	Device_CreateGraphicsPipelineState,
	Device_CreateComputePipelineState,
	Device_CreateCommandList,
	Device_CheckFeatureSupport,
	Device_CreateDescriptorHeap,
	Device_GetDescriptorHandleIncrementSize,
	Device_CreateRootSignature,
	Device_CreateConstantBufferView,
	Device_CreateShaderResourceView,
	Device_CreateUnorderedAccessView,
	Device_CreateRenderTargetView,
	Device_CreateDepthStencilView,
	Device_CreateSampler,
	Device_CopyDescriptors,
	Device_CopyDescriptorsSimple,
	Device_GetResourceAllocationInfo,
	Device_GetCustomHeapProperties,
	Device_CreateCommittedResource,
	Device_CreateHeap,
	Device_CreatePlacedResource,
	Device_CreateReservedResource,
	Device_CreateSharedHandle,
	Device_OpenSharedHandle,
	Device_OpenSharedHandleByName,
	Device_MakeResident,
	Device_Evict,
	Device_CreateFence,
	Device_GetDeviceRemovedReason,
	Device_GetCopyableFootprints,
	Device_CreateQueryHeap,
	Device_SetStablePowerState,
	Device_CreateCommandSignature,
	Device_GetResourceTiling,
	Device_GetAdapterLuid,



	CommandList_QueryInterface,
	CommandList_AddRef,
	CommandList_Release,
	CommandList_GetPrivateData,
	CommandList_SetPrivateData,
	CommandList_SetPrivateDataInterface,
	CommandList_SetName,
	CommandList_GetDevice,
	CommandList_GetType,
	CommandList_Close,
	CommandList_Reset,
	CommandList_ClearState,
	CommandList_DrawInstanced,
	CommandList_DrawIndexedInstanced,
	CommandList_Dispatch,
	CommandList_CopyBufferRegion,
	CommandList_CopyTextureRegion,
	CommandList_CopyResource,
	CommandList_CopyTiles,
	CommandList_ResolveSubresource,
	CommandList_IASetPrimitiveTopology,
	CommandList_RSSetViewports,
	CommandList_RSSetScissorRects,
	CommandList_OMSetBlendFactor,
	CommandList_OMSetStencilRef,
	CommandList_SetPipelineState,
	CommandList_ResourceBarrier,
	CommandList_ExecuteBundle,
	CommandList_SetDescriptorHeaps,
	CommandList_SetComputeRootSignature,
	CommandList_SetGraphicsRootSignature,
	CommandList_SetComputeRootDescriptorTable,
	CommandList_SetGraphicsRootDescriptorTable,
	CommandList_SetComputeRoot32BitConstant,
	CommandList_SetGraphicsRoot32BitConstant,
	CommandList_SetComputeRoot32BitConstants,
	CommandList_SetGraphicsRoot32BitConstants,
	CommandList_SetComputeRootConstantBufferView,
	CommandList_SetGraphicsRootConstantBufferView,
	CommandList_SetComputeRootShaderResourceView,
	CommandList_SetGraphicsRootShaderResourceView,
	CommandList_SetComputeRootUnorderedAccessView,
	CommandList_SetGraphicsRootUnorderedAccessView,
	CommandList_IASetIndexBuffer,
	CommandList_IASetVertexBuffers,
	CommandList_SOSetTargets,
	CommandList_OMSetRenderTargets,
	CommandList_ClearDepthStencilView,
	CommandList_ClearRenderTargetView,
	CommandList_ClearUnorderedAccessViewUint,
	CommandList_ClearUnorderedAccessViewFloat,
	CommandList_DiscardResource,
		CommandList_BeginQuery,
		CommandList_EndQuery,
		CommandList_ResolveQueryData,
		CommandList_SetPredication,
		CommandList_SetMarker,
		CommandList_BeginEvent,
	CommandList_EndEvent,
	CommandList_ExecuteIndirect,


	CommandQueue_EndEvent,
	CommandQueue_Signal,
	CommandQueue_BeginEvent,
	Resource_GetDesc,
	Resource_Map,
	Resource_Unmap,
	Resource_GetGPUVirtualAddress,
	DesHeap_GetCPUDescriptorHandleForHeapStart,
	DesHeap_GetGPUDescriptorHandleForHeapStart,

	SWAPCHAIN_PRESENT,
		last_frame,
		end_frame,
		end_File,

	forcedw = 0xffffffff
};


inline const char *enum_to_string(CommandEnum t) {
	switch (t)
	{
	case Device_QueryInterface:
		return "Device_QueryInterface";
	case Device_AddRef:
		return "Device_AddRef";
	case Device_Release:
		return "Device_Release";
	case Device_GetPrivateData:
		return "Device_GetPrivateData";
	case Device_SetPrivateData:
		return "Device_SetPrivateData";
	case Device_SetPrivateDataInterface:
		return "Device_SetPrivateDataInterface";
	case Device_SetName:
		return "Device_SetName";
	case Device_GetNodeCount:
		return "Device_GetNodeCount";
	case Device_CreateCommandQueue:
		return "Device_GetNodeCount";
	case Device_CreateCommandAllocator:
		return "Device_CreateCommandAllocator";
	case Device_CreateGraphicsPipelineState:
		return "Device_CreateGraphicsPipelineState";
	case Device_CreateComputePipelineState:
		return "Device_CreateComputePipelineState";
	case Device_CreateCommandList:
		return "Device_CreateCommandList";
	case Device_CheckFeatureSupport:
		return "Device_CheckFeatureSupport";
	case Device_CreateDescriptorHeap:
		return "Device_CreateDescriptorHeap";
	case Device_GetDescriptorHandleIncrementSize:
		return "Device_GetDescriptorHandleIncrementSize";
	case Device_CreateRootSignature:
		return "Device_CreateRootSignature";
	case Device_CreateConstantBufferView:
		return "Device_CreateConstantBufferView";
	case Device_CreateShaderResourceView:
		return "Device_CreateShaderResourceView";
	case Device_CreateUnorderedAccessView:
		return "Device_CreateUnorderedAccessView";
	case Device_CreateRenderTargetView:
		return "Device_CreateRenderTargetView";
	case Device_CreateDepthStencilView:
		return "Device_CreateDepthStencilView";
	case Device_CreateSampler:
		return "Device_CreateSampler";
	case Device_CopyDescriptors:
		return "Device_CopyDescriptors";
	case Device_CopyDescriptorsSimple:
		return "Device_CopyDescriptorsSimple";
	case Device_GetResourceAllocationInfo:
		return "Device_GetResourceAllocationInfo";
	case Device_GetCustomHeapProperties:
		return "Device_GetCustomHeapProperties";
	case Device_CreateCommittedResource:
		return "Device_CreateCommittedResource";
	case Device_CreateHeap:
		return "Device_CreateHeap";
	case Device_CreatePlacedResource:
		return "Device_CreatePlacedResource";
	case Device_CreateReservedResource:
		return "Device_CreateReservedResource";
	case Device_CreateSharedHandle:
		return "Device_CreateSharedHandle";
	case Device_OpenSharedHandle:
		return "Device_OpenSharedHandle";
	case Device_OpenSharedHandleByName:
		return "Device_OpenSharedHandleByName";
	case Device_MakeResident:
		return "Device_MakeResident";
	case Device_Evict:
		return "Device_Evict";
	case Device_CreateFence:
		return "Device_CreateFence";
	case Device_GetDeviceRemovedReason:
		return "Device_GetDeviceRemovedReason";
	case Device_GetCopyableFootprints:
		return "Device_GetCopyableFootprints";
	case Device_CreateQueryHeap:
		return "Device_CreateQueryHeap";
	case Device_SetStablePowerState:
		return "Device_SetStablePowerState";
	case Device_CreateCommandSignature:
		return "Device_CreateCommandSignature";
	case Device_GetResourceTiling:
		return "Device_GetResourceTiling";
	case Device_GetAdapterLuid:
		return "Device_GetAdapterLuid";
	case CommandList_QueryInterface:
		return "CommandList_QueryInterface";
	case CommandList_AddRef:
		return "CommandList_AddRef";
	case CommandList_Release:
		return "CommandList_Release";
	case CommandList_GetPrivateData:
		return "CommandList_GetPrivateData";
	case CommandList_SetPrivateData:
		return "CommandList_SetPrivateData";
	case CommandList_SetPrivateDataInterface:
		return "CommandList_SetPrivateDataInterface";
	case CommandList_SetName:
		return "CommandList_SetName";
	case CommandList_GetDevice:
		return "CommandList_GetDevice";
	case CommandList_GetType:
		return "CommandList_GetType";
	case CommandList_Close:
		return "CommandList_Close";
	case CommandList_Reset:
		return "CommandList_Reset";
	case CommandList_ClearState:
		return "CommandList_ClearState";
	case CommandList_DrawInstanced:
		return "CommandList_DrawInstanced";
	case CommandList_DrawIndexedInstanced:
		return "CommandList_DrawIndexedInstanced";
	case CommandList_Dispatch:
		return "CommandList_Dispatch";
	case CommandList_CopyBufferRegion:
		return "CommandList_CopyBufferRegion";
	case CommandList_CopyTextureRegion:
		return "CommandList_CopyTextureRegion";
	case CommandList_CopyResource:
		return "CommandList_CopyResource";
	case CommandList_CopyTiles:
		return "CommandList_CopyTiles";
	case CommandList_ResolveSubresource:
		return "CommandList_ResolveSubresource";
	case CommandList_IASetPrimitiveTopology:
		return "CommandList_IASetPrimitiveTopology";
	case CommandList_RSSetViewports:
		return "CommandList_RSSetViewports";
	case CommandList_RSSetScissorRects:
		return "CommandList_RSSetScissorRects";
	case CommandList_OMSetBlendFactor:
		return "CommandList_OMSetBlendFactor";
	case CommandList_OMSetStencilRef:
		return "CommandList_OMSetStencilRef";
	case CommandList_SetPipelineState:
		return "CommandList_SetPipelineState";
	case CommandList_ResourceBarrier:
		return "CommandList_ResourceBarrier";
	case CommandList_ExecuteBundle:
		return "CommandList_ExecuteBundle";
	case CommandList_SetDescriptorHeaps:
		return "CommandList_SetDescriptorHeaps";
	case CommandList_SetComputeRootSignature:
		return "CommandList_SetComputeRootSignature";
	case CommandList_SetGraphicsRootSignature:
		return "CommandList_SetGraphicsRootSignature";
	case CommandList_SetComputeRootDescriptorTable:
		return "CommandList_SetComputeRootDescriptorTable";
	case CommandList_SetGraphicsRootDescriptorTable:
		return "CommandList_SetGraphicsRootDescriptorTable";
	case CommandList_SetComputeRoot32BitConstant:
		return "CommandList_SetComputeRoot32BitConstant";
	case CommandList_SetGraphicsRoot32BitConstant:
		return "CommandList_SetGraphicsRoot32BitConstant";
	case CommandList_SetComputeRoot32BitConstants:
		return "CommandList_SetComputeRoot32BitConstants";
	case CommandList_SetGraphicsRoot32BitConstants:
		return "CommandList_SetGraphicsRoot32BitConstants";
	case CommandList_SetComputeRootConstantBufferView:
		return "CommandList_SetComputeRootConstantBufferView";
	case CommandList_SetGraphicsRootConstantBufferView:
		return "CommandList_SetGraphicsRootConstantBufferView";
	case CommandList_SetComputeRootShaderResourceView:
		return "CommandList_SetComputeRootShaderResourceView";
	case CommandList_SetGraphicsRootShaderResourceView:
		return "CommandList_SetGraphicsRootShaderResourceView";
	case CommandList_SetComputeRootUnorderedAccessView:
		return "CommandList_SetComputeRootUnorderedAccessView";
	case CommandList_SetGraphicsRootUnorderedAccessView:
		return "CommandList_SetGraphicsRootUnorderedAccessView";
	case CommandList_IASetIndexBuffer:
		return "CommandList_IASetIndexBuffer";
	case CommandList_IASetVertexBuffers:
		return "CommandList_IASetVertexBuffers";
	case CommandList_SOSetTargets:
		return "CommandList_SOSetTargets";
	case CommandList_OMSetRenderTargets:
		return "CommandList_OMSetRenderTargets";
	case CommandList_ClearDepthStencilView:
		return "CommandList_ClearDepthStencilView";
	case CommandList_ClearRenderTargetView:
		return "CommandList_ClearRenderTargetView";
	case CommandList_ClearUnorderedAccessViewUint:
		return "CommandList_ClearUnorderedAccessViewUint";
	case CommandList_ClearUnorderedAccessViewFloat:
		return "CommandList_ClearUnorderedAccessViewFloat";
	case CommandList_DiscardResource:
		return "CommandList_DiscardResource";
	case CommandList_BeginQuery:
		return "CommandList_BeginQuery";
	case CommandList_EndQuery:
		return "CommandList_EndQuery";
	case CommandList_ResolveQueryData:
		return "CommandList_ResolveQueryData";
	case CommandList_SetPredication:
		return "CommandList_SetPredication";
	case CommandList_SetMarker:
		return "CommandList_SetMarker";
	case CommandList_BeginEvent:
		return "CommandList_BeginEvent";
	case CommandList_EndEvent:
		return "CommandList_EndEvent";
	case CommandList_ExecuteIndirect:
		return "CommandList_ExecuteIndirect";
	case CommandQueue_EndEvent:
		return "CommandQueue_EndEvent";
	case CommandQueue_Signal:
		return "CommandQueue_Signal";
	case CommandQueue_BeginEvent:
		return "CommandQueue_BeginEvent";
	case Resource_GetDesc:
		return "Resource_GetDesc";
	case Resource_Map:
		return "Resource_Map";

	case Resource_Unmap:
		return "Resource_Unmap";
// 	case Resource_WriteToSubresource:
// 		return "Resource_WriteToSubresource";
// 	case Resource_ReadFromSubresource:
// 		return "Resource_ReadFromSubresource";

	case Resource_GetGPUVirtualAddress:
		return "Resource_GetGPUVirtualAddress";
	case DesHeap_GetCPUDescriptorHandleForHeapStart:
		return "DesHeap_GetCPUDescriptorHandleForHeapStart";
	case DesHeap_GetGPUDescriptorHandleForHeapStart:
		return "DesHeap_GetGPUDescriptorHandleForHeapStart";
	
	case SWAPCHAIN_PRESENT:
		return "SWAPCHAIN_PRESENT";

	case end_frame:
		return "end_frame";
	case end_File:
		return "end_File";

	case forcedw:
		break;
	default:
		break;
	}


	return "unknown api";
}
