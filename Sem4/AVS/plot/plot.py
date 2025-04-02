import matplotlib
import matplotlib.pyplot as plot
matplotlib.use("TkAgg")

# Читаем данные из файла
iterations = []
execution_time = []

with open("resultsGPU.txt", "r") as file:
    for line in file:
        parts = line.strip().split("|")
        if len(parts) == 2:
            iter_count = int(parts[0].strip())
            exec_time = float(parts[1].strip())
            iterations.append(iter_count)
            execution_time.append(exec_time)

# Строим график
plot.figure(figsize=(10, 5))
plot.plot(iterations, execution_time, marker="o", linestyle="-", color="b", label="Время выполнения / Кол-во итераций")

plot.xlabel("Количество итераций")
plot.ylabel("Время выполнения (сек)")
plot.title("Зависимость времени выполнения от количества итераций")
plot.legend()
plot.grid(True)

# Показываем график
plot.show()
