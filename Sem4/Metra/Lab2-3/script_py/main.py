from antlr4 import *
from KotlinLexer import KotlinLexer
from KotlinParser import KotlinParser
from KotlinParserListener import KotlinParserListener


class JilbMetricsListener(KotlinParserListener):
    def __init__(self):
        self.conditional_count = 0  # Абсолютная сложность (CL)
        self.total_operators = 0   # Общее количество операторов
        self.max_nesting_level = 0 # Максимальный уровень вложенности (CLI)
        self.current_nesting_level = -1 # уровень вложенность начинается с 0
        self.operator_counts = {}  # Словарь для хранения операторов
        self.cases_when_count = -1 # Число кейсов в when

    def _count_operator(self, operator_name):
        """Добавляет оператор в словарь и увеличивает общий счетчик"""
        self.total_operators += 1
        if operator_name in self.operator_counts:
            self.operator_counts[operator_name] += 1
        else:
            self.operator_counts[operator_name] = 1

    @property
    def operators_get(self):
        return self.operator_counts.items()

    @property
    def absolute_complexity_get(self):
        return self.conditional_count

    @property
    def total_operators_count_get(self):
        return self.total_operators

    @property
    def relative_complexity_get(self):
        return round((self.conditional_count / max(self.total_operators, 1)), 2)

    @property
    def max_nesting_get(self):
        return self.max_nesting_level

    # === Условные операторы (if..else, when) ===
    def enterIfExpression(self, ctx: KotlinParser.IfExpressionContext):
        """Обрабатывает вход в конструкцию if"""
        self._count_operator("if..else")
        self.conditional_count += 1
        self.current_nesting_level += 1
        self.max_nesting_level = max(self.max_nesting_level, self.current_nesting_level)

    def exitIfExpression(self, ctx: KotlinParser.IfExpressionContext):
        """Обрабатывает выход из конструкции if"""
        self.current_nesting_level -= 1

    def enterWhenExpression(self, ctx: KotlinParser.WhenExpressionContext):
        """Обрабатывает вход в конструкцию when"""
        self._count_operator("when")
        self.conditional_count += 1

        when_text = ctx.getText()
        if when_text.endswith("}"):
            self._count_operator("{ }")

    def enterWhenEntry(self, ctx:KotlinParser.WhenEntryContext):
        self.current_nesting_level += 1
        self.max_nesting_level = max(self.max_nesting_level, self.current_nesting_level)

        if "else->" != ctx.getText()[0:6]:
            self.cases_when_count += 1

    def exitWhenEntry(self, ctx:KotlinParser.WhenEntryContext):
        self.current_nesting_level -= 1
        if self.current_nesting_level > 0:
            self.max_nesting_level = max(self.max_nesting_level, self.cases_when_count + self.current_nesting_level)
        else:
            self.max_nesting_level = max(self.max_nesting_level, self.cases_when_count)


    def exitWhenExpression(self, ctx: KotlinParser.WhenExpressionContext):
        """Обрабатывает выход из конструкции when"""
        self.cases_when_count = -1


    # === Циклы (for, while, do...while) ===
    def enterForExpression(self, ctx: KotlinParser.ForExpressionContext):
        """Обрабатывает вход в конструкцию for"""
        self._count_operator("for")
        self.conditional_count += 1
        self.current_nesting_level += 1
        self.max_nesting_level = max(self.max_nesting_level, self.current_nesting_level)

    def exitForExpression(self, ctx: KotlinParser.ForExpressionContext):
        """Обрабатывает выход из конструкции for"""
        self.current_nesting_level -= 1

    def enterWhileExpression(self, ctx: KotlinParser.WhileExpressionContext):
        """Обрабатывает вход в конструкцию while"""
        self._count_operator("while")
        self.conditional_count += 1
        self.current_nesting_level += 1
        self.max_nesting_level = max(self.max_nesting_level, self.current_nesting_level)

    def exitWhileExpression(self, ctx: KotlinParser.WhileExpressionContext):
        """Обрабатывает выход из конструкции while"""
        self.current_nesting_level -= 1

    def enterDoWhileExpression(self, ctx: KotlinParser.DoWhileExpressionContext):
        """Обрабатывает вход в конструкцию do..while"""
        self._count_operator("do..while")
        self.conditional_count += 1
        self.current_nesting_level += 1
        self.max_nesting_level = max(self.max_nesting_level, self.current_nesting_level)

    def exitDoWhileExpression(self, ctx: KotlinParser.DoWhileExpressionContext):
        """Обрабатывает выход из конструкции do..while"""
        self.current_nesting_level -= 1

    # === Управление потоком (return, continue, break) ===
    def enterJumpExpression(self, ctx: KotlinParser.JumpExpressionContext):
        text = ctx.getText()

        if "break" in text:
            self._count_operator("break")
        elif "continue" in text:
            self._count_operator("continue")
        elif "return" in text:
            self._count_operator("return")

    # === Оператор присваивания ===
    # Не учитывает инициализацию переменной
    def enterAssignmentOperator(self, ctx:KotlinParser.AssignmentOperatorContext):
        self._count_operator(ctx.getText())

    # Учитывает инициализацию переменной как оператор присваивания
    def enterVariableDeclaration(self, ctx: KotlinParser.VariableDeclarationContext):
        self._count_operator("=")

    def enterComparisonOperator(self, ctx:KotlinParser.ComparisonOperatorContext):
        self._count_operator(ctx.getText())

    # def enterEveryRule(self, ctx: ParserRuleContext):
    #     print(f"Правило: {type(ctx).__name__}, текст: {ctx.getText()}")

    # === Алгебраические операции ===
    def enterAdditiveOperator(self, ctx:KotlinParser.AdditiveOperatorContext):
        text = ctx.getText()

        if "+" in text:
            self._count_operator("+")
        elif "-" in text:
            self._count_operator("-")

    def enterMultiplicativeOperation(self, ctx:KotlinParser.MultiplicativeOperationContext):
        text = ctx.getText()

        if "+" in text:
            self._count_operator("*")
        elif "-" in text:
            self._count_operator("/")
        elif "%" in text:
            self._count_operator("%")

    # === Оператор ==/!= ===
    def enterEqualityOperation(self, ctx:KotlinParser.EqualityOperationContext):
        self._count_operator(ctx.getText())

    # === Обрабатывает унарные операторы ++ -- ===
    def enterPrefixUnaryOperation(self, ctx: KotlinParser.PrefixUnaryOperationContext):
        text = ctx.getText()

        if "++" in text:
            self._count_operator("++")
        elif "--" in text:
            self._count_operator("--")
        elif "!" in text:
            self._count_operator("!")

    def enterPostfixUnaryOperation(self, ctx:KotlinParser.PostfixUnaryOperationContext):
        text = ctx.getText()

        if "++" in text:
            self._count_operator("++")
        elif "--" in text:
            self._count_operator("--")

    # === Оператор ; ===
    def enterAnysemi(self, ctx:KotlinParser.AnysemiContext):
        if ";" in ctx.getText():
            self._count_operator(";")

    # === Оператор in ===
    def enterInOperator(self, ctx: KotlinParser.InOperatorContext):
        self._count_operator(ctx.getText())

    # === Фигурные скобки ===
    def enterBlock(self, ctx: KotlinParser.BlockContext):
        self._count_operator("{ }")

    # Обработчик для вызова функции через CallSuffix
    def enterCallSuffix(self, ctx: KotlinParser.CallSuffixContext):
        self._count_operator("function_call")


def calculate_jilb_metrics(file_path, metric_class: JilbMetricsListener):
    # Чтение кода из файла
    with open(file_path, 'r', encoding='utf-8') as file:
        kotlin_code = file.read()

    # Создание потока символов
    input_stream = InputStream(kotlin_code)

    # Лексический анализ
    lexer = KotlinLexer(input_stream)
    stream = CommonTokenStream(lexer)

    # Парсинг кода
    parser = KotlinParser(stream)
    tree = parser.kotlinFile()

    # Анализ дерева с помощью слушателя
    listener_metric = metric_class
    walker = ParseTreeWalker()
    walker.walk(listener_metric, tree)


# Пример использования
if __name__ == "__main__":
    kotlin_file_path = "kotlin.kt"

    listener = JilbMetricsListener()

    # Расчет метрик Джилба
    calculate_jilb_metrics(kotlin_file_path, listener)

    print("\nЧастота операторов:")
    for op, count in listener.operators_get:
        print(f"  {op} -> {count}")

    print(f"Абсолютная сложность (CL): {listener.absolute_complexity_get}")
    print(f"Общее количество операторов: {listener.total_operators_count_get}")
    print(f"Относительная сложность (cl): {listener.relative_complexity_get}")
    print(f"Максимальный уровень вложенности (CLI): {listener.max_nesting_get}")