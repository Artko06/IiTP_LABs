-- 1. Только часть таблиц (с другой структурой)
CREATE TABLE departments (
    dept_id NUMBER PRIMARY KEY,
    dept_name VARCHAR2(100) NOT NULL
    -- Нет: location, budget
);

CREATE TABLE employees (
    emp_id NUMBER PRIMARY KEY,
    first_name VARCHAR2(50),
    last_name VARCHAR2(50),
    dept_id NUMBER
    -- Нет: hire_date, salary, email
    -- Нет FOREIGN KEY
);

-- Таблица projects ОТСУТСТВУЕТ

-- 2. Последовательность только одна
CREATE SEQUENCE seq_dept_prod START WITH 10 INCREMENT BY 10;

-- 3. Только один индекс
CREATE INDEX idx_emp_last_name ON employees(last_name);

-- 4. Процедура с другим интерфейсом
CREATE OR REPLACE PROCEDURE hire_employee(
    p_first_name VARCHAR2,
    p_last_name VARCHAR2
    -- Нет параметров: p_dept_id, p_salary
) AS
BEGIN
    -- Пустая процедура
    NULL;
END;
/

-- Функции get_dept_salary_total НЕТ
-- Пакета hr_utils НЕТ

-- 5. Другие тестовые данные
INSERT INTO departments VALUES (seq_dept_prod.NEXTVAL, 'Development');
INSERT INTO departments VALUES (seq_dept_prod.NEXTVAL, 'Marketing');

INSERT INTO employees VALUES (1, 'Petr', 'Smirnov', 10);
INSERT INTO employees VALUES (2, 'Olga', 'Kuznetsova', 20);

COMMIT;

-- Проверка
SELECT 'PROD: Tables created: ' || COUNT(*) || ' tables' AS status
FROM user_tables;