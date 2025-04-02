fun main() {
    // Ввод данных
    print("Введите num1: ")
    val n1 = readln().toInt()
    print("Введите num2: ")
    var n2 = readln().toInt() ; var i = 0

    while (i < n1) {
        when (n2) {
            1 -> {
                if (n1 % 2 == 0) {
                    n2 *= 2
                } else {
                    n2 /= 2
                }
            }
            2 -> {
                if (n1 % 2 == 0) {
                    n2 *= 2
                } else {
                    n2 /= 2
                }
            }
            3 -> {
                when {
                    n1 <= 1 -> n2 *= 2
                    n1 <= 2 -> n2 /= 2
                    n1 <= 3 -> n2 += 2
                    n1 <= 4 -> n2 -= 2
                    n1 == 5 -> n2++
                    else -> n2 = 0
                }
            }
        }
        i++
    }

    println("Результат: n2 = $n2")
}