#include <iostream>
#include <windows.h>
#include <winevt.h>
#include <wincodec.h>
#include <shlobj.h>
#include <fstream>
#include <dshow.h>

void WinTakePicture(std::string file) {

    // String to LPCOLESTR
    LPCOLESTR flOle = NULL;
    int len = MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, NULL, 0);
    flOle = (LPCOLESTR) CoTaskMemAlloc(len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, (LPWSTR) flOle, len);
    
    // Some declarations
	ICaptureGraphBuilder2* pBuild;
	IGraphBuilder* pGraph;
	IBaseFilter* pCap;
	IBaseFilter* pMux;
	IMoniker* pMoniker;
	IEnumMoniker* pEnum;
	ICreateDevEnum* pDevEnum;
	IMediaControl* pControl;
    
    // Initialize
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IFilterGraph, (void**)&pGraph);
	pBuild->SetFiltergraph(pGraph);
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
	pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
	
    // Get webcam device
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {

		IPropertyBag* pPropBag;
		pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		VARIANT var;
		VariantInit(&var);

		pPropBag->Read(L"FriendlyName", &var, 0);
        VariantClear(&var);
		
		pPropBag->Release();
		pMoniker->Release();
	}
	pEnum->Reset();
    pEnum->Next(1, &pMoniker, NULL);

    // Bind filters
	pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**) &pCap);
	pGraph->AddFilter(pCap, L"Capture Filter");
	pGraph->QueryInterface(IID_IMediaControl, (void**) &pControl);
	pBuild->SetOutputFileName(&MEDIASUBTYPE_Avi, flOle, &pMux, NULL);
	pBuild->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pCap, NULL, pMux);
	pControl->Run();
	Sleep(1000);

    // Clean up
	pControl->Stop();
	pBuild->Release();
	pGraph->Release();
	pCap->Release();
	pMux->Release();
	pMoniker->Release();
	pEnum->Release();
	pDevEnum->Release();
	pControl->Release();
	CoUninitialize();
}

void TakePicture() {

    // Generate filename
    UUID uuid;
    char* fChar;
    std::string file;
    UuidCreate(&uuid);
    UuidToStringA(&uuid, (RPC_CSTR*) &fChar);
    file = fChar;
    file = "Snoopers/" + file + ".mp4";

    // Take photo and save in snoopers directory
    WinTakePicture(file);
}

int numIter = 0;
DWORD WINAPI EventCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pContext, EVT_HANDLE hEvent) {

    // Call function to take picture
    if (numIter >= 200) {
        TakePicture();
    }

    EvtClose(hEvent);
    return ERROR_SUCCESS;
}

void SubscribeToEvents() {

    // Create subscription
    EVT_HANDLE hSubscription = EvtSubscribe(NULL, NULL, L"Security", L"Event/System[EventID=4625]", EvtCreateBookmark(NULL), NULL, EventCallback, EvtSubscribeStartAfterBookmark);
    if (hSubscription == NULL) {
        DWORD error = GetLastError();
        std::cerr << "Subscription error: " << error << "\n";
        return;
    }

    // Mainloop
    HANDLE hWaitArray[1] = {hSubscription};
    DWORD waitResult;
    while (true) {
        waitResult = WaitForMultipleObjects(1, hWaitArray, FALSE, INFINITE);
        numIter++;
        Sleep(10);
    }

    EvtClose(hSubscription);
}

int main() {
    FreeConsole();
    CreateDirectory("Snoopers", NULL);
    SubscribeToEvents();
    return 0;
}
