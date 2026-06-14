from collections import deque

# Допустимая погрешность для сравнения чисел с плавающей точкой
TOL = 1e-9


def copy_matrix(matrix):
    """Возвращает глубокую копию матрицы (списка списков)."""
    return [row[:] for row in matrix]


def round_vector(vector, digits=6):
    """Округляет элементы вектора до указанного количества знаков."""
    return [round(x, digits) for x in vector]


def round_matrix(matrix, digits=6):
    """Округляет все элементы матрицы."""
    return [round_vector(row, digits) for row in matrix]


def print_matrix(name, matrix):
    """Выводит матрицу с округлением для красоты."""
    print(f"{name} =")
    for row in round_matrix(matrix):
        print(row)


def northwest_corner_method(supply, demand, verbose=True):
    """
    Метод северо-западного угла для построения начального базисного плана.

    Алгоритм:
        1. Начинаем с верхней левой клетки (i=0, j=0).
        2. Перевозим min(a_i, b_j) единиц, уменьшаем остатки.
        3. Если остаток в строке обнулился – переходим к следующей строке (i++).
        4. Если остаток в столбце обнулился – переходим к следующему столбцу (j++).
        5. Повторяем, пока не дойдём до конца.

    Возвращает:
        x – матрица плана перевозок,
        basis – список базисных позиций (кортежи (i, j)).
    """
    # Копируем исходные запасы и заявки, чтобы не изменять переданные списки
    a = supply[:]
    b = demand[:]
    m = len(a)   # количество строк (поставщиков)
    n = len(b)   # количество столбцов (потребителей)

    # Нулевая матрица плана
    x = [[0.0 for _ in range(n)] for _ in range(m)]
    basis = []   # список базисных клеток

    i, j = 0, 0   # начинаем с "северо-западного" угла

    while True:
        # Добавляем текущую клетку в базис
        basis.append((i, j))
        amount = min(a[i], b[j])   # максимально возможная перевозка
        x[i][j] = amount
        a[i] -= amount
        b[j] -= amount

        if verbose:
            print(f"Добавлена базисная позиция ({i+1}, {j+1}), x[{i+1}][{j+1}] = {round(amount, 6)}")

        # Избавляемся от погрешностей
        if abs(a[i]) < TOL:
            a[i] = 0.0
        if abs(b[j]) < TOL:
            b[j] = 0.0

        # Правила перехода:
        # Если в текущей строке больше нет продукции (a[i]==0) и это не последняя строка,
        # переходим вниз.
        if a[i] == 0 and i < m - 1:
            i += 1
        # Если в текущем столбце заявка удовлетворена (b[j]==0) и это не последний столбец,
        # переходим вправо.
        elif a[i] > 0 and b[j] == 0 and j < n - 1:
            j += 1
        else:
            # Выход, когда достигли последней строки/столбца
            break

    return x, basis


def compute_potentials(costs, basis):
    """
    Вычисляет потенциалы u_i и v_j для базисных клеток.

    Система: u_i + v_j = c_{ij} для всех (i,j) в базисе.
    Одно неизвестное фиксируется: u_0 = 0.
    Потенциалы находятся последовательным обходом графа базисных клеток.
    """
    m = len(costs)
    n = len(costs[0])
    u = [None] * m   # потенциалы строк (поставщиков)
    v = [None] * n   # потенциалы столбцов (потребителей)

    u[0] = 0.0       # фиксируем первый потенциал строки

    # Будем итеративно распространять известные потенциалы по соседним базисным клеткам
    changed = True
    while changed:
        changed = False
        for i, j in basis:
            if u[i] is not None and v[j] is None:
                v[j] = costs[i][j] - u[i]
                changed = True
            elif u[i] is None and v[j] is not None:
                u[i] = costs[i][j] - v[j]
                changed = True

    return u, v


def find_entering_position(costs, u, v, basis):
    """
    Находит небазисную клетку для ввода в базис (с отрицательной оценкой Δ_{ij}).

    Оценка: Δ_{ij} = c_{ij} - u_i - v_j.
    Если все оценки ≥ 0, план оптимален.
    Выбирается лексикографически минимальная позиция с Δ < 0.
    """
    m = len(costs)
    n = len(costs[0])
    basis_set = set(basis)          # множество базисных клеток для быстрой проверки
    candidates = []

    for i in range(m):
        for j in range(n):
            if (i, j) in basis_set:
                continue
            delta = costs[i][j] - u[i] - v[j]
            if delta < -TOL:        # отрицательная оценка (с учётом погрешности)
                candidates.append((i, j, delta))

    if not candidates:
        return None                 # оптимальный план найден

    # Лексикографически минимальная позиция (сначала по i, потом по j)
    candidates.sort(key=lambda item: (item[0], item[1]))
    return candidates[0]            # возвращаем (i, j, delta)


def build_cycle(basis, entering, m, n):
    """
    Строит единственный цикл (simple cycle) после добавления вводимой клетки.

    В транспортной задаче добавление небазисной клетки к базису
    создаёт ровно один цикл. Алгоритм:
        - Строим двудольный граф: вершины – строки и столбцы,
          рёбра – базисные клетки.
        - Добавляем ребро (entering), находим путь между его концами.
        - Преобразуем путь в последовательность клеток цикла.
    """
    # Граф: ключи — кортежи ('r', i) или ('c', j), значения — списки смежных вершин
    graph = {}
    for i in range(m):
        graph[("r", i)] = []   # r – строка
    for j in range(n):
        graph[("c", j)] = []   # c – столбец

    # Добавляем рёбра для базисных клеток
    for i, j in basis:
        row_node = ("r", i)
        col_node = ("c", j)
        graph[row_node].append(col_node)
        graph[col_node].append(row_node)

    # Начало и конец пути – вершины, соответствующие вводимой клетке
    start = ("r", entering[0])
    finish = ("c", entering[1])

    # BFS для поиска пути от start до finish
    queue = deque([start])
    parent = {start: None}

    while queue:
        node = queue.popleft()
        if node == finish:
            break
        for nxt in graph[node]:
            if nxt not in parent:
                parent[nxt] = node
                queue.append(nxt)

    # Восстанавливаем путь вершин
    node_path = []
    current = finish
    while current is not None:
        node_path.append(current)
        current = parent[current]
    node_path.reverse()          # теперь от start до finish

    # Преобразуем путь вершин в последовательность клеток цикла.
    # Первая клетка – вводимая.
    cycle = [entering]
    for idx in range(len(node_path) - 1):
        left = node_path[idx]
        right = node_path[idx + 1]
        if left[0] == "r":               # left – строка, right – столбец
            cycle.append((left[1], right[1]))
        else:                            # left – столбец, right – строка
            cycle.append((right[1], left[1]))

    return cycle


def update_plan_by_cycle(plan, basis, cycle, verbose=True):
    """
    Выполняет перераспределение поставок по найденному циклу.

    Правила:
        - Клетки на чётных позициях в цикле (начиная с 0) получают +θ.
        - Клетки на нечётных позициях получают -θ.
        - θ = min(plan[i][j]) по клеткам со знаком "-".
        - Клетка, где достигается минимум, выходит из базиса.
    """
    # Индексы клеток со знаком "-" – нечётные позиции в списке cycle
    minus_positions = [cycle[idx] for idx in range(1, len(cycle), 2)]
    theta = min(plan[i][j] for i, j in minus_positions)

    # Находим лексикографически минимальную клетку, где план обнуляется
    leaving_candidates = [(i, j) for i, j in minus_positions if abs(plan[i][j] - theta) < TOL]
    leaving = min(leaving_candidates)   # лексикографический минимум по (i,j)

    if verbose:
        print(f"Цикл = {[(i+1, j+1) for i, j in cycle]}")
        print(f"theta = {round(theta, 6)}")
        print(f"Из базиса выходит позиция ({leaving[0]+1}, {leaving[1]+1})")

    # Создаём новый план
    new_plan = copy_matrix(plan)
    for idx, (i, j) in enumerate(cycle):
        if idx % 2 == 0:      # чётная позиция – прибавляем
            new_plan[i][j] += theta
        else:                 # нечётная – вычитаем
            new_plan[i][j] -= theta
        if abs(new_plan[i][j]) < TOL:
            new_plan[i][j] = 0.0

    # Обновляем базис: добавляем вводимую клетку, удаляем выходящую
    new_basis = basis[:] + [cycle[0]]
    new_basis.remove(leaving)

    return new_plan, new_basis


def total_cost(plan, costs):
    """Вычисляет суммарную стоимость перевозок."""
    total = 0.0
    for i in range(len(plan)):
        for j in range(len(plan[0])):
            total += plan[i][j] * costs[i][j]
    return total


def solve_transport_problem(costs, supply, demand, verbose=True):
    """
    Основная функция решения транспортной задачи.

    Параметры:
        costs – матрица стоимостей (m x n),
        supply – список запасов поставщиков (длина m),
        demand – список потребностей потребителей (длина n),
        verbose – печатать ли промежуточную информацию.

    Возвращает:
        plan – оптимальный план перевозок,
        basis – оптимальный базис,
        cost – минимальная стоимость.
    """
    # 1. Проверка неотрицательности
    if any(value < -TOL for value in supply) or any(value < -TOL for value in demand):
        raise ValueError("Поставки и заявки должны быть неотрицательными.")

    # 2. Проверка условия баланса
    if abs(sum(supply) - sum(demand)) > TOL:
        raise ValueError("Транспортная задача несовместна: не выполнено условие баланса.")

    if verbose:
        print("--- Метод северо-западного угла ---")

    # 3. Построение начального базисного плана
    plan, basis = northwest_corner_method(supply, demand, verbose=verbose)

    if verbose:
        print_matrix("Начальный план перевозок", plan)
        print(f"Начальный базис B = {[(i+1, j+1) for i, j in basis]}")

    iteration = 1
    while True:
        if verbose:
            print(f"\n--- Итерация улучшения {iteration} ---")

        # 4. Вычисление потенциалов
        u, v = compute_potentials(costs, basis)

        # 5. Поиск вводимой клетки (отрицательной оценки)
        entering_info = find_entering_position(costs, u, v, basis)

        if verbose:
            # Для вывода заменяем None на 0.0 (только для отображения)
            print_vector_u = [0.0 if value is None else value for value in u]
            print_vector_v = [0.0 if value is None else value for value in v]
            print(f"u = {round_vector(print_vector_u)}")
            print(f"v = {round_vector(print_vector_v)}")

        # 6. Проверка оптимальности
        if entering_info is None:
            if verbose:
                print("Все небазисные оценки неотрицательны. План оптимален.")
            return plan, basis, total_cost(plan, costs)

        enter_i, enter_j, delta = entering_info
        if verbose:
            print(f"В базис входит позиция ({enter_i+1}, {enter_j+1}), delta = {round(delta, 6)}")

        # 7. Построение цикла и перераспределение поставок
        cycle = build_cycle(basis, (enter_i, enter_j), len(costs), len(costs[0]))
        plan, basis = update_plan_by_cycle(plan, basis, cycle, verbose=verbose)

        if verbose:
            print_matrix("Новый план перевозок", plan)
            print(f"Новый базис B = {[(i+1, j+1) for i, j in basis]}")

        iteration += 1

if __name__ == "__main__":
    costs_example = [
        [8.0, 4.0, 1.0],
        [8.0, 4.0, 3.0],
        [9.0, 7.0, 5.0],
    ]
    supply_example = [100.0, 300.0, 300.0]
    demand_example = [300.0, 200.0, 200.0]

    plan_result, basis_result, cost_result = solve_transport_problem(
        costs_example,
        supply_example,
        demand_example,
        verbose=True,
    )

    print("\n--- Результат ---")
    print_matrix("Оптимальный план перевозок", plan_result)
    print(f"Оптимальный базис B = {[(i+1, j+1) for i, j in basis_result]}")
    print(f"Минимальная стоимость = {round(cost_result, 6)}")