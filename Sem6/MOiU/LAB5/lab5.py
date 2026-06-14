TOL = 1e-9

def identity_matrix(n):
    """Возвращает единичную матрицу n×n."""
    return [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]

def inverse_matrix(matrix):
    """Находит обратную матрицу методом Гаусса–Жордана."""
    n = len(matrix)
    # Расширенная матрица [A | I]
    aug = [matrix[i][:] + identity_matrix(n)[i] for i in range(n)]

    for col in range(n):
        # Поиск опорного элемента (pivot)
        pivot = col
        while pivot < n and abs(aug[pivot][col]) < TOL:
            pivot += 1
        if pivot == n:
            raise ValueError("Матрица вырождена.")
        if pivot != col:
            aug[col], aug[pivot] = aug[pivot], aug[col]

        # Нормируем строку с pivot
        pivot_value = aug[col][col]
        for j in range(2 * n):
            aug[col][j] /= pivot_value

        # Обнуляем остальные строки в текущем столбце
        for row in range(n):
            if row == col:
                continue
            factor = aug[row][col]
            if abs(factor) < TOL:
                continue
            for j in range(2 * n):
                aug[row][j] -= factor * aug[col][j]

    # Возвращаем правую половину расширенной матрицы
    return [row[n:] for row in aug]

def get_submatrix_columns(matrix, columns):
    """Возвращает подматрицу, составленную из столбцов с указанными индексами."""
    return [[row[j] for j in columns] for row in matrix]

def matrix_vector_mul(matrix, vector):
    """Умножение матрицы на вектор-столбец."""
    return [sum(matrix[i][j] * vector[j] for j in range(len(vector))) for i in range(len(matrix))]

def row_vector_matrix_mul(vector, matrix):
    """Умножение вектора-строки на матрицу."""
    cols = len(matrix[0])
    rows = len(matrix)
    return [sum(vector[i] * matrix[i][j] for i in range(rows)) for j in range(cols)]

def dot(a, b):
    """Скалярное произведение двух векторов."""
    return sum(x * y for x, y in zip(a, b))

def column(matrix, j):
    """Возвращает j-й столбец матрицы (как список)."""
    return [matrix[i][j] for i in range(len(matrix))]

def round_vector(vector, digits=6):
    """Округляет компоненты вектора до заданного числа знаков."""
    return [round(x, digits) for x in vector]

def print_vector(name, vector):
    """Выводит вектор с округлением."""
    print(f"{name} = {round_vector(vector)}")

def print_matrix(name, matrix):
    """Выводит матрицу с округлением каждой строки."""
    print(f"{name} =")
    for row in matrix:
        print(round_vector(row))

def dual_simplex_method(c, a, b, basis, verbose=True):
    """
    Двойственный симплекс-метод.
    Вход:
      c, a, b – параметры прямой задачи: min c^T x, A x = b, x >= 0.
      basis – упорядоченное множество индексов начального базиса,
                соответствующее допустимому базисному плану двойственной задачи.
    Выход:
      optimal_x – оптимальный план прямой задачи,
      None – если прямая задача несовместна.
    """
    m = len(a)
    n = len(a[0])
    basis = basis[:]
    iteration = 1

    while True:
        if verbose:
            print(f"\n--- Итерация {iteration} ---")
            print(f"Текущий базис B = {[idx + 1 for idx in basis]}")

        # ---------- Шаг 1. Составляем базисную матрицу и находим обратную ----------
        a_b = get_submatrix_columns(a, basis)
        a_b_inv = inverse_matrix(a_b)

        # ---------- Шаг 2. Формируем вектор c_B ----------
        c_b = [c[j] for j in basis]

        # ---------- Шаг 3. Находим базисный допустимый план двойственной задачи y ----------
        y = row_vector_matrix_mul(c_b, a_b_inv)

        # ---------- Шаг 4. Находим псевдоплан κ ----------
        kappa_b = matrix_vector_mul(a_b_inv, b)
        kappa = [0.0] * n
        for pos, basis_index in enumerate(basis):
            kappa[basis_index] = kappa_b[pos]

        if verbose:
            print_matrix("A_B^-1", a_b_inv)
            print_vector("c_B", c_b)
            print_vector("Двойственный план y", y)
            print_vector("Псевдоплан kappa", kappa)

        # ---------- Шаг 5. Если κ ≥ 0, то найден оптимальный план прямой задачи ----------
        negative_positions = [i for i in range(m) if kappa_b[i] < -TOL]
        if not negative_positions:
            if verbose:
                print("Псевдоплан неотрицателен. Найден оптимальный план прямой задачи.")
            return [0.0 if abs(value) < TOL else value for value in kappa]

        # ---------- Шаг 6. Выделяем отрицательную компоненту и её индекс ----------
        # Выбираем самую отрицательную (с приоритетом по индексу переменной для определённости)
        k = min(negative_positions, key=lambda idx: (kappa_b[idx], basis[idx]))
        leaving_basis_index = basis[k]
        delta_y = a_b_inv[k][:]      # k-я строка A_B^-1 (Δy)

        if verbose:
            print(f"Отрицательная базисная компонента: kappa_{leaving_basis_index + 1} = {round(kappa_b[k], 6)}")
            print_vector("Delta_y", delta_y)

        # ---------- Шаг 7. Для каждого небазисного j вычисляем μ_j = Δy · A_j ----------
        candidates = []
        for j in range(n):
            if j in basis:
                continue
            mu_j = dot(delta_y, column(a, j))
            if verbose:
                print(f"mu_{j + 1} = {round(mu_j, 6)}")

            # ---------- Шаг 8. Если μ_j ≥ 0 для всех j, задача несовместна ----------
            # (проверка в конце цикла по наличию кандидатов)
            if mu_j < -TOL:   # μ_j < 0
                # ---------- Шаг 9. Вычисляем σ_j = (c_j - y^T A_j) / μ_j ----------
                sigma_j = (c[j] - dot(column(a, j), y)) / mu_j
                candidates.append((sigma_j, j, mu_j))
                if verbose:
                    print(f"sigma_{j + 1} = {round(sigma_j, 6)}")

        # Если нет ни одного j с μ_j < 0, то по шагу 8 прямая задача несовместна
        if not candidates:
            if verbose:
                print("Для всех небазисных индексов mu_j >= 0. Прямая задача несовместна.")
            return None

        # ---------- Шаг 10. Находим минимальное σ_j (наиболее близкое к нулю) ----------
        sigma0, j0, _ = min(candidates, key=lambda item: (item[0], item[1]))
        if verbose:
            print(f"Минимальное sigma0 = {round(sigma0, 6)}")
            print(f"В базис входит x{j0 + 1}, из базиса выходит x{leaving_basis_index + 1}")

        # ---------- Шаг 11. Заменяем k-й базисный индекс на j0 и переходим к следующей итерации ----------
        basis[k] = j0
        iteration += 1


if __name__ == "__main__":
    c_example = [-4.0, -3.0, -7.0, 0.0, 0.0]
    a_example = [
        [-2.0, -1.0, -4.0, 1.0, 0.0],
        [-2.0, -2.0, -2.0, 0.0, 1.0],
    ]
    b_example = [-1.0, -1.5]
    basis_example = [3, 4]   # индексы переменных x4 и x5 (нумерация с 0)

    result = dual_simplex_method(c_example, a_example, b_example, basis_example, verbose=True)
    if result is None:
        print("\nОтвет: прямая задача несовместна.")
    else:
        print("\n--- Результат ---")
        print_vector("Оптимальный план x", result)