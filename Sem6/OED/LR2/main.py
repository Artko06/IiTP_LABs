import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# ------------------------------------------------------------
# 1. Загрузка данных и выбор модели
# ------------------------------------------------------------
# Модель: стоимость автомобиля (y) зависит от:
#   x1 - мощность (HorsePower)
#   x2 - объём двигателя внутреннего сгорания (CC/Battery Capacity)
#   x3 - максимальная скорость (Total Speed)
#   x4 - время разгона 0-100 км/ч (Performance(0-100))
#   x5 - крутящий момент (Torque)

df = pd.read_csv('Cars Datasets.csv', encoding='latin1')

# ------------------------------------------------------------
# 2. Очистка и преобразование данных (универсальная функция)
# ------------------------------------------------------------

def extract_first_number(s):
    """
    Извлекает первое число из строки, игнорируя единицы измерения,
    пробелы внутри чисел и возможные диапазоны (вида '100 - 140').
    """
    if not isinstance(s, str):
        return np.nan
    # Удаляем известные единицы измерения (можно расширить)
    for unit in [' sec', ' km/h', ' hp', ' cc', ' Nm', ' kwh']:
        s = s.replace(unit, '')
    # Удаляем все пробелы (решает проблему "5. 0" -> "5.0")
    s = s.replace(' ', '')
    # На случай использования запятой как десятичного разделителя
    s = s.replace(',', '.')
    # Если есть дефис (диапазон), берём первую часть
    if '-' in s:
        s = s.split('-')[0]
    try:
        return float(s)
    except ValueError:
        return np.nan

# Цена
df['Price'] = df['Cars Prices'].str.replace('$', '', regex=False)\
                               .str.replace(',', '', regex=False)\
                               .str.replace(' ', '', regex=False)
df['Price'] = pd.to_numeric(df['Price'], errors='coerce')

# Мощность
df['HP'] = df['HorsePower'].apply(extract_first_number)

# Объём двигателя (только для ДВС, где есть 'cc')
def extract_cc(val):
    if isinstance(val, str) and 'cc' in val:
        return extract_first_number(val)
    return np.nan
df['CC'] = df['CC/Battery Capacity'].apply(extract_cc)

# Максимальная скорость
df['Speed'] = df['Total Speed'].apply(lambda x: extract_first_number(str(x)) if pd.notna(x) else np.nan)

# Время разгона
df['Accel'] = df['Performance(0 - 100 )KM/H'].apply(extract_first_number)

# Крутящий момент
df['Torque_num'] = df['Torque'].apply(extract_first_number)

# ------------------------------------------------------------
# 3. Формирование рабочей выборки
# ------------------------------------------------------------
cols = ['Price', 'HP', 'CC', 'Speed', 'Accel', 'Torque_num']
data = df[cols].dropna()
print(f"Размер выборки после удаления пропусков: {len(data)}")

# Убираем заведомо некорректные значения (нулевые и отрицательные)
data = data[(data['HP'] > 0) & (data['Price'] > 0) & (data['Speed'] > 0) &
            (data['Accel'] > 0) & (data['CC'] > 0)]
print(f"Размер выборки после фильтрации нулевых значений: {len(data)}")

if len(data) < 40:
    raise ValueError(f"Выборка слишком мала (n={len(data)}). Нужно не менее 40 наблюдений.")

# ------------------------------------------------------------
# 4. Корреляционный анализ (все факторы)
# ------------------------------------------------------------
print("\n" + "="*60)
print("КОРРЕЛЯЦИОННАЯ МАТРИЦА (все исходные факторы)")
print("="*60)
corr_matrix = data.corr()
print(corr_matrix.round(3))

# Тепловая карта
plt.figure(figsize=(8, 6))
sns.heatmap(corr_matrix, annot=True, cmap='coolwarm', center=0, fmt='.3f')
plt.title('Матрица парных корреляций (все факторы)')
plt.tight_layout()
plt.show()

# Выводы о зависимости y от x и между факторами
print("\n--- Выводы о связи цены (y) с факторами ---")
target_corr = corr_matrix['Price'].drop('Price')
for col, val in target_corr.items():
    direction = "прямая" if val > 0 else "обратная"
    if abs(val) >= 0.7:
        strength = "сильная"
    elif abs(val) >= 0.4:
        strength = "умеренная"
    else:
        strength = "слабая"
    print(f"  Price vs {col}: r = {val:.3f} ({direction} {strength} связь)")

print("\n--- Наиболее сильные взаимосвязи между факторами (мультиколлинеарность) ---")
# Ищем пары факторов (исключая Price) с высоким коэффициентом корреляции
factor_cols = [c for c in data.columns if c != 'Price']
pairs = []
for i in range(len(factor_cols)):
    for j in range(i+1, len(factor_cols)):
        col1, col2 = factor_cols[i], factor_cols[j]
        r_val = corr_matrix.loc[col1, col2]
        pairs.append((col1, col2, r_val))
pairs_sorted = sorted(pairs, key=lambda x: abs(x[2]), reverse=True)
for (c1, c2, r) in pairs_sorted[:3]:
    print(f"  {c1} – {c2}: r = {r:.3f}")

# ------------------------------------------------------------
# 5. Исключение факторов с |r| < 0.4 относительно y
# ------------------------------------------------------------
threshold = 0.4
selected = target_corr[abs(target_corr) >= threshold].index.tolist()
print(f"\nОставлены факторы (|r| ≥ {threshold}): {selected}")

if len(selected) == 0:
    print("Все факторы имеют слабую связь с ценой, дальнейший анализ невозможен.")
    exit()

data_sel = data[['Price'] + selected]

print("\n" + "="*60)
print("КОРРЕЛЯЦИОННАЯ МАТРИЦА (после отбора значимых факторов)")
print("="*60)
corr_sel = data_sel.corr()
print(corr_sel.round(3))

plt.figure(figsize=(6, 5))
sns.heatmap(corr_sel, annot=True, cmap='coolwarm', center=0, fmt='.3f')
plt.title('Матрица корреляций (отобранные факторы)')
plt.tight_layout()
plt.show()

print("\n--- Выводы после отбора факторов ---")
target_corr_sel = corr_sel['Price'].drop('Price')
for col, val in target_corr_sel.items():
    direction = "прямая" if val > 0 else "обратная"
    if abs(val) >= 0.7:
        strength = "сильная"
    elif abs(val) >= 0.4:
        strength = "умеренная"
    else:
        strength = "слабая"
    print(f"  Price vs {col}: r = {val:.3f} ({direction} {strength} связь)")

if len(selected) >= 2:
    print("\nМультиколлинеарность среди отобранных факторов:")
    for i, f1 in enumerate(selected):
        for f2 in selected[i+1:]:
            r = corr_sel.loc[f1, f2]
            print(f"  {f1} – {f2}: r = {r:.3f}")