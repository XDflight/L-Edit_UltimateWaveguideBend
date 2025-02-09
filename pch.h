#ifndef PCH_H
#define PCH_H

#include "framework.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

/*
 * UPI_Entry_Point : void -> int
 * L-Edit UPI entry point. / L-Edit UPI 入口点。
 */
EXTERN_DLL_EXPORT int UPI_Entry_Point(void);

/*
 * Macro_UltimateWaveguideBend : void -> void
 * Macro entry point. / 插件宏入口点。
 */
EXTERN_DLL_EXPORT void Macro_UltimateWaveguideBend(void);

/*
 * struct uwb_pts
 * UWB internal point set / UWB内部使用的点集类
 */
struct uwb_pts {
	int n;
	LPoint* pts;
	uwb_pts(int n) {
		if (n < 0) {
			n = 0;
		}
		this->n = n;
		pts = new LPoint[n];
	}
};
typedef struct uwb_pts uwb_pts_t;

/*
 * Macro_UltimateWaveguideBend : LPoint * LPoint * LPoint * LPoint * int -> uwb_pts_t
 * UWB main calculation function / UWB计算主函数
 * - ptWire1Aux: Auxilary point for wire 1 / 第1线段辅助点
 * - ptWire1End: Wire 1 connecting point / 第1线段连接端点
 * - ptWire2Aux: Auxilary point for wire 2 / 第2线段辅助点
 * - ptWire2End: Wire 2 connecting point / 第2线段连接端点
 * - prec: Precision (num of points) / 精度（点数）
 * Return: UWB point set / UWB点集
 */
uwb_pts_t UWB_Calc(LPoint ptWire1Aux, LPoint ptWire1End, LPoint ptWire2Aux, LPoint ptWire2End, int prec);

/*
 * Macro_UltimateWaveguideBend : T * T -> T
 * UWB GCD function / UWB内部使用最大公因数函数
 * - A: Input 1 / 输入1
 * - B: Input 2 / 输入2
 * Return: GCD(A, B)
 */
template <typename T> T UWB_GCD(T a, T b);

/*
 * UWB_Bend : LPoint * LPoint * LPoint * LPoint * int -> uwb_pts_t
 * UWB bend calculation function / UWB弯曲计算函数
 * - ptWireAux: Auxilary point for base wire / 基线段辅助点
 * - ptWireEnd: Base wire connecting point / 基线段连接端点
 * - ptEnd: Target connecting point / 目标连接端点
 * - ptEndAux: Auxilary point for target angle / 目标角度辅助点
 * Return: UWB point set / UWB点集
 */
uwb_pts_t UWB_Bend(LPoint ptWireAux, LPoint ptWireEnd, LPoint ptEnd, LPoint ptEndAux, int prec);

#endif
