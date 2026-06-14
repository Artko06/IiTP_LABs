import pandas as pd
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt


# ------------------------------------------------------------
# 1. Загрузка и подготовка данных
# ------------------------------------------------------------
df = pd.read_csv('Cars Datasets.csv', quotechar='"', encoding='latin1')
print(f"Исходное количество строк: {len(df)}")

# Выбираем количественный показатель "Total Speed" (максимальная скорость)
# Удаляем " km/h" и переводим в число, пропуски (N/A) станут NaN
speed = df['Total Speed'].str.replace(' km/h', '', regex=False).str.strip()
speed = pd.to_numeric(speed, errors='coerce')
# Удаляем пропущенные значения
speed_clean = speed.dropna()
print(f"Количество значений после удаления пропусков: {len(speed_clean)}")

# Проверяем, что выборка не менее 40
if len(speed_clean) < 40:
    raise ValueError("Размер выборки меньше 40, выберите другой показатель.")

# Исходная выборка (до удаления выбросов) — для последующего сравнения
data_raw = speed_clean.values.copy()
n_raw = len(data_raw)
print(f"Размер исходной выборки: {n_raw}")

# ------------------------------------------------------------
# 2. Первичный анализ (мода, медиана, среднее, дисперсия, CV)
# ------------------------------------------------------------
mean_raw = np.mean(data_raw)
median_raw = np.median(data_raw)
var_raw = np.var(data_raw, ddof=0)   # дисперсия по генеральной совокупности (n)
std_raw = np.std(data_raw, ddof=0)

# Мода для непрерывных данных – приближённая (самое частое значение после округления)
unique, counts = np.unique(np.round(data_raw), return_counts=True)
mode_raw = unique[np.argmax(counts)]

cv_raw = (std_raw / mean_raw) * 100  # коэффициент вариации в %

print("\n--- Исходная выборка ---")
print(f"Среднее: {mean_raw:.2f} км/ч")
print(f"Медиана: {median_raw:.2f} км/ч")
print(f"Приближённая мода: {mode_raw:.2f} км/ч (для округлённых значений)")
print(f"Дисперсия: {var_raw:.2f} (км/ч)^2")
print(f"СКО: {std_raw:.2f} км/ч")
print(f"Коэффициент вариации: {cv_raw:.2f}%")

# Однородность по CV
if cv_raw < 33:
    print("Выборка однородна (CV < 33%)")
else:
    print("Выборка неоднородна (CV >= 33%)")

# ------------------------------------------------------------
# 3. Удаление выбросов (метод межквартильного размаха)
# ------------------------------------------------------------
Q1 = np.percentile(data_raw, 25)
Q3 = np.percentile(data_raw, 75)
IQR = Q3 - Q1
lower = Q1 - 1.5 * IQR
upper = Q3 + 1.5 * IQR

# Оставляем только значения без выбросов
mask = (data_raw >= lower) & (data_raw <= upper)
data = data_raw[mask]
n = len(data)
print(f"\nУдалено выбросов: {n_raw - n}")
print(f"Размер выборки после удаления выбросов: {n}")

# ------------------------------------------------------------
# 4. Основные статистики по очищенной выборке
# ------------------------------------------------------------
mean_val = np.mean(data)
median_val = np.median(data)
var_val = np.var(data, ddof=0)
std_val = np.std(data, ddof=0)
cv_val = (std_val / mean_val) * 100

# Мода (приближённо)
unique_clean, counts_clean = np.unique(np.round(data), return_counts=True)
mode_val = unique_clean[np.argmax(counts_clean)]

print("\n--- Очищенная выборка ---")
print(f"Среднее: {mean_val:.2f} км/ч")
print(f"Медиана: {median_val:.2f} км/ч")
print(f"Приближённая мода: {mode_val:.2f} км/ч")
print(f"Дисперсия: {var_val:.2f} (км/ч)^2")
print(f"СКО: {std_val:.2f} км/ч")
print(f"Коэффициент вариации: {cv_val:.2f}%")

if cv_val < 33:
    print("Выборка однородна")
else:
    print("Выборка неоднородна")

# ------------------------------------------------------------
# 5. Разбиение на интервалы и построение гистограммы/полигона
# ------------------------------------------------------------
# Количество интервалов по формуле: s = 1 + [3.322 * lg(n)]
s = 1 + int(3.322 * np.log10(n))
print(f"\nРекомендуемое количество интервалов (по формуле): s = {s}")

# Разбиваем на интервалы равной длины
bins = np.histogram_bin_edges(data, bins=s)
print(f"Границы интервалов: {np.round(bins, 2)}")

# Гистограмма и полигон
counts_hist, _ = np.histogram(data, bins=s)
# Центры интервалов для полигона
centers = (bins[:-1] + bins[1:]) / 2

plt.figure(figsize=(10, 5))
plt.hist(data, bins=s, edgecolor='black', alpha=0.7, density=True, label='Гистограмма')
plt.plot(centers, counts_hist / (n * (bins[1]-bins[0])), 'o-', color='red', label='Полигон частот')
plt.title(f'Гистограмма и полигон (s={s})')
plt.xlabel('Максимальная скорость (км/ч)')
plt.ylabel('Плотность вероятности')
plt.legend()
plt.grid(True)
plt.show()

print("\nПо виду гистограммы можно предположить, что распределение близко к нормальному (симметричное, колоколообразное), либо имеет небольшую правостороннюю асимметрию.")

# ------------------------------------------------------------
# 6. Групповые дисперсии (основное разбиение s интервалов)
# ------------------------------------------------------------
# Присваиваем каждой точке номер интервала
bin_indices = np.digitize(data, bins[:-1]) - 1  # индексы от 0 до s-1

group_means = []
group_vars = []
group_sizes = []

for i in range(s):
    group_data = data[bin_indices == i]
    group_sizes.append(len(group_data))
    if len(group_data) > 0:
        group_means.append(np.mean(group_data))
        group_vars.append(np.var(group_data, ddof=0))
    else:
        group_means.append(0)
        group_vars.append(0)

# Внутригрупповая дисперсия (средневзвешенная по размерам групп)
within_var = sum(size * var for size, var in zip(group_sizes, group_vars)) / n

# Межгрупповая дисперсия
between_var = sum(size * (m - mean_val)**2 for size, m in zip(group_sizes, group_means)) / n

total_var_calc = within_var + between_var

print("\n--- Групповые дисперсии (s интервалов) ---")
print(f"Внутригрупповая дисперсия: {within_var:.2f}")
print(f"Межгрупповая дисперсия: {between_var:.2f}")
print(f"Сумма = {within_var + between_var:.2f} (общая дисперсия = {var_val:.2f})")
print(f"Правило сложения дисперсий выполняется: расхождение = {abs(var_val - total_var_calc):.4f}")
print(f"Доля межгрупповой дисперсии: {between_var / var_val:.4f}")

# ------------------------------------------------------------
# 7. Анализ для разбиений s-1 и s+1 интервалов
# ------------------------------------------------------------
def compute_group_dispersions(data, s):
    bins = np.histogram_bin_edges(data, bins=s)
    bin_idx = np.digitize(data, bins[:-1]) - 1
    groups = []
    means = []
    sizes = []
    for i in range(s):
        g = data[bin_idx == i]
        sizes.append(len(g))
        means.append(np.mean(g) if len(g) > 0 else 0)
        groups.append(g)
    overall_mean = np.mean(data)
    within = sum(size * (np.var(g, ddof=0) if len(g)>0 else 0) for size, g in zip(sizes, groups)) / len(data)
    between = sum(size * (m - overall_mean)**2 for size, m in zip(sizes, means)) / len(data)
    total = within + between
    return within, between, total

for k in (s-1, s, s+1):
    w, b, t = compute_group_dispersions(data, k)
    print(f"\ns={k}: внутригрупповая={w:.2f}, межгрупповая={b:.2f}, общая={t:.2f}, доля межгрупповой={b/t:.4f}")

# Вывод о лучшем разбиении
print("\nЛучшее разбиение даёт наибольшую долю межгрупповой дисперсии, что означает лучшее разделение групп.")

# ------------------------------------------------------------
# 8. Проверка соответствия нормальному закону распределения
# ------------------------------------------------------------
# 8.1 Критерий Пирсона (хи-квадрат)
observed, bins_chi = np.histogram(data, bins=s)
observed = np.array(observed, dtype=float)

# Ожидаемые частоты для нормального распределения
mu, sigma = np.mean(data), np.std(data, ddof=0)
cdf = lambda x: stats.norm.cdf(x, loc=mu, scale=sigma)
expected = n * np.array([cdf(bins_chi[i+1]) - cdf(bins_chi[i]) for i in range(s)])

# Корректировка: сумма expected должна строго равняться сумме observed
# из-за погрешностей округления
expected = expected / expected.sum() * observed.sum()

# Проверка малых ожидаемых частот
if np.any(expected < 5):
    print("\nПредупреждение: некоторые ожидаемые частоты <5, результат критерия Пирсона может быть некорректным.")
    print("Рекомендуется объединить интервалы.")

# Критерий Пирсона (оцениваем 2 параметра: среднее и СКО, поэтому ddof=2)
# Примечание: ddof уменьшает число степеней свободы на количество оценённых параметров
chi2_stat, p_chi = stats.chisquare(f_obs=observed, f_exp=expected, ddof=2)
print(f"\nКритерий Пирсона: статистика = {chi2_stat:.3f}, p-value = {p_chi:.4f}")
if p_chi > 0.05:
    print("На уровне значимости 0.05 нет оснований отвергнуть гипотезу о нормальном распределении.")
else:
    print("Гипотеза о нормальном распределении отвергается.")

# 8.2 Критерий Колмогорова-Смирнова
ks_stat, p_ks = stats.kstest(data, 'norm', args=(mu, sigma))
print(f"\nКритерий Колмогорова-Смирнова: статистика = {ks_stat:.3f}, p-value = {p_ks:.4f}")
if p_ks > 0.05:
    print("По критерию К-С гипотеза о нормальности не отвергается.")
else:
    print("Гипотеза о нормальности отвергается.")

# Дополнительно: Q-Q plot для визуальной оценки
plt.figure()
stats.probplot(data, dist="norm", plot=plt)
plt.title('Q-Q plot (нормальное распределение)')
plt.grid()
plt.show()