#include "pch.h"

EXTERN_DLL_EXPORT int UPI_Entry_Point(void) {
	LMacro_BindToMenuAndHotKey_v9_30("Draw", "Ctrl+B", "Macros\\Ultimate Waveguide Bend / 波导弯曲绘制", "Macro_UltimateWaveguideBend", NULL);
	return 1;
}

EXTERN_DLL_EXPORT void Macro_UltimateWaveguideBend(void) {
	/* ----- Initialization / 初始化  ----- */
	// Get active layer / 获取活动图层
	LCell pCell = LCell_GetVisible();
	if (!pCell) {
		LDialog_AlertBox("No cell is active! \n无活动单元！");
		return;
	}
	LFile pFile = LCell_GetFile(pCell);
	if (!pFile) {
		LDialog_AlertBox("No file is active! \n无活动文件！");
		return;
	}
	LLayer pLayer = LLayer_GetCurrent(pFile);
	if (!pLayer) {
		LDialog_AlertBox("No layer is active! \n无活动层！");
		return;
	}

	/* ----- Read cursor position / 读取坐标 ----- */
	LPoint ptWire1Aux = LCursor_GetPositionEx99(1, 1, "Choose a point on the 1st wire (except the connecting point)\n在第1线段上选取一点（非连接端点）");
	LPoint ptWire1End = LCursor_GetPositionEx99(1, 1, "Click on the connecting point of the 1st wire\n选取第1线段连接端点");
	LPoint ptWire2End = LCursor_GetPositionEx99(1, 1, "Click on the connecting point of the 2st wire\n选取第2线段连接端点");
	LPoint ptWire2Aux = LCursor_GetPositionEx99(1, 1, "Choose a point on the 2st wire (except the connecting point)\n在第2线段上选取一点（非连接端点）");

	char precStr[11] = "100";
	LDialog_InputBox("Precision 精度", "Num of points to be inserted 总插值点数: ", precStr);
	int prec = atoi(precStr);
	if (prec < 0) {
		LDialog_AlertBox("The number of points must be greater or equal to 0! \n插值点数必须大于等于0！");
		return;
	}

	/* ----- Compute and draw shapes / 计算与绘制图形  ----- */
	// Compute shapes / 计算图形 
	uwb_pts_t pts = UWB_Calc(ptWire1Aux, ptWire1End, ptWire2Aux, ptWire2End, prec);
	// Get cell ownership / 获取单元所有权以进行修改
	if (LCell_GetLock(pCell) != 0) {
		LDialog_AlertBox("The cell is locked! \n单元已锁定！");
		return;
	}
	// Draw shapes / 绘制图形
	LWire_New(pCell, pLayer, NULL, 0, pts.pts, pts.n);
	delete[] pts.pts;
	// Update graphics / 更新显示
	LDisplay_Refresh();
}

uwb_pts_t UWB_Calc(LPoint ptWire1Aux, LPoint ptWire1End, LPoint ptWire2Aux, LPoint ptWire2End, int prec) {
	// Convert to vectors / 转换为向量
	vec2_int vecWire1Aux, vecWire1End, vecWire2Aux, vecWire2End;
	vecWire1Aux << ptWire1Aux.x, ptWire1Aux.y;
	vecWire1End << ptWire1End.x, ptWire1End.y;
	vecWire2Aux << ptWire2Aux.x, ptWire2Aux.y;
	vecWire2End << ptWire2End.x, ptWire2End.y;
	// Check input values / 输入数值检查
	if (vecWire1Aux == vecWire1End || vecWire2End == vecWire2Aux) {
		LDialog_AlertBox("The auxilary point cannot be the same as the connecting point! \n端点与辅助点重合！");
		return uwb_pts_t(0);
	}
	if (vecWire1End == vecWire2End) return uwb_pts_t(0);
	// Construct lines / 构建直线
	mat2x2_int matLinesCoefs;
	matLinesCoefs <<
		vecWire1End(1) - vecWire1Aux(1), vecWire1Aux(0) - vecWire1End(0),
		vecWire2End(1) - vecWire2Aux(1), vecWire2Aux(0) - vecWire2End(0);
	vec2_int vecLinesConsts;
	vecLinesConsts <<
		vecWire1Aux(0) * vecWire1End(1) - vecWire1End(0) * vecWire1Aux(1),
		vecWire2Aux(0)* vecWire2End(1) - vecWire2End(0) * vecWire2Aux(1);
	// Check if parallel / 检查是否平行
	if (matLinesCoefs.determinant() == 0) {
		// Normalization / 统一数值大小
		if (matLinesCoefs(0, 0) != 0) {
			// by 1st column / 按第1列统一
			bool sameSign = matLinesCoefs(0, 0) * matLinesCoefs(1, 0) > 0;
			vecLinesConsts(1) = sameSign ? vecLinesConsts(1) : -vecLinesConsts(1);
			matLinesCoefs.row(1) = sameSign ? matLinesCoefs.row(1) : -matLinesCoefs.row(1);
			linalg_int gcd = UWB_GCD(matLinesCoefs(0, 0), matLinesCoefs(1, 0));
			linalg_int lcm = matLinesCoefs(0, 0) * matLinesCoefs(1, 0) / gcd;
			vecLinesConsts(0) *= lcm / matLinesCoefs(0, 0);
			vecLinesConsts(1) *= lcm / matLinesCoefs(1, 0);
			matLinesCoefs.row(0) *= lcm / matLinesCoefs(0, 0);
			matLinesCoefs.row(1) *= lcm / matLinesCoefs(1, 0);
		}
		else {
			// by 2nd column / 按第2列统一
			bool sameSign = matLinesCoefs(0, 1) * matLinesCoefs(1, 1) > 0;
			vecLinesConsts(1) = sameSign ? vecLinesConsts(1) : -vecLinesConsts(1);
			matLinesCoefs.row(1) = sameSign ? matLinesCoefs.row(1) : -matLinesCoefs.row(1);
			linalg_int gcd = UWB_GCD(matLinesCoefs(0, 1), matLinesCoefs(1, 1));
			linalg_int lcm = matLinesCoefs(0, 1) * matLinesCoefs(1, 1) / gcd;
			vecLinesConsts(0) *= lcm / matLinesCoefs(0, 1);
			vecLinesConsts(1) *= lcm / matLinesCoefs(1, 1);
			matLinesCoefs.row(0) *= lcm / matLinesCoefs(0, 1);
			matLinesCoefs.row(1) *= lcm / matLinesCoefs(1, 1);
		}
		// Check if two lines overlap / 检查是否重合
		if (vecLinesConsts(0) == vecLinesConsts(1)) {
			uwb_pts_t pts(2);
			if (pts.pts == NULL) {
				LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
				return uwb_pts_t(0);
			}
			pts.pts[0] = ptWire1End;
			pts.pts[1] = ptWire2End;
			return pts;
		}
		vec2_int vecProj2on1 = (vecWire1Aux - vecWire1End) * 
			(vecWire1Aux - vecWire1End).dot(vecWire2End - vecWire1End)
			/ (vecWire1Aux - vecWire1End).dot(vecWire1Aux - vecWire1End);
		// Check directions / 判断是否是同向
		if ((vecWire1Aux - vecWire1End).dot(vecWire2Aux - vecWire2End) < 0) {
			// Opposite directions / 异向，分成两段处理
			int newPrec = prec / 2;
			vec2_int vecMid = (vecWire1End + vecWire2End) / 2;
			// Check if vertically overlap / 判断是否纵向有重合
			if (vecProj2on1.dot(vecWire1Aux - vecWire1End) < 0) {
				// No overlap / 无重合
				vec2_int vecDir = (vecWire1Aux - vecWire2Aux) - (vecWire2End - vecWire1End) * 
					(vecWire1Aux - vecWire2Aux).dot(vecWire2End - vecWire1End)
					/ (vecWire2End - vecWire1End).dot(vecWire2End - vecWire1End);
				LPoint ptAuxWireEnd, ptAuxWire1Aux, ptAuxWire2Aux;
				ptAuxWireEnd.x = (LCoord)vecMid(0);
				ptAuxWireEnd.y = (LCoord)vecMid(1);
				ptAuxWire1Aux.x = (LCoord)(vecMid(0) + vecDir(0));
				ptAuxWire1Aux.y = (LCoord)(vecMid(1) + vecDir(1));
				ptAuxWire2Aux.x = (LCoord)(vecMid(0) - vecDir(0));
				ptAuxWire2Aux.y = (LCoord)(vecMid(1) - vecDir(1));
				uwb_pts_t pts1 = UWB_Calc(ptWire1Aux, ptWire1End, ptAuxWire1Aux, ptAuxWireEnd, newPrec);
				uwb_pts_t pts2 = UWB_Calc(ptWire2Aux, ptWire2End, ptAuxWire2Aux, ptAuxWireEnd, newPrec);
				// If one is empty / 若有一方为空
				if (pts2.n == 0) {
					delete[] pts2.pts;
					return pts1;
				}
				if (pts1.n == 0) {
					for (int i = 0; i < pts2.n / 2; i++) {
						LPoint pt = pts2.pts[i];
						pts2.pts[i] = pts2.pts[pts2.n - 1 - i];
						pts2.pts[pts2.n - 1 - i] = pt;
					}
					delete[] pts1.pts;
					return pts2;
				}
				// Merge / 合并
				uwb_pts_t pts(pts1.n + pts2.n - 1);
				if (pts.pts == NULL) {
					LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
					delete[] pts1.pts;
					delete[] pts2.pts;
					return uwb_pts_t(0);
				}
				for (int i = 0; i < pts1.n; i++) {
					pts.pts[i] = pts1.pts[i];
				}
				for (int i = 0; i < pts2.n - 1; i++) {
					pts.pts[pts1.n + pts2.n - 2 - i] = pts2.pts[i];
				}
				delete[] pts1.pts;
				delete[] pts2.pts;
				return pts;
			}
			// Overlap / 有重合
			LPoint ptAuxWire1End, ptAuxWire2End;
			ptAuxWire1End.x = (LCoord)(vecMid(0) - vecProj2on1(0) / 2);
			ptAuxWire1End.y = (LCoord)(vecMid(1) - vecProj2on1(1) / 2);
			ptAuxWire2End.x = (LCoord)(vecMid(0) + vecProj2on1(0) / 2);
			ptAuxWire2End.y = (LCoord)(vecMid(1) + vecProj2on1(1) / 2);
			uwb_pts_t pts1 = UWB_Calc(ptWire1Aux, ptWire1End, ptAuxWire2End, ptAuxWire1End, newPrec);
			uwb_pts_t pts2 = UWB_Calc(ptWire2Aux, ptWire2End, ptAuxWire1End, ptAuxWire2End, newPrec);
			uwb_pts_t pts(pts1.n + pts2.n);
			if (pts.pts == NULL) {
				LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
				delete[] pts1.pts;
				delete[] pts2.pts;
				return uwb_pts_t(0);
			}
			for (int i = 0; i < pts1.n; i++) {
				pts.pts[i] = pts1.pts[i];
			}
			for (int i = 0; i < pts2.n; i++) {
				pts.pts[pts1.n + pts2.n - 1 - i] = pts2.pts[i];
			}
			delete[] pts1.pts;
			delete[] pts2.pts;
			return pts;
		}
		else {
			// Same directions, check alignment / 同向，判断两边是否对齐
			linalg_int align = vecProj2on1.dot(vecWire1Aux - vecWire1End);
			if (align > 0) {
				// Wire2 too short / 第2线段过短
				LPoint ptExtWireEnd;
				ptExtWireEnd.x = (LCoord)(vecWire2End(0) - vecProj2on1(0));
				ptExtWireEnd.y = (LCoord)(vecWire2End(1) - vecProj2on1(1));
				uwb_pts_t ptsBend = UWB_Bend(ptWire1Aux, ptWire1End, ptExtWireEnd, ptWire2End, prec);
				uwb_pts_t pts(ptsBend.n + 1);
				if (pts.pts == NULL) {
					LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
					delete[] ptsBend.pts;
					return uwb_pts_t(0);
				}
				for (int i = 0; i < ptsBend.n; i++) {
					pts.pts[i] = ptsBend.pts[i];
				}
				pts.pts[ptsBend.n] = ptWire2End;
				delete[] ptsBend.pts;
				return pts;
			}
			else if (align < 0) {
				// Wire1 too short / 第2线段过短
				LPoint ptExtWireEnd;
				ptExtWireEnd.x = (LCoord)(vecWire1End(0) + vecProj2on1(0));
				ptExtWireEnd.y = (LCoord)(vecWire1End(1) + vecProj2on1(1));
				uwb_pts_t ptsBend = UWB_Bend(ptWire2Aux, ptWire2End, ptExtWireEnd, ptWire1End, prec);
				uwb_pts_t pts(ptsBend.n + 1);
				if (pts.pts == NULL) {
					LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
					delete[] ptsBend.pts;
					return uwb_pts_t(0);
				}
				for (int i = 0; i < ptsBend.n; i++) {
					pts.pts[i] = ptsBend.pts[i];
				}
				pts.pts[ptsBend.n] = ptWire1End;
				delete[] ptsBend.pts;
				return pts;
			}
			return UWB_Bend(ptWire1Aux, ptWire1End, ptWire2End, ptWire2Aux, prec);
		}
	}
	// Compute intersect / 计算交点
	mat2x2_int matCramerX, matCramerY;
	matCramerX << vecLinesConsts, matLinesCoefs.col(1);
	matCramerY << matLinesCoefs.col(0), vecLinesConsts;
	vec2_int vecIntersect;
	vecIntersect << matCramerX.determinant() / matLinesCoefs.determinant(), matCramerY.determinant() / matLinesCoefs.determinant();
	// Check if the intersect is on the lines / 判断交点是否在线段上
	bool intersectLine1 = (vecIntersect - vecWire1End).dot(vecWire1Aux - vecWire1End) >= 0;
	bool intersectLine2 = (vecIntersect - vecWire2End).dot(vecWire2Aux - vecWire2End) >= 0;
	if (intersectLine1 || intersectLine2) {
		// On the lines / 交点在线段上
		if (intersectLine1 && intersectLine2) {
			// On both lines / 两线段均有交点
			linalg_int dist1 = (linalg_int)sqrt((linalg_real)(vecIntersect - vecWire1End).squaredNorm());
			linalg_int dist2 = (linalg_int)sqrt((linalg_real)(vecIntersect - vecWire2End).squaredNorm());
			if (dist1 < dist2) {
				LPoint ptExtWireEnd;
				vec2_int vecExtDir = vecWire1End - vecIntersect;
				linalg_int dist = (linalg_int)sqrt((linalg_real)vecExtDir.squaredNorm());
				vec2_int vecExt = vecExtDir * (dist2 - dist1) / max(dist, 1);
				ptExtWireEnd.x = (LCoord)(vecWire1End(0) + vecExt(0));
				ptExtWireEnd.y = (LCoord)(vecWire1End(1) + vecExt(1));
				uwb_pts_t ptsBend = UWB_Bend(ptWire2Aux, ptWire2End, ptExtWireEnd, ptWire1End, prec);
				uwb_pts_t pts(ptsBend.n + 1);
				if (pts.pts == NULL) {
					LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
					delete[] ptsBend.pts;
					return uwb_pts_t(0);
				}
				for (int i = 0; i < ptsBend.n; i++) {
					pts.pts[ptsBend.n - i] = ptsBend.pts[i];
				}
				pts.pts[0] = ptWire1End;
				delete[] ptsBend.pts;
				return pts;
			}
			else if (dist1 > dist2) {
				LPoint ptExtWireEnd;
				vec2_int vecExtDir = vecWire2End - vecIntersect;
				linalg_int dist = (linalg_int)sqrt((linalg_real)vecExtDir.squaredNorm());
				vec2_int vecExt = vecExtDir * (dist1 - dist2) / max(dist, 1);
				ptExtWireEnd.x = (LCoord)(vecWire2End(0) + vecExt(0));
				ptExtWireEnd.y = (LCoord)(vecWire2End(1) + vecExt(1));
				uwb_pts_t ptsBend = UWB_Bend(ptWire1Aux, ptWire1End, ptExtWireEnd, ptWire2End, prec);
				uwb_pts_t pts(ptsBend.n + 1);
				if (pts.pts == NULL) {
					LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
					delete[] ptsBend.pts;
					return uwb_pts_t(0);
				}
				for (int i = 0; i < ptsBend.n; i++) {
					pts.pts[i] = ptsBend.pts[i];
				}
				pts.pts[ptsBend.n] = ptWire2End;
				delete[] ptsBend.pts;
				return pts;
			}
			return UWB_Bend(ptWire1Aux, ptWire1End, ptWire2End, ptWire2Aux, prec);
		}
		// On one line / 一线段有交点
		int newPrec = prec / 2;
		vec2_int vecMid, vecDir;
		if (intersectLine1) {
			vec2_int vecProj2on1 = (vecWire1Aux - vecWire1End) *
				(vecWire1Aux - vecWire1End).dot(vecWire2End - vecWire1End)
				/ (vecWire1Aux - vecWire1End).dot(vecWire1Aux - vecWire1End);
			vecMid = vecWire1End + (vecWire2End - vecWire1End - vecProj2on1) / 2;
			vecDir = vecWire1Aux - vecWire1End;
		}
		else {
			vec2_int vecProj1on2 = (vecWire2Aux - vecWire2End) *
				(vecWire2Aux - vecWire2End).dot(vecWire1End - vecWire2End)
				/ (vecWire2Aux - vecWire2End).dot(vecWire2Aux - vecWire2End);
			vecMid = vecWire2End + (vecWire1End - vecWire2End - vecProj1on2) / 2;
			vecDir = vecWire2Aux - vecWire2End;
		}
		LPoint ptAuxWireEnd, ptAuxWire1Aux, ptAuxWire2Aux;
		ptAuxWireEnd.x = (LCoord)vecMid(0);
		ptAuxWireEnd.y = (LCoord)vecMid(1);
		ptAuxWire1Aux.x = (LCoord)(vecMid(0) + vecDir(0));
		ptAuxWire1Aux.y = (LCoord)(vecMid(1) + vecDir(1));
		ptAuxWire2Aux.x = (LCoord)(vecMid(0) - vecDir(0));
		ptAuxWire2Aux.y = (LCoord)(vecMid(1) - vecDir(1));
		uwb_pts_t pts1 = UWB_Calc(ptWire1Aux, ptWire1End, ptAuxWire1Aux, ptAuxWireEnd, newPrec);
		uwb_pts_t pts2 = UWB_Calc(ptWire2Aux, ptWire2End, ptAuxWire2Aux, ptAuxWireEnd, newPrec);
		// If one is empty / 若有一方为空
		if (pts2.n == 0) {
			delete[] pts2.pts;
			return pts1;
		}
		if (pts1.n == 0) {
			for (int i = 0; i < pts2.n / 2; i++) {
				LPoint pt = pts2.pts[i];
				pts2.pts[i] = pts2.pts[pts2.n - 1 - i];
				pts2.pts[pts2.n - 1 - i] = pt;
			}
			delete[] pts1.pts;
			return pts2;
		}
		// Merge / 合并
		uwb_pts_t pts(pts1.n + pts2.n - 1);
		if (pts.pts == NULL) {
			LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
			delete[] pts1.pts;
			delete[] pts2.pts;
			return uwb_pts_t(0);
		}
		for (int i = 0; i < pts1.n; i++) {
			pts.pts[i] = pts1.pts[i];
		}
		for (int i = 0; i < pts2.n - 1; i++) {
			pts.pts[pts1.n + pts2.n - 2 - i] = pts2.pts[i];
		}
		delete[] pts1.pts;
		delete[] pts2.pts;
		return pts;
	}
	// Not on the lines / 交点不在线段上
	linalg_int dist1 = (linalg_int)sqrt((linalg_real)(vecIntersect - vecWire1End).squaredNorm());
	linalg_int dist2 = (linalg_int)sqrt((linalg_real)(vecIntersect - vecWire2End).squaredNorm());
	if (dist1 > dist2) {
		LPoint ptExtWireEnd;
		vec2_int vecExtDir = vecIntersect - vecWire1End;
		linalg_int dist = (linalg_int)sqrt((linalg_real)vecExtDir.squaredNorm());
		vec2_int vecExt = vecExtDir * (dist1 - dist2) / max(dist, 1);
		ptExtWireEnd.x = (LCoord)(vecWire1End(0) + vecExt(0));
		ptExtWireEnd.y = (LCoord)(vecWire1End(1) + vecExt(1));
		uwb_pts_t ptsBend = UWB_Bend(ptWire2Aux, ptWire2End, ptExtWireEnd, ptWire1End, prec);
		uwb_pts_t pts(ptsBend.n + 1);
		if (pts.pts == NULL) {
			LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
			delete[] ptsBend.pts;
			return uwb_pts_t(0);
		}
		for (int i = 0; i < ptsBend.n; i++) {
			pts.pts[ptsBend.n - i] = ptsBend.pts[i];
		}
		pts.pts[0] = ptWire1End;
		delete[] ptsBend.pts;
		return pts;
	}
	else if (dist1 < dist2) {
		LPoint ptExtWireEnd;
		vec2_int vecExtDir = vecIntersect - vecWire2End;
		linalg_int dist = (linalg_int)sqrt((linalg_real)vecExtDir.squaredNorm());
		vec2_int vecExt = vecExtDir * (dist2 - dist1) / max(dist, 1);
		ptExtWireEnd.x = (LCoord)(vecWire2End(0) + vecExt(0));
		ptExtWireEnd.y = (LCoord)(vecWire2End(1) + vecExt(1));
		uwb_pts_t ptsBend = UWB_Bend(ptWire1Aux, ptWire1End, ptExtWireEnd, ptWire2End, prec);
		uwb_pts_t pts(ptsBend.n + 1);
		if (pts.pts == NULL) {
			LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
			delete[] ptsBend.pts;
			return uwb_pts_t(0);
		}
		for (int i = 0; i < ptsBend.n; i++) {
			pts.pts[i] = ptsBend.pts[i];
		}
		pts.pts[ptsBend.n] = ptWire2End;
		delete[] ptsBend.pts;
		return pts;
	}
	return UWB_Bend(ptWire1Aux, ptWire1End, ptWire2End, ptWire2Aux, prec);
}

template <typename T> T UWB_GCD(T a, T b) {
	while (b != 0) {
		T t = b;
		b = a % b;
		a = t;
	}
	return a;
}

uwb_pts_t UWB_Bend(LPoint ptWireAux, LPoint ptWireEnd, LPoint ptEnd, LPoint ptEndAux, int prec) {
	// Convert to vectors / 转换为向量
	vec2_int vecWireAux, vecWireEnd, vecEnd, vecEndAux;
	vecWireAux << ptWireAux.x, ptWireAux.y;
	vecWireEnd << ptWireEnd.x, ptWireEnd.y;
	vecEnd << ptEnd.x, ptEnd.y;
	vecEndAux << ptEndAux.x, ptEndAux.y;
	// Convert to floating-point numbers / 浮点化
	vec2_real vecWireRel = (vecWireAux - vecWireEnd).cast<linalg_real>();
	vec2_real vecEndRel = (vecEnd - vecWireEnd).cast<linalg_real>();
	vec2_real vecEndAuxRel = (vecEndAux - vecWireEnd).cast<linalg_real>();
	vecWireRel.normalize();
	vecEndAuxRel /= vecEndRel.norm();
	vecEndRel.normalize();
	// Change of basis / 基变更
	vec2_real vecBasisX = -vecWireRel;
	mat2x2_real matBasis;
	matBasis <<
		vecBasisX(0), -vecBasisX(1),
		vecBasisX(1), vecBasisX(0);
	mat2x2_real matBasisInv = matBasis.inverse();
	vec2_real vecEndBasis = matBasisInv * vecEndRel;
	vec2_real vecEndAuxBasis = matBasisInv * vecEndAuxRel;
	// Target angle / 目标角度
	linalg_real sign = (linalg_real)((vecEndBasis(0) * vecEndAuxBasis(1) - vecEndBasis(1) * vecEndAuxBasis(0)) >= 0 ? 1 : -1);
	linalg_real theta = acos(max(min(((vecEndAuxBasis - vecEndBasis) / (vecEndAuxBasis - vecEndBasis).norm())(0), 1), -1));
	theta = (vecEndAuxBasis - vecEndBasis)(1) * sign >= 0 ? theta : 2 * linalg_PI - theta;
	if (theta > linalg_PI + linalg_PI / 4) {
		if (!LDialog_YesNoBox("The bend angle is too large! \n弯曲角度过大！\nDo you want to continue? \n是否继续？")) {
			return uwb_pts_t(0);
		}
	}
	// Compute / 计算
	linalg_real delta = theta / (prec + 1);
	linalg_real alpha = theta / 2;
	int alpha_n = (int)(alpha / delta);
	mat_real matAlpha(2, alpha_n + 1);
	matAlpha(0, 0) = 0;
	matAlpha(1, 0) = 0;
	for (int i = 1; i <= alpha_n; i++) {
		linalg_real x = delta * i;
		matAlpha(0, i) = pow(x, (linalg_real)1 / 2) - pow(x, (linalg_real)5 / 2) / 10 + pow(x, (linalg_real)9 / 2) / 216;
		matAlpha(1, i) = pow(x, (linalg_real)3 / 2) / 3 - pow(x, (linalg_real)7 / 2) / 42 + pow(x, (linalg_real)11 / 2) / 1320;
		matAlpha(1, i) *= sign;
	}
	mat_real matAlpha2 = matAlpha;
	matAlpha2.row(1) *= -1;
	matAlpha2.colwise() -= matAlpha2.col(alpha_n);
	mat2x2_real matAlpha2Rot;
	matAlpha2Rot <<
		cos((2 * alpha - linalg_PI) * sign), -sin((2 * alpha - linalg_PI) * sign),
		sin((2 * alpha - linalg_PI) * sign), cos((2 * alpha - linalg_PI) * sign);
	matAlpha2 = matAlpha2Rot * matAlpha2;
	matAlpha2.colwise() += matAlpha.col(alpha_n);
	mat_real matPtsRel(2, 2 * alpha_n + 1);
	matPtsRel << matAlpha, matAlpha2.block(0, 0, 2, alpha_n).rowwise().reverse();
	matPtsRel /= matPtsRel.col(matPtsRel.cols() - 1).norm();
	// Change back to standard basis / 转换回原坐标系
	matPtsRel = matBasis * matPtsRel;
	matPtsRel *= (vecEnd - vecWireEnd).cast<linalg_real>().norm();
	mat_int matPts = matPtsRel.cast<linalg_int>();
	matPts.colwise() += vecWireEnd.cast<linalg_int>();
	// Convert to points / 转换为点
	uwb_pts_t pts(2 + max((int)matPts.cols() - 2, 0));
	if (pts.pts == NULL) {
		LDialog_AlertBox("Memory allocation failed! \n内存分配失败！");
		return uwb_pts_t(0);
	}
	pts.pts[0] = ptWireEnd;
	pts.pts[pts.n - 1] = ptEnd;
	for (int i = 1; i < pts.n - 1; i++) {
		pts.pts[i].x = (LCoord)matPts(0, i);
		pts.pts[i].y = (LCoord)matPts(1, i);
	}
	return pts;
}
