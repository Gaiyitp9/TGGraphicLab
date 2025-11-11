/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

// 参考了Eigen的实现，使用Expression template

#include <concepts>
#include <utility>
#include <cmath>
#include <array>
#include <algorithm>
#include <numbers>
#include "Core/Constants.hpp"
#include "Core/Declarations.hpp"
#include "Core/Reduction.hpp"
#include "Core/Determinant.hpp"
#include "Core/OrthoMethod.hpp"
#include "Core/Assignment.hpp"
#include "Core/MatrixBase.hpp"
#include "Core/Matrix.hpp"
#include "Core/CWiseBinaryOp.hpp"
#include "Core/Transpose.hpp"
#include "Core/Block.hpp"
#include "Core/Product.hpp"
#include "Core/CWiseNullaryOp.hpp"
#include "Core/CWiseUnaryOp.hpp"
#include "Core/Inverse.hpp"
#include "Core/Evaluator/CoreEvaluators.hpp"
#include "Core/Random.hpp"
