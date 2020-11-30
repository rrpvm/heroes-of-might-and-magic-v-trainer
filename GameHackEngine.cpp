#include "GameHackEngine.h"
#include "GlobalVariables.h"

std::vector<DWORD> threadList(DWORD pid);
void* GetThreadStackTopAddress_x86(HANDLE hProcess, HANDLE hThread);
GameHackEngine::GameHackEngine()
{
	player_info.attack = 0;
	player_info.defense = 0;
	player_info.knoweledge = 0;
	player_info.experience = 0;
	player_info.lvl = 0;
	player_info.remaining_movement = 0;
	player_info.current_mana = 0;
	player_info.max_mana = 0;
	h_process = NULL;
	this->proc_id = get_process_id("H5_Game.exe");
	if (this->proc_id <= 0)return;
	h_process = OpenProcess(PROCESS_ALL_ACCESS, false, this->proc_id);
	if (!h_process)return;
	this->homm_base = get_module("H5_Game.exe", this->proc_id);
	if (!this->homm_base)return;
	uintptr_t stackaddr_0;
	std::vector<DWORD> threadId = threadList(proc_id);
	HANDLE threadHandle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, *threadId.begin());
	thread_stack_0 = GetThreadStartAddress(h_process, threadHandle);
}
GameHackEngine::~GameHackEngine()
{
	if(h_process)
	CloseHandle(h_process);
}
void GameHackEngine::sync_data_with_game()
{
	int* player = nullptr;
	ReadProcessMemory(this->h_process, (int*)(this->thread_stack_0 - 0x000003A8), &player, 4, 0);
	if (!player)return;
	int check = 0;
	ReadProcessMemory(this->h_process, (int*)((int)player + 0x030), &check, 4, 0);
	if (!check || check > 100000 || check < 0)return;
	static DWORD cooldown = GetTickCount();
	if (override_data)
	{
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x030), &this->get_attack(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x0034), &this->get_defense(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x0038), &this->get_magic(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x003C), &this->get_knoweledge(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x0048), &this->get_experience(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x004C), &this->get_lvl(), 4, 0);		
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x0118), &this->get_current_mana(), 4, 0);
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x011C), &this->get_max_mana(), 4, 0);
		cooldown = GetTickCount();
		//write_process_memory((int)player, 0x030, 4);
	}
	else {	
		if (cooldown + 2000 > GetTickCount()) {
			return;
		}
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x030), &this->get_attack(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x0034), &this->get_defense(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x0038), &this->get_magic(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x003C), &this->get_knoweledge(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x0048), &this->get_experience(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x004C), &this->get_lvl(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x0118), &this->get_current_mana(), 4, 0);
		ReadProcessMemory(this->h_process, (int*)((int)player + 0x011C), &this->get_max_mana(), 4, 0);
	}
	if (infinity_movement)
	{
		int movement = 350000;
		WriteProcessMemory(this->h_process, (LPVOID)((int)player + 0x0108), &movement, 4, 0);
	}
}
bool GameHackEngine::clicked_in_rect(RECT rc)
{
	if (!GetAsyncKeyState(VK_LBUTTON))
	{
		return false;
	}
	if (mouse.x >= rc.left && mouse.x <= rc.right)
	{
		if (mouse.y >= rc.top && mouse.y <= rc.bottom)return true;
			return false;
	}
	return false;
}
bool GameHackEngine::clicked_in_rect(const vec2& p1, const vec2& s1)
{
	if (!GetAsyncKeyState(VK_LBUTTON))
	{
		return false;
	}
	if (mouse.x >= p1.x && mouse.x <= p1.x + s1.x)
	{
		if (mouse.y >= p1.y && mouse.y <= p1.y + s1.y)return true;
		return false;
	}
	return false;
}
bool GameHackEngine::hover(const vec2& p1, const vec2& s1)
{
	if (mouse.x >= p1.x && mouse.x <= p1.x + s1.x)
	{
		if (mouse.y >= p1.y && mouse.y <= p1.y + s1.y)return true;
		return false;
	}
	return false;
}
void GameHackEngine::update_mouse()
{
	GetCursorPos(&this->mouse);
	ScreenToClient(global_vars.hWnd, &this->mouse);

}
void GameHackEngine::give_resourses()
{
	const static int given_gold = 1337000;
	const static int other_res_given = 228;
	int resourses_ptr = 0;
	ReadProcessMemory(this->h_process, (int*)(this->thread_stack_0 - 0x410), &resourses_ptr, 4, 0);
	if (!resourses_ptr)return;
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x0), &other_res_given, 4, 0);//derevo
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x4), &other_res_given, 4, 0);//rtut
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x8), &other_res_given, 4, 0);//etc
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0xC), &other_res_given, 4, 0);
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x10), &other_res_given, 4, 0);
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x14), &other_res_given, 4, 0);
	WriteProcessMemory(this->h_process, (LPVOID)(resourses_ptr + 0x18), &given_gold, 4, 0);//gold
}
DWORD GameHackEngine::get_process_id(const std::string& proc_name)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnap, &pe32))
	{
		do {
			if (!strcmp(pe32.szExeFile, proc_name.c_str()))
			{
				CloseHandle(hSnap);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnap, &pe32));
	}
	return -1;
}
uintptr_t GameHackEngine::get_module(const std::string& proc_name, DWORD pId)
{
	if (pId <= 0 )return -1337;
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hDump = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,pId );
	if (Module32First(hDump, &me32))
	{
		do {
			if (!strcmp(proc_name.c_str(), me32.szModule))
			{
				CloseHandle(hDump);
				return (uintptr_t)me32.modBaseAddr;
				break;//dumb code
			}
		} while (Module32Next(hDump, &me32));
	}
	return -1337;
}
DWORD GameHackEngine::GetThreadStartAddress(HANDLE processHandle, HANDLE hThread)
{
	/* rewritten from https://github.com/cheat-engine/cheat-engine/blob/master/Cheat%20Engine/CEFuncProc.pas#L3080 */
	DWORD used = 0, ret = 0;
	DWORD stacktop = 0, result = 0;

	MODULEINFO mi;

	GetModuleInformation(processHandle, GetModuleHandle("kernel32.dll"), &mi, sizeof(mi));
	stacktop = (DWORD)GetThreadStackTopAddress_x86(processHandle, hThread);


	CloseHandle(hThread);

	if (stacktop) {
		//find the stack entry pointing to the function that calls "ExitXXXXXThread"
		//Fun thing to note: It's the first entry that points to a address in kernel32

		DWORD* buf32 = new DWORD[4096];

		if (ReadProcessMemory(processHandle, (LPCVOID)(stacktop - 4096), buf32, 4096, NULL)) {
			for (int i = 4096 / 4 - 1; i >= 0; --i) {
				if (buf32[i] >= (DWORD)mi.lpBaseOfDll && buf32[i] <= (DWORD)mi.lpBaseOfDll + mi.SizeOfImage) {
					result = stacktop - 4096 + i * 4;
					break;
				}

			}
		}

		delete buf32;
	}

	return result;
}
std::vector<DWORD> threadList(DWORD pid) {
	/* solution from http://stackoverflow.com/questions/1206878/enumerating-threads-in-windows */
	std::vector<DWORD> vect = std::vector<DWORD>();
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h == INVALID_HANDLE_VALUE)
		return vect;

	THREADENTRY32 te;
	te.dwSize = sizeof(te);
	if (Thread32First(h, &te)) {
		do {
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) +
				sizeof(te.th32OwnerProcessID)) {


				if (te.th32OwnerProcessID == pid) {
					printf("PID: %04d Thread ID: 0x%04x\n", te.th32OwnerProcessID, te.th32ThreadID);
					vect.push_back(te.th32ThreadID);
				}

			}
			te.dwSize = sizeof(te);
		} while (Thread32Next(h, &te));
	}

	return vect;
}
typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, * PCLIENT_ID;
typedef struct _THREAD_BASIC_INFORMATION
{
	NTSTATUS                ExitStatus;
	PVOID                   TebBaseAddress;
	CLIENT_ID               ClientId;
	KAFFINITY               AffinityMask;
	DWORD                Priority;
	DWORD                BasePriority;
} THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;
enum THREADINFOCLASS
{
	ThreadBasicInformation,
};
void* GetThreadStackTopAddress_x86(HANDLE hProcess, HANDLE hThread) {

	LPSTR moduleName = (LPSTR)"ntdll.dll";

	bool loadedManually = false;
	HMODULE module = GetModuleHandle(moduleName);

	if (!module)
	{
		module = LoadLibrary(moduleName);
		loadedManually = true;
	}

	NTSTATUS(__stdcall * NtQueryInformationThread)(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);
	NtQueryInformationThread = reinterpret_cast<decltype(NtQueryInformationThread)>(GetProcAddress(module, "NtQueryInformationThread"));

	if (NtQueryInformationThread)
	{
		NT_TIB tib = { 0 };
		THREAD_BASIC_INFORMATION tbi = { 0 };

		NTSTATUS status = NtQueryInformationThread(hThread, ThreadBasicInformation, &tbi, sizeof(tbi), nullptr);
		if (status >= 0)
		{
			ReadProcessMemory(hProcess, tbi.TebBaseAddress, &tib, sizeof(tbi), nullptr);

			if (loadedManually)
			{
				FreeLibrary(module);
			}
			return tib.StackBase;
		}
	}


	if (loadedManually)
	{
		FreeLibrary(module);
	}

	return nullptr;
}