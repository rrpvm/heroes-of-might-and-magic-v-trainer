#pragma once
#include <Windows.h>
#include <cstdint>
#include "vec2.h"
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <Psapi.h>
#define player_pointer_1 0x000003B0
#define player_pointer_2 0x000003A8
#define thread_stack_p1 0xCF5F0C//this
#define thread_stack_p2 0x190//and this = thread_stack_0
#define thread_player "THREADSTACK0"
class homm_hero_info
{
public:
	char pad_0000[36]; //0x0000
	char* HeroName; //0x0024
	char pad_0028[8]; //0x0028
	uint32_t attack; //0x0030
	uint32_t defense; //0x0034
	uint32_t magic; //0x0038
	uint32_t knoweledge; //0x003C
	char pad_0040[8]; //0x0040
	uint32_t experience; //0x0048
	uint32_t lvl; //0x004C
	char pad_0050[184]; //0x0050
	uint32_t remaining_movement; //0x0108
	char pad_010C[12]; //0x010C
	uint32_t current_mana; //0x0118
	uint32_t max_mana; //0x011C
	char pad_0120[3872]; //0x0120
}; //Size: 0x1040
class GameHackEngine
{
public:
	GameHackEngine();
	~GameHackEngine();
	void sync_data_with_game();
	template <typename T>
	T read_process_memory(uint32_t base, uint32_t offset) {
		T buffer;
		ReadProcessMemory(this->h_process, (LPCVOID)(base + offset), (LPVOID)&buffer, sizeof(T), 0);
		return buffer;
	}
	template <typename T>
	T read_process_memory(int base, int offset) {
		T buffer;
		ReadProcessMemory(this->h_process, (LPCVOID)(base + offset), (LPVOID)&buffer, sizeof(T), 0);
		return buffer;
	}
	template <typename T>
	void write_process_memory(uint32_t base, uint32_t offset, T data) {
		WriteProcessMemory(this->h_process, (LPVOID)(base + offset), (LPCVOID)data, sizeof(T), 0);
	}
	template <typename T>
	void write_process_memory(int base, int offset, T data) {
		WriteProcessMemory(this->h_process, (LPVOID)(base + offset), &data, sizeof(T), 0);//doesnt work...
	}
	uint32_t& get_attack() { return player_info.attack; };
	uint32_t& get_defense() { return player_info.defense; };
	uint32_t& get_magic() { return player_info.magic; };
	uint32_t& get_knoweledge() { return player_info.knoweledge; };
	uint32_t& get_experience() { return player_info.experience; };
	uint32_t& get_lvl() { return player_info.lvl; };
	uint32_t& get_remaining_movement() { return player_info.remaining_movement; };
	uint32_t& get_current_mana() { return player_info.current_mana; };
	uint32_t& get_max_mana() { return player_info.max_mana; };
	bool& get_inf_movement() { return infinity_movement; };
	bool clicked_in_rect(RECT rc);
	bool clicked_in_rect(const vec2& p1, const vec2& s1);
	bool hover(const vec2& p1, const vec2& s1);
	void update_mouse();
	POINT get_mouse() { return mouse; };
	bool override_data = false;
	void give_resourses();
private:
	DWORD get_process_id(const std::string& proc_name);
	uintptr_t get_module(const std::string& proc_name, DWORD pId);
	DWORD GetThreadStartAddress(HANDLE processHandle, HANDLE hThread);
	HANDLE h_process;
	homm_hero_info player_info;
	bool infinity_movement = false;
	POINT mouse = { NULL, NULL };	
	uintptr_t homm_base;
	uintptr_t thread_stack_0;
	DWORD proc_id;
};
inline GameHackEngine hack_engine;

