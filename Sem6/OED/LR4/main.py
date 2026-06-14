import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import statsmodels.api as sm
from scipy import stats
import warnings
warnings.filterwarnings('ignore')

# ------------------------------------------------------------
# 1. Загрузка данных и очистка (как в ЛР2)
# ------------------------------------------------------------
df = pd.read_csv('Cars Datasets.csv', encoding='latin1')

def extract_first_number(s):
    if not isinstance(s, str):
        return np.nan
    for unit in [' sec', ' km/h', ' hp', ' cc', ' Nm', ' kwh']:
        s = s.replace(unit, '')
    s = s.replace(' ', '')
    s = s.replace(',', '.')
    if '-' in s:
        s = s.split('-')[0]
    try:
        return float(s)
    except ValueError:
        return np.nan

df['Price'] = df['Cars Prices'].str.replace('$', '', regex=False)\
                               .str.replace(',', '', regex=False)\
                               .str.replace(' ', '', regex=False)
df['Price'] = pd.to_numeric(df['Price'], errors='coerce')

df['HP'] = df['HorsePower'].apply(extract_first_number)

def extract_cc(val):
    if isinstance(val, str) and 'cc' in val:
        return extract_first_number(val)
    return np.nan
df['CC'] = df['CC/Battery Capacity'].apply(extract_cc)

df['Speed'] = df['Total Speed'].apply(lambda x: extract_first_number(str(x)) if pd.notna(x) else np.nan)
df['Accel'] = df['Performance(0 - 100 )KM/H'].apply(extract_first_number)
df['Torque_num'] = df['Torque'].apply(extract_first_number)

# Рабочая выборка
cols = ['Price', 'HP', 'CC', 'Speed', 'Accel', 'Torque_num']
data = df[cols].dropna()
data = data[(data['HP'] > 0) & (data['Price'] > 0) & (data['Speed'] > 0) &
            (data['Accel'] > 0) & (data['CC'] > 0) & (data['Torque_num'] > 0)]
print(f"Размер рабочей выборки: {len(data)}")

# ------------------------------------------------------------
# 2. Корреляционный анализ и отбор факторов с |r| >= 0.4 (из ЛР2)
# ------------------------------------------------------------
corr_matrix = data.corr()
target_corr = corr_matrix['Price'].drop('Price')
threshold = 0.4
selected_factors = target_corr[abs(target_corr) >= threshold].index.tolist()
print(f"Факторы с |r| >= 0.4 по отношению к Price: {selected_factors}")

if len(selected_factors) == 0:
    raise ValueError("Нет факторов с достаточной корреляцией")

# ------------------------------------------------------------
# 3. Вычисление частных корреляций (partial correlation)
# ------------------------------------------------------------
try:
    from pingouin import partial_corr
except ImportError:
    # Установка пакета при необходимости
    import subprocess, sys
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pingouin"])
    from pingouin import partial_corr

# Создаём DataFrame с переменными: Price + отобранные факторы
df_partial = data[['Price'] + selected_factors]

# Для каждого фактора вычисляем частную корреляцию с Price,
# контролируя все остальные факторы из selected_factors
partial_corrs = {}
for factor in selected_factors:
    # Контролируемые переменные – все остальные факторы
    control_vars = [f for f in selected_factors if f != factor]
    pc = partial_corr(data=df_partial, x='Price', y=factor, covar=control_vars, method='pearson')
    partial_r = pc['r'].values[0]
    partial_corrs[factor] = partial_r
    print(f"Частная корреляция Price ~ {factor} (контроль: {control_vars}): r_p = {partial_r:.4f}")

# Выбор фактора с максимальным абсолютным значением частной корреляции
best_factor = max(partial_corrs, key=lambda k: abs(partial_corrs[k]))
best_partial_r = partial_corrs[best_factor]
print(f"\nВыбран фактор с наибольшей частной корреляцией: {best_factor} (r_p = {best_partial_r:.4f})")

# ------------------------------------------------------------
# 4. Построение модели парной линейной регрессии
# ------------------------------------------------------------
X = sm.add_constant(data[best_factor])
y = data['Price']
model = sm.OLS(y, X).fit()

print("\n" + "="*60)
print("РЕЗУЛЬТАТЫ ОЦЕНКИ МОДЕЛИ ПАРНОЙ РЕГРЕССИИ")
print("="*60)
print(model.summary())

# ------------------------------------------------------------
# 5. График рассеяния и линия регрессии
# ------------------------------------------------------------
plt.figure(figsize=(10, 6))
plt.scatter(data[best_factor], data['Price'], alpha=0.6, label='Наблюдения')
# Сортировка для красивой линии
sorted_idx = np.argsort(data[best_factor])
x_sorted = data[best_factor].iloc[sorted_idx]
y_pred = model.predict(sm.add_constant(x_sorted))
plt.plot(x_sorted, y_pred, color='red', linewidth=2, label='Линия регрессии')
plt.xlabel(best_factor)
plt.ylabel('Price (USD)')
plt.title(f'Парная регрессия: Price ~ {best_factor}')
plt.legend()
plt.grid(True)
plt.show()

# ------------------------------------------------------------
# 6. Доверительные интервалы для прогноза
# ------------------------------------------------------------
# Получим прогнозные значения и доверительные интервалы для среднего и индивидуального
pred = model.get_prediction(sm.add_constant(x_sorted))
summary_frame = pred.summary_frame(alpha=0.05)  # 95% доверительные интервалы

plt.figure(figsize=(10, 6))
plt.scatter(data[best_factor], data['Price'], alpha=0.6, label='Наблюдения')
plt.plot(x_sorted, summary_frame['mean'], color='red', label='Прогноз')
plt.fill_between(x_sorted, summary_frame['obs_ci_lower'], summary_frame['obs_ci_upper'],
                 alpha=0.2, color='blue', label='95% дов. интервал (индив.)')
plt.fill_between(x_sorted, summary_frame['mean_ci_lower'], summary_frame['mean_ci_upper'],
                 alpha=0.4, color='orange', label='95% дов. интервал (среднего)')
plt.xlabel(best_factor)
plt.ylabel('Price (USD)')
plt.title(f'Доверительные интервалы прогнозов')
plt.legend()
plt.grid(True)
plt.show()

# ------------------------------------------------------------
# 7. График остатков
# ------------------------------------------------------------
residuals = model.resid
plt.figure(figsize=(8, 5))
plt.scatter(data[best_factor], residuals, alpha=0.6)
plt.axhline(0, color='red', linestyle='--')
plt.xlabel(best_factor)
plt.ylabel('Остатки')
plt.title('График остатков регрессии')
plt.grid(True)
plt.show()

# Q-Q plot остатков
plt.figure(figsize=(6, 6))
stats.probplot(residuals, dist="norm", plot=plt)
plt.title('Q-Q plot остатков')
plt.grid(True)
plt.show()

print("\nВывод: построена парная линейная регрессия Price от", best_factor)
print("Модель показывает", "значимую" if model.f_pvalue < 0.05 else "незначимую",
      "связь (F-тест). Коэффициент детерминации R² =", round(model.rsquared, 4))