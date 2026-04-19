-- 1. Таблицы
CREATE TABLE departments (
    dept_id NUMBER PRIMARY KEY,
    dept_name VARCHAR2(100) NOT NULL,
    location VARCHAR2(100),
    budget NUMBER(15,2)
);

CREATE TABLE employees (
    emp_id NUMBER PRIMARY KEY,
    first_name VARCHAR2(50),
    last_name VARCHAR2(50),
    dept_id NUMBER REFERENCES departments(dept_id),
    hire_date DATE DEFAULT SYSDATE,
    salary NUMBER(10,2),
    email VARCHAR2(100)
);

CREATE TABLE projects (
    project_id NUMBER PRIMARY KEY,
    project_name VARCHAR2(100),
    manager_id NUMBER REFERENCES employees(emp_id),
    start_date DATE,
    end_date DATE
);

-- 2. Последовательности
CREATE SEQUENCE seq_dept START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE seq_emp START WITH 1000 INCREMENT BY 1;
CREATE SEQUENCE seq_proj START WITH 5000 INCREMENT BY 1;

-- 3. Индексы
CREATE INDEX idx_emp_dept ON employees(dept_id);
CREATE INDEX idx_emp_name ON employees(last_name, first_name);
CREATE INDEX idx_proj_dates ON projects(start_date, end_date);

-- 4. Процедура
CREATE OR REPLACE PROCEDURE hire_employee(
    p_first_name VARCHAR2,
    p_last_name VARCHAR2,
    p_dept_id NUMBER,
    p_salary NUMBER DEFAULT 50000
) AS
BEGIN
    INSERT INTO employees (emp_id, first_name, last_name, dept_id, salary, hire_date)
    VALUES (seq_emp.NEXTVAL, p_first_name, p_last_name, p_dept_id, p_salary, SYSDATE);
    COMMIT;
    DBMS_OUTPUT.PUT_LINE('Employee hired: ' || p_first_name || ' ' || p_last_name);
END;
/

-- 5. Функция
CREATE OR REPLACE FUNCTION get_dept_salary_total(p_dept_id NUMBER)
RETURN NUMBER AS
    v_total NUMBER;
BEGIN
    SELECT SUM(salary) INTO v_total
    FROM employees
    WHERE dept_id = p_dept_id;

    RETURN NVL(v_total, 0);
END;
/

-- 6. Пакет
CREATE OR REPLACE PACKAGE hr_utils AS
    PROCEDURE update_salary(p_emp_id NUMBER, p_new_salary NUMBER);
    FUNCTION count_employees(p_dept_id NUMBER) RETURN NUMBER;
END hr_utils;
/

CREATE OR REPLACE PACKAGE BODY hr_utils AS
    PROCEDURE update_salary(p_emp_id NUMBER, p_new_salary NUMBER) AS
    BEGIN
        UPDATE employees SET salary = p_new_salary
        WHERE emp_id = p_emp_id;
        COMMIT;
    END;

    FUNCTION count_employees(p_dept_id NUMBER) RETURN NUMBER AS
        v_count NUMBER;
    BEGIN
        SELECT COUNT(*) INTO v_count
        FROM employees
        WHERE dept_id = p_dept_id;
        RETURN v_count;
    END;
END hr_utils;
/

-- 7. Тестовые данные
INSERT INTO departments VALUES (seq_dept.NEXTVAL, 'IT', 'Moscow', 1000000);
INSERT INTO departments VALUES (seq_dept.NEXTVAL, 'HR', 'St. Petersburg', 500000);

BEGIN
    hire_employee('Ivan', 'Ivanov', 1, 75000);
    hire_employee('Maria', 'Petrova', 1, 80000);
    hire_employee('Alexey', 'Sidorov', 2, 60000);

    INSERT INTO projects VALUES (seq_proj.NEXTVAL, 'New Portal', 1000, SYSDATE, SYSDATE + 180);
    INSERT INTO projects VALUES (seq_proj.NEXTVAL, 'HR System', 1002, SYSDATE, SYSDATE + 90);

    COMMIT;
END;
/

-- Проверка
SELECT 'DEV: Tables created: ' || COUNT(*) || ' tables' AS status
FROM user_tables;