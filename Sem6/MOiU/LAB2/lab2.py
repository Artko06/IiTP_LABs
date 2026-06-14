import numpy as np
from copy import deepcopy

def print_lp_problem(c, d, A, b, r, sigma, title="Задача ЛП"):
    """Выводит задачу линейного программирования в читаемом виде."""
    print(f"--- {title} ---")
    
    # Целевая функция
    func_str = " + ".join([f"{c[i]}x{i+1}" for i in range(len(c))])
    print(f"Целевая функция: {func_str} -> {d}")
    
    # Ограничения
    print("Ограничения:")
    for i in range(A.shape[0]):
        row_str = " + ".join([f"({A[i, j]})x{j+1}" for j in range(A.shape[1])])
        print(f"  {row_str} {r[i]} {b[i]}")
        
    # Ограничения на переменные
    print("Переменные:")
    for i in range(len(sigma)):
        print(f"  x{i+1} {sigma[i]}")
    print("--------------------")


def to_normal_form(c, d, A, b, r, sigma):
    """
    Приводит задачу ЛП к нормальной форме.
    Нормальная форма:
    - c'x -> max
    - A'x <= b'
    - x >= 0
    """
    c, d, A, b, r, sigma = deepcopy(c), deepcopy(d), deepcopy(A), deepcopy(b), deepcopy(r), deepcopy(sigma)

    # Шаг 1: Целевая функция на max
    if d == 'min':
        c = -c
        d = 'max'

    # Шаг 2: Преобразование переменных
    num_vars = A.shape[1]
    for j in range(num_vars):
        # Переменные <= 0
        if sigma[j] == '<= 0':
            c[j] = -c[j]
            A[:, j] = -A[:, j]
            sigma[j] = '>= 0'
        # Переменные без ограничения знака
        elif sigma[j] == 'unrestricted':
            # Заменяем x_j на x_j' - x_j''
            A = np.insert(A, j + 1, -A[:, j], axis=1)
            c = np.insert(c, j + 1, -c[j])
            sigma[j] = '>= 0'
            sigma.insert(j + 1, '>= 0')
    
    num_constraints = A.shape[0]
    i = 0
    while i < num_constraints:
        # Шаг 3: Ограничения-равенства
        if r[i] == '=':
            A = np.vstack([A, A[i]])
            b = np.append(b, b[i])
            r[i] = '<='
            r.insert(i + 1, '>=')
            num_constraints += 1
        
        # Шаг 4: Ограничения >=
        elif r[i] == '>=':
            A[i, :] = -A[i, :]
            b[i] = -b[i]
            r[i] = '<='
        i += 1

    return c, d, A, b, r, sigma


def to_canonical_form(c, d, A, b, r, sigma):
    """
    Приводит задачу ЛП к канонической форме.
    Каноническая форма:
    - c'x -> max
    - A'x = b'
    - x >= 0
    """
    c, d, A, b, r, sigma = deepcopy(c), deepcopy(d), deepcopy(A), deepcopy(b), deepcopy(r), deepcopy(sigma)
    
    # Шаг 1: Целевая функция на max
    if d == 'min':
        c = -c
        d = 'max'

    # Шаг 2: Преобразование переменных (аналогично нормальной форме)
    num_vars = A.shape[1]
    j = 0
    while j < num_vars:
        if sigma[j] == '<= 0':
            c[j] = -c[j]
            A[:, j] = -A[:, j]
            sigma[j] = '>= 0'
        elif sigma[j] == 'unrestricted':
            A = np.insert(A, j + 1, -A[:, j], axis=1)
            c = np.insert(c, j + 1, -c[j])
            sigma[j] = '>= 0'
            sigma.insert(j + 1, '>= 0')
            num_vars +=1
        j += 1

    # Шаг 3: Преобразование ограничений
    num_constraints = A.shape[0]
    for i in range(num_constraints):
        # b_i < 0
        if b[i] < 0:
            A[i,:] *= -1
            b[i] *= -1
            if r[i] == '<=': r[i] = '>='
            elif r[i] == '>=': r[i] = '<='

    for i in range(num_constraints):
        if r[i] == '<=':
            # Добавляем slack-переменную
            A = np.insert(A, A.shape[1], 0, axis=1)
            A[i, -1] = 1
            c = np.append(c, 0)
            sigma.append('>= 0')
            r[i] = '='
        elif r[i] == '>=':
            # Добавляем surplus-переменную
            A = np.insert(A, A.shape[1], 0, axis=1)
            A[i, -1] = -1
            c = np.append(c, 0)
            sigma.append('>= 0')
            r[i] = '='

    return c, d, A, b, r, sigma


if __name__ == "__main__":
    # Исходная задача из примера (3) в lab2_conditions.pdf
    c_initial = np.array([-1.0, 1.0, -2.0])
    d_initial = 'min'
    A_initial = np.array([
        [1.0, 0.0, 1.0],
        [1.0, 0.0, -1.0],
        [1.0, 1.0, 0.0]
    ])
    b_initial = np.array([1.0, 2.0, 10.0])
    r_initial = ['<=', '>=', '=']
    sigma_initial = ['>= 0', '<= 0', 'unrestricted']

    print_lp_problem(c_initial, d_initial, A_initial, b_initial, r_initial, sigma_initial, "Исходная задача (3)")

    # Приведение к нормальной форме
    c_norm, d_norm, A_norm, b_norm, r_norm, sigma_norm = to_normal_form(c_initial, d_initial, A_initial, b_initial, r_initial, sigma_initial)
    print_lp_problem(c_norm, d_norm, A_norm, b_norm, r_norm, sigma_norm, "Нормальная форма")
    
    # Приведение к канонической форме
    c_canon, d_canon, A_canon, b_canon, r_canon, sigma_canon = to_canonical_form(c_initial, d_initial, A_initial, b_initial, r_initial, sigma_initial)
    print_lp_problem(c_canon, d_canon, A_canon, b_canon, r_canon, sigma_canon, "Каноническая форма")
