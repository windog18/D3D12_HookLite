////////////////
// D3D12 HOOK //
////////////////

#include "main.h"

#include "D3D12DeviceHookInterface.h"
#include "D3D12ResourceHookInterface.h"
#include "D3D12CommandListHookInterface.h"
#include "D3D12CommandQueueHookInterface.h"


#include <sstream>





void HookWindowProc(HWND hWnd);
typedef long(__stdcall* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present12 oPresent12 = NULL;

long __stdcall hkPresent12(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{

	static bool InitOnce = true;
	//Log_WithThreadID("[d3d12]present be called");
	if (InitOnce)
	{

		
		ID3D12Device *dDevice = NULL;
		//get device
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void **)&dDevice)))
		{
			InitOnce = false;
			pSwapChain->GetDevice(__uuidof(dDevice), (void**)&dDevice);


			DXGI_SWAP_CHAIN_DESC sd;

			if (pSwapChain->GetDesc(&sd) == S_OK) {
				stringstream ss;
				ss << sd.BufferCount;
				Log(ss.str().c_str());
				ss << sd.Flags;
				Log(ss.str().c_str());
				ss << sd.Windowed;
				Log(ss.str().c_str());
			}
			HookWindowProc(sd.OutputWindow);
		}



	}

	auto res = oPresent12(pSwapChain, SyncInterval, Flags);
	return res;

}
//////////////////////////////////////////////////////////////////


//=========================================================================================================================//
typedef LRESULT(__stdcall* WINPROC) (_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WINPROC OriginalDefWindowProc = nullptr;
WINPROC OriginalWindowProc = nullptr;


LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	// 	static std::once_flag flag;
	// 	std::call_once(flag, []() {Logger::get("DetourDefWindowProc").information("++ DetourDefWindowProc called"); });
	// 
	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	// 	static std::once_flag flag;
	// 	std::call_once(flag, []() {Logger::get("DetourWindowProc").information("++ DetourWindowProc called"); });
	// 
		//Log("detourWindow Proc called");
	if (Msg != 77) {
		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
	}
	return OriginalWindowProc(hWnd, Msg, wParam, lParam);
}


LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Log(__FUNCTION__);
// 	if (msg != 132 && msg != 32 && msg != 675 && msg != 77 && msg != 83 && msg != 642 && msg != 641 && msg != 7 && msg != 255 && msg != 581) {
// 		stringstream ss;
// 		ss << msg;
// 		std::string outputString = "procHandler be called " + ss.str();
// 		Log(outputString.c_str());
// 	}

	static bool onceCalled = true;
	if (onceCalled) {
		onceCalled = false;
		auto lptrWndProc = reinterpret_cast<WINPROC>(GetWindowLongPtr(hwnd, GWLP_WNDPROC));
		//Log_Simple("handler: %d", lptrWndProc);
		if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
		{
			Log("Couldn't create hook for GWLP_WNDPROC");
		}

		if (MH_EnableHook(lptrWndProc) != MH_OK)
		{
			Log("Couldn't enable GWLP_WNDPROC hook");
		}
		else {
			Log("hook GWLP_WNDPROC success!!!");
		}
	}
	if (msg == 77) { //F1
		if (TempCluster::GetInstance()->IsRecording()) {
			TempCluster::GetInstance()->SetRecording(false);
			TempCluster::GetInstance()->SetFrameTagForAll(CommandEnum::end_File);
			TempCluster::GetInstance()->WriteAllBufferToResult();
			TempCluster::GetInstance()->ResetRecordState();
		}
	}

	//// if(msg == 264){ alt + ?
	////  
	//// }

	//// if(msg == 33){ mouse click
	////}

	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//Log_Simple("windows key pressed! -- 2019-SYL %d", wParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:

		break;
	default:
		break;
	}

	return 0;

}

void HookWindowProc(HWND hWnd)
{
	MH_STATUS ret;
	if ((ret = MH_CreateHook(&DefWindowProcW, &DetourDefWindowProc, reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))) != MH_OK)
	{
		Log("Couldn't create hook for DefWindowProcW: ");
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcW) != MH_OK)
	{
		Log("Couldn't enable DefWindowProcW hook");
	}

	if ((ret = MH_CreateHook(
		&DefWindowProcA,
		&DetourDefWindowProc,
		reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))
		) != MH_OK)
	{
		Log("Couldn't create hook for DefWindowProcA");
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcA) != MH_OK)
	{
		Log("Couldn't enable DefWindowProcW hook");
	}

	auto lptrWndProc = reinterpret_cast<WINPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	if (hWnd == NULL) {
		Log("could not GetWindowLongPtr ");
	}

	if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
	{
		Log("Couldn't create hook for GWLP_WNDPROC");
		return;
	}

	if (MH_EnableHook(lptrWndProc) != MH_OK)
	{
		Log("Couldn't enable GWLP_WNDPROC hook");
	}

}


///////////////////////////////////////////////////////////////

int dx12Thread()
{
	if (dx12::init(dx12::RenderType::D3D12) == dx12::Status::Success)
	{
		MH_Initialize();

		MH_STATUS createResult;
		do{

			createResult = MH_CreateHook((LPVOID)dx12::getMethodsTable()[140], hkPresent12, (LPVOID*)&oPresent12);
			Sleep(10);
			if (createResult != MH_OK) {
				OutputDebugStringA("create failed! sleep for a while");
			}
		} while (createResult != MH_OK);


		try {
			MH_STATUS enableStat = MH_EnableHook((LPVOID)dx12::getMethodsTable()[140]);
			if (enableStat == MH_OK) {
				OutputDebugStringA("enable hook success!");
			}
		}
		catch (...) {
			OutputDebugStringA("hook failed");
		}

		CreateHookD3D12ResourceInterface(dx12::getMethodsTable());
		CreateHookD3D12DeviceInterface(dx12::getMethodsTable());
		//CreateHookD3D12CommandListInterface(dx12::getMethodsTable());

	}

	return 0;
}


//=========================================================================================================================//

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)
{
	DisableThreadLibraryCalls(hInstance);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
// 		GetModuleFileNameA(hInstance, dlldir, 512);
// 		for (size_t i = strlen(dlldir); i > 0; i--) { if (dlldir[i] == '\\') { dlldir[i + 1] = 0; break; } }
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dx12Thread, NULL, 0, NULL);
		break;
	}

	return TRUE;
}
