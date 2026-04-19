CREATE OR REPLACE FUNCTION CALCULATE_ANNUAL_REWARD(p_salary NUMBER, p_bonus_percent NUMBER)
RETURN NUMBER IS
    v_error_null_salary CONSTANT NUMBER := -20001;
    v_error_null_percent CONSTANT NUMBER := -20002;
    v_error_negative_salary CONSTANT NUMBER := -20003;
    v_error_negative_percent CONSTANT NUMBER := -20004;

    v_total_reward NUMBER;
BEGIN
    IF p_salary IS NULL THEN
        RAISE_APPLICATION_ERROR(v_error_null_salary, 'Salary cannot be NULL');
    END IF;

    IF p_bonus_percent IS NULL THEN
        RAISE_APPLICATION_ERROR(v_error_null_percent, 'Percent cannot be NULL');
    END IF;

    IF p_salary < 0 THEN
        RAISE_APPLICATION_ERROR(v_error_negative_salary, 'Salary must not be negative');
    END IF;

    IF p_bonus_percent < 0 THEN
        RAISE_APPLICATION_ERROR(v_error_negative_percent, 'The bonus percentage cannot be negative');
    END IF;

    v_total_reward := (1 + p_bonus_percent / 100) * 12 * p_salary;
    RETURN v_total_reward;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('ERROR: ' || SQLERRM);
        RETURN null;
END CALCULATE_ANNUAL_REWARD;

SELECT 'NULL, NULL' AS parameters,
       calculate_annual_reward(NULL, NULL) AS result
FROM dual
UNION ALL
SELECT 'NULL, 5',
       calculate_annual_reward(NULL, 5)
FROM dual
UNION ALL
SELECT '1000, NULL',
       calculate_annual_reward(1000, NULL)
FROM dual
UNION ALL
SELECT '-1, 5',
       calculate_annual_reward(-1, 5)
FROM dual
UNION ALL
SELECT '0, 5',
       calculate_annual_reward(0, 5)
FROM dual
UNION ALL
SELECT '4800, 12',
       calculate_annual_reward(4800, 12)
FROM dual
UNION ALL
SELECT '1600, 5.5',
       calculate_annual_reward(1600, 5.5)
FROM dual;