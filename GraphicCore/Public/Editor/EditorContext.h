/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/CommonInterfaces.h"

namespace TG::Editor
{
	class EditorContext
	{
	public:
		explicit EditorContext(const IDefaultVideoPort& display);
		~EditorContext();

	private:
		bool m_isInitialized{ false };
	};
}
