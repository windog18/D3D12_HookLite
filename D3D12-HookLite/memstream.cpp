
#include "memstream.h"
//#include "GlobalGathering.h"


const int MaxDatasize = 20 * 1024 * 1024;

const int MaxCallSequenceSize = 1024 * 1024 * 1024;


// void SaveRecordToFile(const char* name)
// {
// 	MemStream::getstream()->writetoFile(name);
// }
// 
// void ResetStream()
// {
// 	MemStream::getstream()->reset();
// }



void MemStream::writetoFile(const char* pname)
{
	FILE *f;
	fopen_s(&f, pname, "ab");

	fwrite(&streamcount, sizeof(INT64), 1, f);
	fwrite(&memhandle[0], streamcount, 1, f);

	fclose(f);

}

void MemStream::writeCommandNameToFile(const char * pname)
{

	ofstream fout(pname,ios::app);
	fout << nameListCache.str();
}

void MemStream::readfromfile(const char* pname)
{
	reset();

	FILE *f;
	fopen_s(&f, pname, "rb");

	INT64 memcount = 0;
	fread(&memcount, sizeof(INT64), 1, f);
	fread(&memhandle[0], memcount, 1, f);

	fclose(f);
}

void MemStream::reset()
{
	streamcount = 0;
	streamhandle = &memhandle[0];

	nameListCache.clear();
}



MemStream::MemStream()
{
	memhandle.clear();

	beginRecordPresent = false;

	prepareendfile = false;

}

MemStream::~MemStream()
{
	memhandle.clear();

}

void MemStream::init()
{
	//Log_Detail_0(Enum_other1,"alloc new memStream!!!");
	//memhandle = new unsigned char[MaxDatasize];
	memhandle.resize(MaxDatasize);
	streamcount = 0;
	streamhandle = &memhandle[0];

	nameListCache.clear();
}




void MemStream::write(const void* pdata, size_t datasize)
{

	WriteToMemStream(pdata, datasize);
// 	if (datasize + streamcount >= MaxDatasize) {
// 		memhandle.resize(memhandle.size() * 2);
// 		
// 		//Log_Detail_0(Enum_other1, "error! out of range");
// 		return;
// 	}
// 	memcpy((void*)streamhandle, pdata, datasize);
// 
// 	streamhandle = (unsigned char*)streamhandle + datasize;
// 
// 	streamcount += datasize;
}


void MemStream::write(CommandEnum enu)
{
	
	if (prepareendfile == true)
	{
		prepareendfile = false;
		beginRecordPresent = true;
		write(CommandEnum::SWAPCHAIN_PRESENT);
	}
	

	int datasize = sizeof(CommandEnum);
	WriteToMemStream(&enu, datasize);
	
/*
	{
		std::lock_guard<std::mutex> locker(m_gMutex);
		std::string commandName = enum_to_string(enu);
		commandName = commandName + "\n";

		nameListCache << commandName;
	}
*/

	
}






void MemStream::read(const void* pdata, size_t datasize)
{

	memcpy((void*)pdata,(void*)streamhandle, datasize);

	streamhandle = (unsigned char*)streamhandle + datasize;

	streamcount += datasize;
}

void MemStream::write(const D3D12_INPUT_LAYOUT_DESC& desc)
{

	write(desc.NumElements);

	for (UINT i = 0; i < desc.NumElements; i++)
	{
		const char* pname = desc.pInputElementDescs[i].SemanticName;
		size_t lent = strlen(pname);
		write(lent);
		write(pname, lent);

		write(desc.pInputElementDescs[i].SemanticIndex);
		write(desc.pInputElementDescs[i].Format);
		write(desc.pInputElementDescs[i].InputSlot);
		write(desc.pInputElementDescs[i].AlignedByteOffset);
		write(desc.pInputElementDescs[i].InputSlotClass);
		write(desc.pInputElementDescs[i].InstanceDataStepRate);

	}

	
}

void MemStream::read(D3D12_COMMAND_SIGNATURE_DESC& desc)
{
	read(desc.ByteStride);
	read(desc.NodeMask);
	read(desc.NumArgumentDescs);
	desc.pArgumentDescs = new D3D12_INDIRECT_ARGUMENT_DESC[desc.NumArgumentDescs];
	for (UINT i = 0; i < desc.NumArgumentDescs; i++)
	{
		read(desc.pArgumentDescs[i]);
	}

}

void MemStream::write(const D3D12_COMMAND_SIGNATURE_DESC& desc)
{

	write(desc.ByteStride);
	write(desc.NodeMask);
	write(desc.NumArgumentDescs);

	for (UINT i = 0; i < desc.NumArgumentDescs; i++)
	{
		write(desc.pArgumentDescs[i]);
	}
}

void MemStream::write(const D3D12_STREAM_OUTPUT_DESC& desc)
{
	write(desc.NumEntries);

	if (desc.NumEntries == 0)
	{

	}
	else
	{
		//assert(0);
	}

	write(desc.NumStrides);

	if (desc.NumStrides == 0)
	{

	}
	else
	{
		//assert(0);
	}
	write(desc.RasterizedStream);
}

void MemStream::write(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc)
{
	write(desc.pRootSignature);

	write(desc.Flags);

	write(desc.NodeMask);

	size_t psosize = desc.CachedPSO.CachedBlobSizeInBytes;

	write(psosize);

	write(desc.CachedPSO.pCachedBlob, psosize);

	size_t codesize = desc.CS.BytecodeLength;

	write(codesize);

	write(desc.CS.pShaderBytecode, codesize);

}

void MemStream::write(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
	write(desc.pRootSignature);

	write(desc.BlendState);

	size_t psosize = desc.CachedPSO.CachedBlobSizeInBytes;
	write(psosize);
	if (psosize > 0)
	{
		write(desc.CachedPSO.pCachedBlob, psosize);
	}


	write(desc.DepthStencilState);

	size_t codesize = desc.DS.BytecodeLength;
	write(codesize);
	if (codesize > 0)
	{
		write(desc.DS.pShaderBytecode, codesize);
	}

	write(desc.DSVFormat);

	write(desc.Flags);

	codesize = desc.GS.BytecodeLength;
	write(codesize);
	if (codesize > 0)
	{
		write(desc.GS.pShaderBytecode, codesize);
	}

	codesize = desc.HS.BytecodeLength;
	write(codesize);
	if (codesize > 0)
	{
		write(desc.HS.pShaderBytecode, codesize);
	}

	write(desc.IBStripCutValue);

	write(desc.InputLayout);

	write(desc.NodeMask);

	write(desc.NumRenderTargets);

	write(desc.PrimitiveTopologyType);

	codesize = desc.PS.BytecodeLength;
	write(codesize);
	if (codesize > 0)
	{
		write(desc.PS.pShaderBytecode, codesize);
	}

	write(desc.RasterizerState);

	write(desc.RTVFormats, 8 * sizeof(DXGI_FORMAT));

	write(desc.SampleDesc);

	write(desc.SampleMask);

	write(desc.StreamOutput);

	codesize = desc.VS.BytecodeLength;
	write(codesize);
	if (codesize > 0)
	{
		write(desc.VS.pShaderBytecode, codesize);
	}
}



void MemStream::read( D3D12_STREAM_OUTPUT_DESC& desc)
{
	read(desc.NumEntries);

	if (desc.NumEntries == 0)
	{
		desc.pSODeclaration = NULL;
	}
	else
	{
		//assert(0);
	}

	read(desc.NumStrides);

	if (desc.NumStrides == 0)
	{
		desc.pBufferStrides = NULL;
	}
	else
	{
		//assert(0);
	}
	read(desc.RasterizedStream);
}




void MemStream::read(D3D12_INPUT_LAYOUT_DESC& desc)
{
	read(desc.NumElements);


	LPCSTR *semename = new LPCSTR[desc.NumElements];

	UINT* indexarray = new UINT[desc.NumElements];

	DXGI_FORMAT* formatarray = new DXGI_FORMAT[desc.NumElements];

	UINT* slotarray = new UINT[desc.NumElements];

	UINT* offsetarray = new UINT[desc.NumElements];

	D3D12_INPUT_CLASSIFICATION* classarray = new D3D12_INPUT_CLASSIFICATION[desc.NumElements];

	UINT* ratearray = new UINT[desc.NumElements];



	for (UINT i = 0; i < desc.NumElements; i++)
	{

		InputNameEnu inputenu;
		read(inputenu);

		semename[i] = sem_name[inputenu];
		read(indexarray[i]);
		read(formatarray[i]);
		read(slotarray[i]);
		read(offsetarray[i]);
		read(classarray[i]);
		read(ratearray[i]);
	}

	UINT8* descdata = new UINT8[sizeof(D3D12_INPUT_ELEMENT_DESC)*desc.NumElements];

	UINT8* adr = descdata;
	for (UINT i = 0; i < desc.NumElements; i++)
	{
		D3D12_INPUT_ELEMENT_DESC desc = { semename[i],indexarray[i], formatarray[i], slotarray[i], offsetarray[i],
			classarray[i], ratearray[i] };

		memcpy(adr, &desc, sizeof(D3D12_INPUT_ELEMENT_DESC));
		adr += sizeof(D3D12_INPUT_ELEMENT_DESC);
	}

	desc.pInputElementDescs = (const D3D12_INPUT_ELEMENT_DESC*)descdata;
}


