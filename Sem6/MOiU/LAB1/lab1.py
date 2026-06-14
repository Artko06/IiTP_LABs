import numpy as np
import json

def invert_modified_matrix(A_inv, x, i):
    n = A_inv.shape[0]
    if i < 1 or i > n:
        raise ValueError(f"Индекс i должен быть в диапазоне [1, {n}]")

    # Переводим i в 0-based индекс
    i_zero_based = i - 1

    # Шаг 1: Находим вектор l = A^-1 * x
    l = np.dot(A_inv, x)
    print(f"Шаг 1: Вектор l = A^-1 * x:\n{l}\n")

    if np.isclose(l[i_zero_based], 0):
        print(f"l[{i_zero_based}] = {l[i_zero_based]}, матрица A_bar необратима.")
        return None
    else:
        print(f"l[{i_zero_based}] = {l[i_zero_based]} != 0, матрица A_bar обратима.")

    # Шаг 2: Формируем вектор l_prime (замена i-го элемента на -1)
    l_prime = l.copy()
    l_prime[i_zero_based] = -1
    print(f"Шаг 2: Вектор l' (l_prime):{l_prime}")

    # Шаг 3: Находим вектор l_hat
    l_hat = (-1 / l[i_zero_based]) * l_prime
    print(f"Шаг 3: Вектор l_hat:{l_hat}")

    # Шаг 4: Формируем матрицу Q
    Q = np.identity(n)
    Q[:, i_zero_based] = l_hat

    print(f"Шаг 4: Матрица Q:{Q}")

    # Шаг 5: Находим (A_bar)^-1 = Q * A^-1
    A_bar_inv = np.dot(Q, A_inv)
    print(f"Шаг 5: Итоговая обратная матрица (A_bar)^-1 = Q * A^-1:{A_bar_inv}")

    return {
        "l": l.tolist(),
        "li": float(l[i_zero_based]),
        "l_prime": l_prime.tolist(),
        "l_hat": l_hat.tolist(),
        "Q": Q.tolist(),
        "A_bar_inv": A_bar_inv.tolist()
    }

def main():
    with open('lab1_input.json', 'r') as f:
        data = json.load(f)

    A = np.array(data['A'])
    A_inv = np.array(data['A_inv'])
    x = np.array(data['x'])
    i = data['i']

    print(f"Исходная матрица A:{A}")
    print(f"Обратная матрица A^-1:{A_inv}")
    print(f"Вектор-столбец x:{x}")
    print(f"Индекс столбца для замены i = {i}")

    A_bar = A.copy()
    A_bar[:, i-1] = x
    print(f"Матрица A_bar (A с замененным {i}-м столбцом):{A_bar}")

    results = invert_modified_matrix(A_inv, x, i)

    if results is not None:
        print("--- Проверка результата ---")
        numpy_inv = np.linalg.inv(A_bar)
        print(f"Обратная матрица, вычисленная с помощью np.linalg.inv(A_bar):{numpy_inv}")

        output_data = {
            "A_bar": A_bar.tolist(),
            "A_bar_inv": results["A_bar_inv"],
            "l": results["l"],
            "li": results["li"],
            "l_tilde": results["l_prime"],
            "l_hat": results["l_hat"],
            "Q": results["Q"]
        }

        with open('lab1_output.json', 'w') as f:
            json.dump(output_data, f, indent=2)
        print("\nРезультаты сохранены в lab1_output.json")

if __name__ == "__main__":
    main()
