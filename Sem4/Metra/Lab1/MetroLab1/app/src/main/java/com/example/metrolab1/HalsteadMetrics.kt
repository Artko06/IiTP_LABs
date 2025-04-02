package com.example.metrolab1

object HalsteadMetrics {
    private val keywords = listOf(
        "as", "as?", "break", "class", "continue", "do", "else", "for", "fun",
        "if", "in", "!in", "interface", "is", "!is", "object", "package", "return",
        "super", "this", "throw", "try", "typealias", "val", "var", "when", "while", "main", "import",
       "Boolean", "String", "Int", "Double", "Float", "Short", "Long", "Char", "repeat"
    ) // Актуальные ключевые слова Kotlin

    private val operatorPatterns = listOf(
        "===|!==|" +
                "\\+\\+|--|\\+=|-=|\\*=|/=|%=|&=|\\|=|\\^=|" +
                "==|!=|<=|>=|<|(?<!-)>|=|\\+|-(?!>)|/|\\*|%|&&|\\|\\||!",
        // Операторы in и !in
        "\\bin\\b|\\b!in\\b",
        // Операторы is и !is
        "\\bis\\b|\\b!is\\b",
        // Оператор эльвиса
        "\\?:",
        // Операторы управления потоком выполнения
        "\\breturn\\b|\\bcontinue\\b|\\bbreak\\b",
        // Условный оператор
        "\\bif\\b|\\bwhen\\b",
        // Операторы циклов
        "\\bdo\\b\\s*\\{[^}]*\\}\\s*while\\b|\\bwhile\\b|\\bfor\\b|\\brepeat\\b",
        // разделитель операторов программы
        ";",
        // Диапазон
        "\\.\\.<|\\.\\.",
        // Индексный оператор доступа
        "\\[.*\\]",
        // Вызов функции
        "\\b(?!${keywords.joinToString("|")})\\w+\\s*\\([^)]*\\)",
        // Открывающая и закрывающая фигурные скобки отдельно
        "\\{|\\}",
        // Круглые скобки
        "(?<!\\w|\\s)\\s*\\([^)]*\\)"
    )

    private val operandPatterns = listOf(
        "(?<!\")(?<!\\.)\\b\\w+(\\.\\d+)?(?!\\.|\\()\\b",    // Идентификаторы переменных, функций и т. д.
        "(?<!\\.)\"[^\"]*\"",     // Строки
        "(?<!\\.)'[^\']*'"        // Символы (одиночные кавычки)
    )

    val operatorsMap = mutableMapOf<String, Int>()
    val operandsMap = mutableMapOf<String, Int>()

    fun analyze(code: String) {
        operatorsMap.clear()
        operandsMap.clear()

        // Поиск операторов
        operatorPatterns.forEach { pattern ->
            val regex = Regex(pattern)
            val matches = regex.findAll(code)
            matches.forEach { match ->
                val operator = match.value
                operatorsMap[match.value] = operatorsMap.getOrDefault(match.value, 0) + 1
                println("Найдено оператор: $operator по паттерну: $pattern")
            }
        }

        // Поиск операндов
        operandPatterns.forEach { pattern ->
            val regex = Regex(pattern)
            val matches = regex.findAll(code)
            matches.forEach { match ->
                val operand = match.value
                if (operand !in keywords) { // Исключаем ключевые слова
                    operandsMap[operand] = operandsMap.getOrDefault(operand, 0) + 1
                    println("Найден операнд: $operand по паттерну: $pattern")
                }
            }
        }

        println("Статистика операторов:")
        operatorsMap.forEach { (operator, count) ->
            println("Оператор '$operator': $count раз(а)")
        }

        println("\nСтатистика операндов:")
        operandsMap.forEach { (operand, count) ->
            println("Операнд '$operand': $count раз(а)")
        }
    }

    fun getTableData(): List<TableRow> {
        val maxSize = maxOf(operatorsMap.size, operandsMap.size)
        val operatorList = operatorsMap.toList()
        val operandList = operandsMap.toList()

        return List(maxSize) { index ->
            TableRow(
                index = index + 1,
                operator = operatorList.getOrNull(index)?.first,
                operatorFrequency = operatorList.getOrNull(index)?.second,
                operand = operandList.getOrNull(index)?.first,
                operandFrequency = operandList.getOrNull(index)?.second
            )
        }
    }

    fun getCountOperators()  = operatorsMap.values.sum()
    fun getCountOperands() = operandsMap.values.sum()

    fun getCountUniqueOperators() = operatorsMap.size
    fun getCountUniqueOperands() = operandsMap.size
}