-- 1. Функция для получения DDL объектов
CREATE OR REPLACE FUNCTION get_object_ddl(
    p_owner IN VARCHAR2,
    p_object_name IN VARCHAR2,
    p_object_type IN VARCHAR2
) RETURN CLOB IS
    v_ddl CLOB;
BEGIN
    IF p_object_type = 'PACKAGE BODY' THEN
        SELECT DBMS_METADATA.GET_DDL('PACKAGE_BODY', p_object_name, p_owner)
        INTO v_ddl
        FROM dual;
    ELSIF p_object_type = 'TABLE' THEN
        SELECT DBMS_METADATA.GET_DDL('TABLE', p_object_name, p_owner)
        INTO v_ddl
        FROM dual;
    ELSIF p_object_type IN ('PROCEDURE', 'FUNCTION', 'PACKAGE', 'INDEX', 'SEQUENCE') THEN
        SELECT DBMS_METADATA.GET_DDL(p_object_type, p_object_name, p_owner)
        INTO v_ddl
        FROM dual;
    ELSE
        v_ddl := '-- Неподдерживаемый тип объекта: ' || p_object_type;
    END IF;
    
    RETURN v_ddl;
EXCEPTION
    WHEN OTHERS THEN
        RETURN '-- Ошибка получения DDL для ' || p_object_name || ': ' || SQLERRM;
END get_object_ddl;
/

-- 2. Процедура сравнения схем
CREATE OR REPLACE PROCEDURE compare_schemas_complete(
    p_dev_schema_name  IN VARCHAR2 DEFAULT USER,
    p_prod_schema_name IN VARCHAR2 DEFAULT USER
) IS
    TYPE t_diff_rec IS RECORD (
        object_name VARCHAR2(128),
        object_type VARCHAR2(32),
        diff_type   VARCHAR2(20)
    );
    
    TYPE t_diff_table IS TABLE OF t_diff_rec;
    v_diffs t_diff_table := t_diff_table();
    
    v_output CLOB;

    FUNCTION get_object_count(p_owner VARCHAR2, p_type VARCHAR2) RETURN NUMBER IS
        v_count NUMBER := 0;
    BEGIN
        SELECT COUNT(*)
        INTO v_count
        FROM all_objects
        WHERE owner = UPPER(p_owner)
          AND object_type = p_type;
        RETURN v_count;
    EXCEPTION
        WHEN OTHERS THEN RETURN 0;
    END;
    
BEGIN
    v_output := v_output || 'Dev схема: ' || p_dev_schema_name || CHR(10);
    v_output := v_output || 'Prod схема: ' || p_prod_schema_name || CHR(10);
    v_output := v_output || '=' || RPAD('=', 50, '=') || CHR(10);
    
    -- 1. СРАВНЕНИЕ ТАБЛИЦ
    v_output := v_output || CHR(10) || '1. ТАБЛИЦЫ:' || CHR(10);
    v_output := v_output || 'Dev: ' || get_object_count(p_dev_schema_name, 'TABLE') || ' таблиц' || CHR(10);
    v_output := v_output || 'Prod: ' || get_object_count(p_prod_schema_name, 'TABLE') || ' таблиц' || CHR(10);

    -- Таблицы только в Dev
    FOR rec IN (
        SELECT table_name, 'TABLE' as object_type, 'ONLY_IN_DEV' as diff_type
        FROM all_tables
        WHERE owner = UPPER(p_dev_schema_name)
        MINUS
        SELECT table_name, 'TABLE', 'ONLY_IN_DEV'
        FROM all_tables
        WHERE owner = UPPER(p_prod_schema_name)
    ) LOOP
        v_diffs.EXTEND;
        v_diffs(v_diffs.LAST) := rec;
        v_output := v_output || '   + ' || rec.table_name || ' (только в Dev)' || CHR(10);
    END LOOP;
    
    -- Таблицы только в Prod
    FOR rec IN (
        SELECT table_name, 'TABLE' as object_type, 'ONLY_IN_PROD' as diff_type
        FROM all_tables
        WHERE owner = UPPER(p_prod_schema_name)
        MINUS
        SELECT table_name, 'TABLE', 'ONLY_IN_PROD'
        FROM all_tables
        WHERE owner = UPPER(p_dev_schema_name)
    ) LOOP
        v_diffs.EXTEND;
        v_diffs(v_diffs.LAST) := rec;
        v_output := v_output || '   - ' || rec.table_name || ' (только в Prod, нужно удалить)' || CHR(10);
    END LOOP;
    
    -- 2. СРАВНЕНИЕ ИНДЕКСОВ
    v_output := v_output || CHR(10) || '2. ИНДЕКСЫ:' || CHR(10);
    
    -- Индексы только в Dev
    FOR rec IN (
        SELECT index_name, 'INDEX' as object_type, 'ONLY_IN_DEV' as diff_type
        FROM all_indexes
        WHERE owner = UPPER(p_dev_schema_name)
          AND index_name NOT LIKE 'SYS_%'
        MINUS
        SELECT index_name, 'INDEX', 'ONLY_IN_DEV'
        FROM all_indexes
        WHERE owner = UPPER(p_prod_schema_name)
          AND index_name NOT LIKE 'SYS_%'
    ) LOOP
        v_diffs.EXTEND;
        v_diffs(v_diffs.LAST) := rec;
        v_output := v_output || '   + ' || rec.index_name || ' (только в Dev)' || CHR(10);
    END LOOP;
    
    -- 3. СРАВНЕНИЕ ПРОЦЕДУР И ФУНКЦИЙ
    v_output := v_output || CHR(10) || '3. ПРОЦЕДУРЫ/ФУНКЦИИ:' || CHR(10);
    
    DECLARE
        TYPE t_obj_types IS TABLE OF VARCHAR2(30);
        v_obj_types t_obj_types := t_obj_types('PROCEDURE', 'FUNCTION', 'PACKAGE', 'PACKAGE BODY');
    BEGIN
        FOR i IN 1..v_obj_types.COUNT LOOP
            FOR rec IN (
                SELECT object_name, v_obj_types(i) as object_type, 'ONLY_IN_DEV' as diff_type
                FROM all_objects
                WHERE owner = UPPER(p_dev_schema_name)
                  AND object_type = v_obj_types(i)
                  AND object_name NOT LIKE 'BIN$%'
                MINUS
                SELECT object_name, v_obj_types(i), 'ONLY_IN_DEV'
                FROM all_objects
                WHERE owner = UPPER(p_prod_schema_name)
                  AND object_type = v_obj_types(i)
            ) LOOP
                v_diffs.EXTEND;
                v_diffs(v_diffs.LAST) := rec;
                v_output := v_output || '   + ' || rec.object_name || 
                           ' (' || rec.object_type || ', только в Dev)' || CHR(10);
            END LOOP;
        END LOOP;
    END;
    
    -- 4. СРАВНЕНИЕ ПОСЛЕДОВАТЕЛЬНОСТЕЙ
    v_output := v_output || CHR(10) || '4. ПОСЛЕДОВАТЕЛЬНОСТИ:' || CHR(10);
    
    FOR rec IN (
        SELECT sequence_name, 'SEQUENCE' as object_type, 'ONLY_IN_DEV' as diff_type
        FROM all_sequences
        WHERE sequence_owner = UPPER(p_dev_schema_name)
        MINUS
        SELECT sequence_name, 'SEQUENCE', 'ONLY_IN_DEV'
        FROM all_sequences
        WHERE sequence_owner = UPPER(p_prod_schema_name)
    ) LOOP
        v_diffs.EXTEND;
        v_diffs(v_diffs.LAST) := rec;
        v_output := v_output || '   + ' || rec.sequence_name || ' (только в Dev)' || CHR(10);
    END LOOP;
    
    -- 5. ОБЩИЕ РАЗЛИЧИЯ
    v_output := v_output || CHR(10) || '5. ОБЩИЕ РАЗЛИЧИЯ:' || CHR(10);
    v_output := v_output || 'Всего различий: ' || v_diffs.COUNT || CHR(10);

    DBMS_OUTPUT.PUT_LINE(v_output);
    
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка при сравнении схем: ' || SQLERRM);
END compare_schemas_complete;
/

-- 3. Процедура миграционного скрипта
CREATE OR REPLACE PROCEDURE generate_migration_script(
    p_dev_schema IN VARCHAR2 DEFAULT USER,
    p_prod_schema IN VARCHAR2 DEFAULT USER
) IS
    v_script CLOB := EMPTY_CLOB();
    
    -- Функция для добавления строки в скрипт
    PROCEDURE add_line(p_text VARCHAR2) IS
    BEGIN
        v_script := v_script || p_text || CHR(10);
    END add_line;
    
BEGIN
    add_line('-- ============================================');
    add_line('-- СКРИПТ МИГРАЦИИ');
    add_line('-- Dev: ' || p_dev_schema);
    add_line('-- Prod: ' || p_prod_schema);
    add_line('-- ============================================');
    add_line('');
    
    -- 1. УДАЛЕНИЕ ЛИШНИХ ОБЪЕКТОВ В PROD
    add_line('-- 1. УДАЛЕНИЕ ОБЪЕКТОВ, ОТСУТСТВУЮЩИХ В DEV');
    
    -- Таблицы
    FOR rec IN (
        SELECT table_name
        FROM all_tables
        WHERE owner = UPPER(p_prod_schema)
        MINUS
        SELECT table_name
        FROM all_tables
        WHERE owner = UPPER(p_dev_schema)
    ) LOOP
        add_line('DROP TABLE ' || rec.table_name || ' CASCADE CONSTRAINTS;');
    END LOOP;
    
    -- Процедуры/Функции
    FOR rec IN (
        SELECT object_name, object_type
        FROM all_objects
        WHERE owner = UPPER(p_prod_schema)
          AND object_type IN ('PROCEDURE', 'FUNCTION')
        MINUS
        SELECT object_name, object_type
        FROM all_objects
        WHERE owner = UPPER(p_dev_schema)
          AND object_type IN ('PROCEDURE', 'FUNCTION')
    ) LOOP
        add_line('DROP ' || rec.object_type || ' ' || rec.object_name || ';');
    END LOOP;
    
    -- Пакеты
    FOR rec IN (
        SELECT DISTINCT object_name
        FROM all_objects
        WHERE owner = UPPER(p_prod_schema)
          AND object_type LIKE 'PACKAGE%'
        MINUS
        SELECT DISTINCT object_name
        FROM all_objects
        WHERE owner = UPPER(p_dev_schema)
          AND object_type LIKE 'PACKAGE%'
    ) LOOP
        add_line('DROP PACKAGE ' || rec.object_name || ';');
    END LOOP;
    
    -- Последовательности
    FOR rec IN (
        SELECT sequence_name
        FROM all_sequences
        WHERE sequence_owner = UPPER(p_prod_schema)
        MINUS
        SELECT sequence_name
        FROM all_sequences
        WHERE sequence_owner = UPPER(p_dev_schema)
    ) LOOP
        add_line('DROP SEQUENCE ' || rec.sequence_name || ';');
    END LOOP;
    
    add_line('');
    
    -- 2. СОЗДАНИЕ НОВЫХ ОБЪЕКТОВ
    add_line('-- 2. СОЗДАНИЕ НОВЫХ ТАБЛИЦ');
    
    -- Таблицы только в Dev
    FOR rec IN (
        SELECT table_name
        FROM all_tables
        WHERE owner = UPPER(p_dev_schema)
        MINUS
        SELECT table_name
        FROM all_tables
        WHERE owner = UPPER(p_prod_schema)
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.table_name, 'TABLE'));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для таблицы ' || rec.table_name);
        END;
    END LOOP;
    
    -- 3. СОЗДАНИЕ/ОБНОВЛЕНИЕ ПРОЦЕДУР И ФУНКЦИЙ
    add_line('-- 3. ПРОЦЕДУРЫ И ФУНКЦИИ');
    
    FOR rec IN (
        SELECT object_name, object_type
        FROM all_objects
        WHERE owner = UPPER(p_dev_schema)
          AND object_type IN ('PROCEDURE', 'FUNCTION')
        ORDER BY object_type, object_name
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.object_name, rec.object_type));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для ' || rec.object_type || ' ' || rec.object_name);
        END;
    END LOOP;
    
    -- 4. СОЗДАНИЕ/ОБНОВЛЕНИЕ ПАКЕТОВ
    add_line('-- 4. ПАКЕТЫ');
    
    -- Спецификации пакетов
    FOR rec IN (
        SELECT object_name
        FROM all_objects
        WHERE owner = UPPER(p_dev_schema)
          AND object_type = 'PACKAGE'
        ORDER BY object_name
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.object_name, 'PACKAGE'));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для пакета ' || rec.object_name);
        END;
    END LOOP;
    
    -- Тела пакетов
    FOR rec IN (
        SELECT object_name
        FROM all_objects
        WHERE owner = UPPER(p_dev_schema)
          AND object_type = 'PACKAGE BODY'
        ORDER BY object_name
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.object_name, 'PACKAGE BODY'));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для тела пакета ' || rec.object_name);
        END;
    END LOOP;
    
    -- 5. СОЗДАНИЕ ПОСЛЕДОВАТЕЛЬНОСТЕЙ
    add_line('-- 5. ПОСЛЕДОВАТЕЛЬНОСТИ');
    
    FOR rec IN (
        SELECT sequence_name
        FROM all_sequences
        WHERE sequence_owner = UPPER(p_dev_schema)
        ORDER BY sequence_name
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.sequence_name, 'SEQUENCE'));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для последовательности ' || rec.sequence_name);
        END;
    END LOOP;
    
    -- 6. СОЗДАНИЕ ИНДЕКСОВ (кроме PK и системных)
    add_line('-- 6. ИНДЕКСЫ');
    
    FOR rec IN (
        SELECT index_name
        FROM all_indexes
        WHERE owner = UPPER(p_dev_schema)
          AND index_name NOT LIKE 'SYS_%'
          AND index_name NOT LIKE '%_PK'
        ORDER BY table_name, index_name
    ) LOOP
        BEGIN
            add_line(get_object_ddl(p_dev_schema, rec.index_name, 'INDEX'));
            add_line('/');
            add_line('');
        EXCEPTION
            WHEN OTHERS THEN
                add_line('-- Ошибка при получении DDL для индекса ' || rec.index_name);
        END;
    END LOOP;

    DBMS_OUTPUT.PUT_LINE(v_script);
    
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка при генерации скрипта: ' || SQLERRM);
END generate_migration_script;
/

-- 4. Процедура для определения порядка таблиц с учетом FK
CREATE OR REPLACE PROCEDURE get_table_creation_order(
    p_schema_name IN VARCHAR2 DEFAULT USER
) IS
    TYPE t_dep_table IS TABLE OF VARCHAR2(128) INDEX BY PLS_INTEGER;
    v_ordered t_dep_table;
    v_index PLS_INTEGER := 1;
    
    CURSOR c_tables IS
        SELECT table_name
        FROM all_tables
        WHERE owner = UPPER(p_schema_name)
        ORDER BY table_name;
        
    CURSOR c_dependencies(p_table VARCHAR2) IS
        SELECT uc.table_name
        FROM all_constraints uc
        JOIN all_constraints pk ON uc.r_constraint_name = pk.constraint_name
        WHERE uc.owner = UPPER(p_schema_name)
          AND uc.constraint_type = 'R'
          AND pk.table_name = p_table
          AND pk.owner = UPPER(p_schema_name);
BEGIN
    DBMS_OUTPUT.PUT_LINE('=== ПОРЯДОК СОЗДАНИЯ ТАБЛИЦ ДЛЯ СХЕМЫ ' || p_schema_name || ' ===');

    FOR table_rec IN c_tables LOOP
        OPEN c_dependencies(table_rec.table_name);
        FETCH c_dependencies INTO v_ordered(v_index);
        
        IF c_dependencies%NOTFOUND THEN
            DBMS_OUTPUT.PUT_LINE(v_index || '. ' || table_rec.table_name || ' (без внешних ключей)');
            v_index := v_index + 1;
        END IF;
        CLOSE c_dependencies;
    END LOOP;

    FOR table_rec IN c_tables LOOP
        OPEN c_dependencies(table_rec.table_name);
        FETCH c_dependencies INTO v_ordered(v_index);
        
        IF c_dependencies%FOUND THEN
            DBMS_OUTPUT.PUT_LINE(v_index || '. ' || table_rec.table_name || ' (с внешними ключами)');
            v_index := v_index + 1;
        END IF;
        CLOSE c_dependencies;
    END LOOP;
    
    DBMS_OUTPUT.PUT_LINE('==============================================');
    
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Внимание: возможны циклические зависимости!');
        DBMS_OUTPUT.PUT_LINE('Рекомендуется создавать таблицы вручную.');
END get_table_creation_order;
/

BEGIN
    -- 1. Сравнение схем
    compare_schemas_complete('C##DEV_SCHEMA', 'C##PROD_SCHEMA');
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('=== СКРИПТ МИГРАЦИИ ===');
    DBMS_OUTPUT.PUT_LINE('');
    
    -- 2. Скрипта миграции
    generate_migration_script('C##DEV_SCHEMA', 'C##PROD_SCHEMA');
    
    DBMS_OUTPUT.PUT_LINE('');
    DBMS_OUTPUT.PUT_LINE('=== ПОРЯДОК СОЗДАНИЯ ТАБЛИЦ ===');
    DBMS_OUTPUT.PUT_LINE('');
    
    -- 3. Порядок создания таблиц
    get_table_creation_order('C##DEV_SCHEMA');
END;
/