#include "RenderHelper.h"
#include "GameHackEngine.h"
#include "GlobalVariables.h"
void RenderHelper::add_col(std::string s, void* ptr, std::function<void()> btn_up_f, std::function<void()> btn_down_f)
{
	column col;
	int counter = 0;
	col.name = s;
	col.ptr = ptr;
	int top = 40 + (margin_top * COL_SIZE) + margin_top;
	int bottom = top + COL_SIZE;
	col.rc = { 0, top,200,bottom };
	col.btn_down.set_func(btn_down_f);
	col.btn_up.set_func(btn_up_f);
	RECT _rs_btn_dwn = { 175,top + (COL_SIZE - RECT_SIZE) / 2 + 15, 185, bottom - (COL_SIZE - RECT_SIZE) / 2 - 5 };
	RECT _rs_btn_up = { 175,top + (COL_SIZE - RECT_SIZE) / 2  , 185, bottom - (COL_SIZE - RECT_SIZE) / 2 - 20 };
	col.btn_down.set_rc(_rs_btn_dwn);
	col.btn_up.set_rc(_rs_btn_up);
	col_list.emplace_back(col);
	margin_top++;
}

void RenderHelper::render_columns()
{
	for (column& cols : col_list)
	{
		vec2 size_txt = render.calculate_txt_size(cols.name,Color(0,0,0),false, false );
		RECT txt = { 320 - (size_txt.x / 2),cols.rc.top, cols.rc.right, cols.rc.bottom };
		RECT line = { 0,cols.rc.bottom, 640, cols.rc.bottom + 1 };
		RECT rect = { 25,cols.rc.top + (COL_SIZE - RECT_SIZE) / 2, 160, cols.rc.bottom - (COL_SIZE - RECT_SIZE) / 2 };		
		render.draw_string(vec2(txt.left, txt.top), cols.name, Color(188, 188, 188), true, true);
		render.draw_filled_rectangle(vec2(rect.left, rect.top), vec2(rect.right - rect.left, rect.bottom - rect.top), Color(0, 0, 0));
		render.draw_filled_rectangle(vec2(line.left, line.top), vec2(line.right - line.left, line.bottom - line.top), Color(0, 0, 0));		
		bool clicked_up = hack_engine.clicked_in_rect(cols.btn_up.rc);
		bool clicked_down = hack_engine.clicked_in_rect(cols.btn_down.rc);
		render.draw_filled_rectangle(vec2(cols.btn_down.rc.left, cols.btn_down.rc.top), vec2(cols.btn_down.rc.right - cols.btn_down.rc.left, cols.btn_down.rc.bottom - cols.btn_down.rc.top), Color(44, 0, 0));
		render.draw_filled_rectangle(vec2(cols.btn_up.rc.left, cols.btn_up.rc.top), vec2(cols.btn_up.rc.right - cols.btn_up.rc.left, cols.btn_up.rc.bottom - cols.btn_up.rc.top), Color(44, 0, 0));		
		if (clicked_up)
		{
			cols.btn_up.func();
		}
		if (clicked_down)
		{
			cols.btn_down.func();
		}		
		uint32_t value = *(uint32_t*)(cols.ptr);	
		render.draw_string(vec2(rect.left, rect.top), std::to_string(value), Color(188, 188, 188), true, true);
	}
}
void renderer::SetPDevice(IDirect3DDevice9* dev)
{
	this->pDevice = dev;
}
void renderer::SetD3D9_object(IDirect3D9* d3d_obj)
{
	this->d3d9_object = d3d9_object;
}
void renderer::Init()
{
	main_vertex_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 512);
	D3DXCreateFontA(this->pDevice, 28, 0, 400, 2, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, 0, "Calibri", &this->game_font);
}
IDirect3DDevice9** renderer::GetPDevice()
{
	return &this->pDevice;
}
IDirect3D9** renderer::GetDirectxObject()
{
	return &this->d3d9_object;
}
IDirect3DVertexBuffer9* renderer::create_vertex_buffer(UINT Length, DWORD FVF, D3DPOOL Pool, HANDLE* pSharedHandle, DWORD usage)
{
	IDirect3DVertexBuffer9* buffer = nullptr;
	this->pDevice->CreateVertexBuffer(Length,
		usage,
		FVF,
		Pool,
		&buffer,
		pSharedHandle);
	return buffer;
}
void renderer::main_frame()
{
	this->pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 0.f, 0);
	this->pDevice->BeginScene();
	this->pDevice->SetTexture(0, 0);
	this->pDevice->SetStreamSource(0, 0, 0, 0);
	this->pDevice->SetFVF(CUSTOMFVF);
	this->pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	this->pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	this->pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);//фикс
	this->pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	this->pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	

	static const int offsetX = 1280/9;
	static const int offsetY = 25;
	static const int rect_size_y = 50;
	static std::string arr[9] = { "LVL", "EXP","ATTACK", "SHIELD","KNOWLEDGES", "MAGIC","MANA","MAX MANA","inf.mov" };
	static void* ptrs[9] = { &hack_engine.get_lvl(),&hack_engine.get_experience(),&hack_engine.get_attack(), &hack_engine.get_defense(),&hack_engine.get_knoweledge(), &hack_engine.get_magic(),&hack_engine.get_current_mana(),&hack_engine.get_max_mana(),&hack_engine.get_inf_movement() };
	static std::function<void()> fncts[18] = {
		[]()->void {
			hack_engine.get_lvl()++;

			},
		[]()->void {
				uint32_t& lvl = hack_engine.get_lvl();
				if (lvl > 0)lvl--;
		},
		[]()->void {
			hack_engine.get_experience() += 5000;
			},
		[]()->void {
					uint32_t& exp = hack_engine.get_experience();
					if (exp >= 2000)exp -= 2000;
		 },
		[]()->void {
			hack_engine.get_attack() += 3;
			},
		[]()->void {
				uint32_t& tmp = hack_engine.get_attack();
					if (tmp >= 3)tmp -= 3;
		},
		[]()->void {
			hack_engine.get_defense() += 3;
			},
		[]()->void {
				uint32_t& tmp = hack_engine.get_defense();
					if (tmp >= 1)tmp -= 1;
		},
		[]()->void {
			hack_engine.get_knoweledge() += 3;
		},
		[]()->void {
			uint32_t& tmp = hack_engine.get_knoweledge();
					if (tmp >= 1)tmp -= 1;
		},
		[]()->void {
			hack_engine.get_magic() += 3;
		},
		[]()->void {
			uint32_t& tmp = hack_engine.get_magic();
					if (tmp >= 1)tmp -= 1;
		},
		[]()->void {
			hack_engine.get_current_mana() += 250;
		},
		[]()->void {
			uint32_t& tmp = hack_engine.get_current_mana();
					if (tmp >= 50)tmp -= 50;
			
		},
		[]()->void {
			hack_engine.get_max_mana() += 250;
		},
		[]()->void {
			uint32_t& tmp = hack_engine.get_max_mana();
					if (tmp >= 50)tmp -= 50;
		},
		[]()->void {
			hack_engine.get_inf_movement() = true;
		},
		[]()->void {
			hack_engine.get_inf_movement() = false;
		}
	};
	int counter = 0;
	static DWORD last_override_data = GetTickCount();
	if(last_override_data + 1200 < GetTickCount())
	hack_engine.override_data = false;
	for (int i = 0; i < 9; i++)
	{
		uint32_t value = *(uint32_t*)(ptrs[i]);
		vec2 txt_size = this->calculate_txt_size(arr[i], Color(0, 0, 0), true, true);
		vec2 label_txt_size = this->calculate_txt_size(std::to_string(value), Color(0, 0, 0), true, true);
		this->draw_string(vec2((offsetX * (i + 1)) - txt_size.x / 2 - offsetX / 2, offsetY), arr[i], Color(22, 22, 22), false, false);
		this->draw_filled_rectangle(vec2(offsetX * (i)+1, 80), vec2(offsetX - 2, rect_size_y), Color(0, 0, 0));
		this->draw_line(vec2(offsetX * (i + 1), 0), vec2(offsetX * (i + 1), 360), Color(55, 55, 55));
		this->draw_string(vec2(offsetX * (i + 1) - label_txt_size.x / 2 - offsetX / 2, 80 + rect_size_y / 2 - label_txt_size.y / 2), std::to_string(value), Color(199, 199, 199), false, false);
		vec2 p1_l = vec2(offsetX * i, 180);
		vec2 p2_l = vec2(offsetX * (i + 1) - offsetX / 2 - 10, 150);
		vec2 p3_l = vec2(offsetX * (i + 1) - offsetX / 2 - 10, 210);		
		if (GetFocus() == global_vars.hWnd && hack_engine.clicked_in_rect(vec2(offsetX * i, 150), vec2(offsetX / 2 - 10, 60)))
		{
			fncts[counter]();
			hack_engine.override_data = true;
		}
		counter++;
		vec2 p1_r = vec2(offsetX * (i + 1) - offsetX / 2 + 10, 210);
		vec2 p2_r = vec2(offsetX * (i + 1) - offsetX / 2 + 10, 150);
		vec2 p3_r = vec2(offsetX * (i + 1), 180);
		this->draw_triangle(p1_l, p2_l, p3_l, Color(22, 222, 22));
		this->draw_triangle(p1_r, p2_r, p3_r, Color(22, 222, 22));	
		if (GetFocus() == global_vars.hWnd && hack_engine.clicked_in_rect(vec2(offsetX * (i + 1) - offsetX / 2 - 20, 150), vec2(offsetX / 2 + 10, 60)))
		{
			fncts[counter]();
			hack_engine.override_data = true;
			last_override_data = GetTickCount();
		}
		counter++;
	}	
	static const vec2 resourse_button = vec2(50, 250);
	static const vec2 resouse_btn_size = vec2(1180, 50);
	this->draw_filled_rectangle(resourse_button, resouse_btn_size, hack_engine.hover(resourse_button, resouse_btn_size) ? Color(110,110,150,120) : Color(110, 110, 110,100));
	if (GetFocus() == global_vars.hWnd &&  hack_engine.clicked_in_rect(resourse_button, resouse_btn_size))
	{
		hack_engine.give_resourses();
	}
	this->draw_rectangle(resourse_button, resouse_btn_size, Color(44, 44, 44));
	this->draw_rectangle(vec2(resourse_button.x - 2, resourse_button.y - 2), vec2(1180 + 4, 50 + 4), Color(0, 0, 0));
	this->draw_string(resourse_button, vec2(1180, 55), "resourses",Color(11,11,11), false, true);
}
void renderer::end_frame()
{	
	this->pDevice->EndScene();
	this->pDevice->Present(NULL, NULL, NULL, NULL);
}
void renderer::second_frame()
{

}
FORCEINLINE void renderer::clear(D3DRECT* rc, const Color& color)
{
	if (this->pDevice)this->pDevice->Clear(0L, rc, D3DCLEAR_TARGET, color.to_dword(), 0.f, NULL);
}
void renderer::clear_all(const Color& color)
{
	if (this->pDevice)this->pDevice->Clear(0L, nullptr, D3DCLEAR_TARGET, color.to_dword(), 0.f, NULL);
}
void renderer::draw_triangle(const vec2& p1, const vec2& p2, const vec2& p3, const Color& color)
{
	DWORD col = color.to_dword();
	CUSTOMVERTEX vertecies_[3]{
		p1.x, p1.y, 0.f, 1.f,col,
		p2.x, p2.y, 0.f, 1.f,col,
		p3.x, p3.y, 0.f, 1.f,col
	};
	
	VOID* pVoid;    // the void pointer
	IDirect3DVertexBuffer9* v_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 3);
	if (!v_buffer)return;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertecies_, sizeof(CUSTOMVERTEX) * 3);    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer	
	this->pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));//запись буффера в бэк
	this->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);	//рисование из бэка
	v_buffer->Release();
}
void renderer::draw_filled_rectangle(const vec2& pos, const vec2& size, const Color& color)
{
	float x =pos.x;
	float size_x = size.x;
	float y = pos.y;
	float size_y = size.y;
	const DWORD c = color.to_dword();
	CUSTOMVERTEX vertecies_[6] = {
		{x ,y + size_y,0.f,1.f, c},
		{x ,y,0.f,1.f, c},
		{x + size_x ,y,0.f,1.f,c},//left triangle
		{x + size_x ,y,0.f,1.f, c},//right triangle
		{x + size_x ,y + size_y,0.f,1.f, c},
		{x ,y + size_y,0.f,1.f, c},
	};
	VOID* pVoid;    // the void pointer
	IDirect3DVertexBuffer9* v_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 6);
	if (!v_buffer)return;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertecies_, sizeof(CUSTOMVERTEX) * 6);    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer	
	this->pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));//запись буффера в бэк
	this->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);	//рисование из бэка
	v_buffer->Release();
}
void renderer::draw_filled_rectangle(const vec2& pos, const vec2& size, DWORD color)
{
	float x = pos.x;
	float size_x = size.x;
	float y = pos.y;
	float size_y = size.y;
	const DWORD c = color;
	CUSTOMVERTEX vertecies_[6] = {
		{x ,y + size_y,0.f,1.f, c},
		{x ,y,0.f,1.f, c},
		{x + size_x ,y,0.f,1.f,c},//left triangle
		{x + size_x ,y,0.f,1.f, c},//right triangle
		{x + size_x ,y + size_y,0.f,1.f, c},
		{x ,y + size_y,0.f,1.f, c},
	};
	VOID* pVoid;    // the void pointer
	IDirect3DVertexBuffer9* v_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 6);
	if (!v_buffer)return;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertecies_, sizeof(CUSTOMVERTEX) * 6);    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer	
	this->pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));//запись буффера в бэк
	this->pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);	//рисование из бэка
	v_buffer->Release();
}
void renderer::draw_rectangle(const vec2& pos, const vec2& size, const Color& color)
{
	float x = pos.x;
	float size_x = size.x;
	float y = pos.y;
	float size_y = size.y;
	const DWORD c = color.to_dword();
	CUSTOMVERTEX vertecies_[5] = {
		{x ,y,0.f,1.f, c},
		{x +size_x,y,0.f,1.f, c},
		{x + size_x ,y+size_y,0.f,1.f,c},
		{x  ,y + size_y,0.f,1.f, c},
		{x  ,y,0.f,1.f, c},		
	};
	VOID* pVoid;    // the void pointer
	IDirect3DVertexBuffer9* v_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 6);
	if (!v_buffer)return;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertecies_, sizeof(CUSTOMVERTEX) * 5);    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer	
	this->pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));//запись буффера в бэк
	this->pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);	//рисование из бэка
	v_buffer->Release();

	/*draw_filled_rectangle(Vector2D(x, y - 1), Vector2D(size_x, 1.f), color);
	draw_filled_rectangle(Vector2D(x, y + size_y - 1), Vector2D(size_x, 1.f), color);
	draw_filled_rectangle(Vector2D(x, y - 1), Vector2D(1.f, size_y - 1), color);
	draw_filled_rectangle(Vector2D(x + size_x, y - 1), Vector2D(1.f, size_y - 1), color);*/
}
void renderer::draw_line(const vec2& pos, const vec2& end, const Color& color)
{
	const DWORD col = color.to_dword();
	CUSTOMVERTEX vertecies_[2] = {
		{pos.x ,pos.y ,0.f,1.f, col},
		{end.x ,end.y,0.f,1.f, col},
	};
	VOID* pVoid;    // the void pointer
	IDirect3DVertexBuffer9* v_buffer = create_vertex_buffer(sizeof(CUSTOMVERTEX) * 2);
	if (!v_buffer)return;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertecies_, sizeof(CUSTOMVERTEX) * 2);    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer	
	this->pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));//запись буффера в бэк
	this->pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);	//рисование из бэка
	v_buffer->Release();
}
void renderer::draw_string(const vec2& pos, const std::string& message, const Color& color, bool outlined , bool centered )
{
	if (this->game_font)
	{
		if (outlined)
		{
			float x = pos.x;
			float y = pos.y;
			draw_string(vec2(pos.x - 1, pos.y + 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y - 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y + 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y - 1), message, Color::black(), false, centered);
			draw_string(pos, message, color, false, centered);
		}
		else {

			RECT rc = { pos.x, pos.y,0,0 };
			this->game_font->DrawTextA(NULL, message.c_str(), message.length(), &rc, DT_NOCLIP | (centered ? DT_CENTER : DT_LEFT), color.to_dword());
		}
	}
	else {
		//assert
		MessageBox(NULL, "FONT IS NULLPTR", "Error", MB_OK);
	}
}
void renderer::draw_string(const vec2& pos, const vec2& size, std::string message, const Color& color, bool outlined , bool centered )
{
	if (this->game_font)
	{
		if (outlined)
		{
			float x = pos.x;
			float y = pos.y;

			draw_string(vec2(pos.x - 1, pos.y + 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y - 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y+ 1), message, Color::black(), false, centered);
			draw_string(vec2(pos.x + 1, pos.y - 1), message, Color::black(), false, centered);
			draw_string(pos, size, message, color, false, centered);
		}
		else {
			int x = pos.x;
			int y = pos.y;
			vec2 calc_size = calculate_txt_size(message, color, outlined, centered);

			RECT rc = { x, y + size.y / 2 - calc_size.y / 2, x + size.x,y + size.y };
			this->game_font->DrawTextA(NULL, message.c_str(), message.length(), &rc, DT_NOCLIP | (centered ? DT_CENTER : DT_LEFT), color.to_dword());
		}
	}
	else {
		//assert
		MessageBox(NULL, "FONT IS NULLPTR", "Error", MB_OK);
	}
}
vec2 renderer::calculate_txt_size(const std::string& message, const Color& color, bool outlined , bool centered)
{
	RECT rc = { 0, 0, 0,0 };
	this->game_font->DrawTextA(NULL, message.c_str(), message.length(), &rc, DT_CALCRECT, color.to_dword());
	return vec2((int)(rc.right - rc.left), (int)(rc.bottom - rc.top));
}

