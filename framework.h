#pragma once

/* Standard C/C++ libraries / 标准C/C++库 */
#include <cstdint>

/* Windows libraries / Windows库 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* L-Edit libraries / L-Edit库 */
#define EXCLUDE_LEDIT_LEGACY_UPI
#include "ldata.h"
#pragma comment(lib, "upilink64.lib")

/* Linear algebra libraries & definitions / 线性代数库 & 定义 */
#include <Eigen/Dense>
typedef int64_t linalg_int;
typedef float linalg_real;
#define linalg_PI ((linalg_real)M_PI)
typedef Eigen::Matrix<linalg_int, 2, 1> vec2_int;
typedef Eigen::Matrix<linalg_real, 2, 1> vec2_real;
typedef Eigen::Matrix<linalg_int, 2, 2> mat2x2_int;
typedef Eigen::Matrix<linalg_real, 2, 2> mat2x2_real;
typedef Eigen::Matrix<linalg_int, Eigen::Dynamic, 1> vec_int;
typedef Eigen::Matrix<linalg_real, Eigen::Dynamic, 1> vec_real;
typedef Eigen::Matrix<linalg_int, Eigen::Dynamic, Eigen::Dynamic> mat_int;
typedef Eigen::Matrix<linalg_real, Eigen::Dynamic, Eigen::Dynamic> mat_real;
