/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename ScalarT, std::size_t RowsT, std::size_t ColumnsT, StorageOrder Order>
	struct Traits<Matrix<ScalarT, RowsT, ColumnsT, Order>>
	{
		using Scalar = ScalarT;
        static constexpr std::size_t	Rows = RowsT;
        static constexpr std::size_t	Columns = ColumnsT;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Order == StorageOrder::RowMajor ? XprFlag::RowMajor : XprFlag::None) |
			XprFlag::LeftValue | XprFlag::LinearAccess | XprFlag::NestByRef;
	};

	template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order>
	class Matrix final : public MatrixBase<Matrix<Scalar, Rows, Columns, Order>>
	{
		static constexpr bool IsVector = Rows == 1 || Columns == 1;
		static constexpr std::size_t Size = Rows * Columns;

	public:
		Matrix() = default;
		Matrix(const Matrix& other) = default;
		Matrix(Matrix&& other) = default;
		Matrix& operator=(const Matrix& other) = default;
		Matrix& operator=(Matrix&& other) = default;

		// 构造时才会调用，不存在aliasing问题
        template<typename Derived>
        Matrix(const MatrixBase<Derived>& other)
        {
            CallAssignmentNoAlias(this->Expression(), other.Expression());
        }

		Matrix(std::initializer_list<Scalar> values)
        {
        	const std::size_t count = std::min(values.size(), Rows * Columns);
        	auto it = values.begin();
        	for (int i = 0; i < count; ++i)
        	{
        		std::size_t index = i;
        		if constexpr (!HasFlag<Matrix, XprFlag::RowMajor>)
        		{
        			const std::size_t row = i / Columns;
        			const std::size_t column = i % Columns;
        			index = row + column * Rows;
        		}
        		m_storage[index] = *it++;
        	}
        }

		template<typename... Args>
		requires ((sizeof...(Args) > 0) && std::conjunction_v<std::is_convertible<Args, Scalar>...>)
		explicit Matrix(Args... values)
        {
        	Assign(values...);
        }

		template<typename Derived>
		Matrix& operator=(const MatrixBase<Derived>& other)
        {
        	CallAssignment(this->Expression(), other.Expression());
	        return *this;
        }

		Scalar const* Data() const noexcept { return m_storage; }

		/**
		 * 按照row major的形式填充矩阵，与矩阵保存数据的形式无关
		 * @values 填充矩阵的数据
		 */
		template<typename... Args> requires std::conjunction_v<std::is_convertible<Args, Scalar>...>
		void Assign(Args... values)
        {
        	Scalar temp[] = { static_cast<Scalar>(values)... };
        	constexpr std::size_t count = std::min(sizeof...(values), Rows * Columns);
        	for (int i = 0; i < count; ++i)
        	{
        		std::size_t index = i;
				if constexpr (!HasFlag<Matrix, XprFlag::RowMajor>)
				{
        			const std::size_t row = i / Columns;
        			const std::size_t column = i % Columns;
					index = row + column * Rows;
        		}
        		m_storage[index] = temp[i];
        	}
        }

		Scalar X() const requires (IsVector && Size > 0) { return m_storage[0]; }
		Scalar Y() const requires (IsVector && Size > 1) { return m_storage[1]; }
		Scalar Z() const requires (IsVector && Size > 2) { return m_storage[2]; }
		Scalar W() const requires (IsVector && Size > 3) { return m_storage[3]; }
		Scalar& X() requires (IsVector && Size > 0) { return m_storage[0]; }
		Scalar& Y() requires (IsVector && Size > 1) { return m_storage[1]; }
		Scalar& Z() requires (IsVector && Size > 2) { return m_storage[2]; }
		Scalar& W() requires (IsVector && Size > 3) { return m_storage[3]; }

		Scalar operator[](std::size_t index) const
		{
			return m_storage[index];
		}
		Scalar operator[](std::size_t row, std::size_t column) const
		{
			if constexpr (HasFlag<Matrix, XprFlag::RowMajor>)
				return m_storage[column + row * Columns];
			else
				return m_storage[row + column * Rows];
		}
		Scalar& operator[](std::size_t index)
		{
			return m_storage[index];
		}
		Scalar& operator[](std::size_t row, std::size_t column)
		{
            if constexpr (HasFlag<Matrix, XprFlag::RowMajor>)
				return m_storage[column + row * Columns];
            else
				return m_storage[row + column * Rows];
		}

	private:
		Scalar m_storage[Rows * Columns]{};
	};

	template<typename Scalar, std::size_t Size> using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, std::size_t Size> using RowVector = Matrix<Scalar, 1, Size>;

	template<typename Scalar> using Vector2 = Vector<Scalar, 2>;
	template<typename Scalar> using Vector3 = Vector<Scalar, 3>;
	template<typename Scalar> using Vector4 = Vector<Scalar, 4>;
	template<typename Scalar> using RowVector2 = Vector<Scalar, 2>;
	template<typename Scalar> using RowVector3 = Vector<Scalar, 3>;
	template<typename Scalar> using RowVector4 = Vector<Scalar, 4>;
	template<typename Scalar> using Matrix2 = Matrix<Scalar, 2, 2>;
	template<typename Scalar> using Matrix3 = Matrix<Scalar, 3, 3>;
	template<typename Scalar> using Matrix4 = Matrix<Scalar, 4, 4>;

	using Vector2f = Vector<float, 2>;
	using Vector3f = Vector<float, 3>;
	using Vector4f = Vector<float, 4>;
	using RowVector2f = RowVector<float, 2>;
	using RowVector3f = RowVector<float, 3>;
	using RowVector4f = RowVector<float, 4>;
	using Matrix2f = Matrix<float, 2, 2>;
	using Matrix3f = Matrix<float, 3, 3>;
	using Matrix4f = Matrix<float, 4, 4>;

	using Vector2d = Vector<double, 2>;
	using Vector3d = Vector<double, 3>;
	using Vector4d = Vector<double, 4>;
	using RowVector2d = RowVector<double, 2>;
	using RowVector3d = RowVector<double, 3>;
	using RowVector4d = RowVector<double, 4>;
	using Matrix2d = Matrix<double, 2, 2>;
	using Matrix3d = Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;
}
