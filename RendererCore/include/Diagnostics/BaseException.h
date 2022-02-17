/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <exception>
#include <string>

namespace LCH
{
	class BaseException : public std::exception
	{
	public:
		BaseException(int lineNum, std::wstring filePath);

		virtual const char* what() const override;
		virtual const wchar_t* GetType() const noexcept;
		int GetLineNum() const noexcept;
		const std::wstring& GetFilePath() const noexcept;

	private:
		int lineNum;
		std::wstring filePath;

	protected:
		mutable std::string whatBuffer;
	};
}