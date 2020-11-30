#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <functional>
#include "vec2.h"
#include "Color.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#define COL_SIZE 40
#define RECT_SIZE 30
#define TXT_UP 10
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


typedef struct CUSTOMVERTEX
{
	float x, y, z, rhw;
	D3DCOLOR color;
};
class renderer 
{
public:
	~renderer() {
		pDevice->Release();
		d3d9_object->Release();
		game_font->Release();

	}
	renderer() {

	}
	void SetPDevice(IDirect3DDevice9* dev);
	void SetD3D9_object(IDirect3D9* d3d_obj);
	void Init();
	void main_frame();
	void end_frame();
	void second_frame();
	IDirect3DDevice9** GetPDevice();
	IDirect3D9** GetDirectxObject();	
private:
	IDirect3DDevice9* pDevice = nullptr;
	IDirect3D9* d3d9_object = nullptr;
	IDirect3DVertexBuffer9* main_vertex_buffer = nullptr;
	ID3DXFont* game_font = nullptr;
	IDirect3DVertexBuffer9* create_vertex_buffer(UINT Length, DWORD FVF = CUSTOMFVF, D3DPOOL Pool = D3DPOOL_MANAGED, HANDLE* pSharedHandle = NULL, DWORD usage = D3DUSAGE_WRITEONLY);
	FORCEINLINE  void clear(D3DRECT* rc, const Color& color = Color());
	FORCEINLINE  void clear_all(const Color& color = Color());
public:
	void draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, const Color& color);
	void draw_filled_rectangle(const vec2& pos, const vec2& size, const Color& color);
	void draw_filled_rectangle(const vec2& pos, const vec2& size, DWORD color);
	void draw_rectangle(const vec2& pos, const vec2& size, const Color& color);
	void draw_line(const vec2& pos, const vec2& end, const Color& color);
	void draw_string(const vec2& pos, const std::string& message, const Color& color, bool outlined = false, bool centered = true);
	void draw_string(const vec2& pos, const vec2& size, std::string message, const Color& color, bool outlined = false, bool centered = true);//size = right bottom
	vec2 calculate_txt_size(const std::string& message, const Color& color, bool outlined = false, bool centered = true);//move to engine
};
inline renderer render;
class btn {	
public:
	btn() {
		func = nullptr;
		rc = { 0,0,0,0 };
	}
	void set_func(std::function<void()> f) { func = f; };
	void set_rc(RECT f) { rc = f; };
	RECT rc;
	std::function<void()> func;
	~btn() {};
};
struct column {
	std::string name;
	void* ptr{ nullptr };
	RECT rc;
	btn btn_up;
	btn btn_down;
	column() {
		rc = { 0,0,0,0 };

	};
};
class RenderHelper
{
public:
	static const vec2& calculate_txt_size(const HDC& hdc, const std::string& txt)
	{
		RECT* lp_rect = new RECT();
		DrawText(hdc, txt.c_str(), txt.length(), lp_rect, DT_CALCRECT);
		vec2 tmp;
		tmp.x = lp_rect->right;
		tmp.y = lp_rect->bottom;
		delete lp_rect;
		return tmp;
	}
	void add_col(std::string s, void* ptr, std::function<void()> btn_up_f, std::function<void()> btn_down_f);
	void render_columns();
private:
	std::vector<column>col_list;
	int margin_top = 0;
};
inline RenderHelper renderer_;

