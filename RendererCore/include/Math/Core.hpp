/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once
#define MATH_CORE_H

#include <type_traits>
#include "Constants.hpp"
#include "ForwardDeclarations.hpp"
#include "Simd.hpp"
#include "XprHelper.hpp"
#include "BinaryFunctors.hpp"
#include "AssignmentFunctors.hpp"
#include "CwiseBinaryOp.hpp"
#include "CoreEvaluator.hpp"
#include "AssignEvaluator.hpp"
#include "Base.hpp"
#include "CoeffsBase.hpp"
#include "Storage.hpp"
#include "MatrixBase.hpp"
#include "Matrix.hpp"
#include "Transpose.hpp"
#include "Block.hpp"
#include "Dot.hpp"
#include "Redux.hpp"