/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

// 参考Eigen的实现，使用Expression template

#include <concepts>
#include <utility>
#include <cmath>
#include <array>
#include <algorithm>
#include <numbers>
#include "Constants.hpp"
#include "Declarations.hpp"
#include "Reduction.hpp"
#include "Determinant.hpp"
#include "OrthoMethod.hpp"
#include "Assignment.hpp"
#include "MatrixBase.hpp"
#include "Matrix.hpp"
#include "CWiseBinaryOp.hpp"
#include "Transpose.hpp"
#include "Block.hpp"
#include "Product.hpp"
#include "CWiseNullaryOp.hpp"
#include "Inverse.hpp"
#include "Transform/RotationBase.hpp"
#include "Transform/Quaternion.hpp"
#include "Transform/AngleAxis.hpp"
#include "Transform/Transform.hpp"
#include "Internal/Evaluator/CoreEvaluators.hpp"
