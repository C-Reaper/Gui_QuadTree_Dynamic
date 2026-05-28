#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Container/DQuadTree.h"

typedef struct AObject {
	Vec2 v;
	Pixel c;
} AObject;

TransformedView tv;
DQuadTree tree;
const float Area = 100000.0f;

void DQuadTree_Item_UpdateDraw(DQuadTree_Item* e){
	AObject* obj = (AObject*)e->data;
	e->rect.p = Vec2_Add(e->rect.p,Vec2_Mulf(obj->v,window.ElapsedTime));

	if(e->rect.p.x < 0.0f){
		e->rect.p.x = 0.0f;
		obj->v.x *= -1.0f;
	}else if(e->rect.p.x >= Area){
		e->rect.p.x = Area;
		obj->v.x *= -1.0f;
	}

	if(e->rect.p.y < 0.0f){
		e->rect.p.y = 0.0f;
		obj->v.y *= -1.0f;
	}else if(e->rect.p.y >= Area){
		e->rect.p.y = Area;
		obj->v.y *= -1.0f;
	}

	const Vec2 p = TransformedView_WorldScreenPos(&tv,e->rect.p);
	const Vec2 d = TransformedView_WorldScreenLength(&tv,e->rect.d);
	RenderRect(p.x,p.y,d.x,d.y,obj->c);
}

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.01f,0.01f },
		(float)GetWidth() / (float)GetHeight()
	);
	TransformedView_Zoom(&tv,(Vec2){ 0.05f,0.05f });

	tree = DQuadTree_New((Rect){ { 0.0f,0.0f },{ Area,Area } },10);

	for(int i = 0;i<1000000;i++){
		const Rect area = {
			{ Random_f64_MinMax(0.0f,Area),Random_f64_MinMax(0.0f,Area) },
			{ Random_f64_MinMax(10.0f,100.0f),Random_f64_MinMax(10.0f,100.0f) }
		};

		AObject Obj = {
			//{ 0.0f,0.0f },
			{ Random_f64_MinMax(1.0f,100.0f),Random_f64_MinMax(1.0f,100.0f) },
			Pixel_toRGBA(Random_f64_New(),Random_f64_New(),Random_f64_New(),1.0f)
		};
		DQuadTree_Add(&tree,(AObject[]){ Obj },sizeof(AObject),area);
	}
}
void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	const Vec2 area = { 500.0f,500.0f };
	const Vec2 w_mouse = TransformedView_ScreenWorldPos(&tv,GetMouse());
	const Rect w_screen = TransformedView_ScreenWorldRect(&tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });
	
	if(Stroke(ALX_MOUSE_L).DOWN){
		const Rect brush = Rect_New(Vec2_Sub(w_mouse,Vec2_Mulf(area,0.5f)),area);
		Vector inside = DQuadTree_Search(&tree,brush);
		DQuadTree_Remove(&tree,&inside);
		Vector_Free(&inside);
	}

	Clear(BLACK);
	
	Timepoint start = Time_Nano();
	Vector visible = DQuadTree_Search(&tree,w_screen);
	const int ObjectCount = visible.size;
	
	for(int i = 0;i<visible.size;i++){
        DQuadTree_Item* qti = *(DQuadTree_Item**)Vector_Get(&visible,i);
		DQuadTree_Item_UpdateDraw(qti);
    }
	DQuadTree_Relocate(&tree,&visible);
	Vector_Free(&visible);

	const Vec2 mp = TransformedView_WorldScreenPos(&tv,Vec2_Sub(w_mouse,Vec2_Mulf(area,0.5f)));
	const Vec2 md = TransformedView_WorldScreenLength(&tv,area);
	RenderRectAlpha(mp.x,mp.y,md.x,md.y,0x55FFFFFF);

	const size_t tree_size = DQuadTree_Size(&tree);
	FDuration ElapsedTime = Time_Elapsed(start,Time_Nano());
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,WHITE,"O: %f,%f | Z: %f,%f",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,GetAlxFont()->CharSizeY,WHITE,"%d/%d in %f",ObjectCount,tree_size,ElapsedTime);
}
void Delete(AlxWindow* w){
	DQuadTree_Free(&tree);
}

int main(){
    if(Create("Quad Tree (Dynamic)",2500,1300,1,1,Setup,Update,Delete))
        Start();
    return 0;
}