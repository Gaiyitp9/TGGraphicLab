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
	        Set(values);
        }

		template<typename Derived>
		Matrix& operator=(const MatrixBase<Derived>& other)
        {
        	CallAssignment(this->Expression(), other.Expression());
	        return *this;
        }

		Scalar const* Data() const noexcept { return m_storage; }

		void Set(std::initializer_list<Scalar> values)
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

	using Vector2F = Vector<float, 2>;
	using Vector3F = Vector<float, 3>;
	using Vector4F = Vector<float, 4>;
	using RowVector2F = RowVector<float, 2>;
	using RowVector3F = RowVector<float, 3>;
	using RowVector4F = RowVector<float, 4>;
	using Matrix2F = Matrix<float, 2, 2>;
	using Matrix3F = Matrix<float, 3, 3>;
	using Matrix4F = Matrix<float, 4, 4>;

	using Vector2D = Vector<double, 2>;
	using Vector3D = Vector<double, 3>;
	using Vector4D = Vector<double, 4>;
	using RowVector2D = RowVector<double, 2>;
	using RowVector3D = RowVector<double, 3>;
	using RowVector4D = RowVector<double, 4>;
	using Matrix2D = Matrix<double, 2, 2>;
	using Matrix3D = Matrix<double, 3, 3>;
	using Matrix4D = Matrix<double, 4, 4>;

    template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order, bool IsConst>
    class Evaluator<Matrix<Scalar, Rows, Columns, Order>, IsConst>
    {
        using Xpr = Matrix<Scalar, Rows, Columns, Order>;
    	using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;

    public:
        explicit Evaluator(InternalXpr& matrix) : m_matrix(matrix) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const
        {
            return m_matrix[index];
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_matrix[row, column];
        }

    	Scalar& Entry(std::size_t index) requires !IsConst
        {
        	return m_matrix[index];
        }

    	Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
        	return m_matrix[row, column];
        }

    private:
        InternalXpr& m_matrix;
    };
}
