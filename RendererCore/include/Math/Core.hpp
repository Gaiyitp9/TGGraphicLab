/************************************************************************
* This file is part of Eigen, a lightweight C++ template library		*
* for linear algebra.													*
*																		*
* Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>			*
* Copyright (C) 2011-2014 Gael Guennebaud <gael.guennebaud@inria.fr>	*
* Copyright (C) 2011-2012 Jitse Niesen <jitse@maths.leeds.ac.uk>		*
*																		*
* This Source Code Form is subject to the terms of the Mozilla			*
* Public License v. 2.0. If a copy of the MPL was not distributed		*
* with this file, You can obtain one at http://mozilla.org/MPL/2.0/.	*
*																		*
* Noted: I made some modifications in this file.						*
*************************************************************************/
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