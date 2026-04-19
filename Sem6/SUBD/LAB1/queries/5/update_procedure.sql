CREATE OR REPLACE PROCEDURE UPDATE_MYTABLE(p_id NUMBER, p_val NUMBER) IS
    v_exists NUMBER(1);
BEGIN
    SELECT COUNT(*) INTO v_exists FROM MyTable
    WHERE p_id = id;

    IF v_exists = 1 THEN
        UPDATE MyTable SET val = p_val
        WHERE id = p_id;

        COMMIT;
        DBMS_OUTPUT.PUT_LINE('Successful update: id = ' || p_id || ' new value = ' || p_val);
    ELSE
        DBMS_OUTPUT.PUT_LINE('ERROR update: id = ' || p_id || ' not found!');
    END IF;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('ERROR: ' || SQLERRM);
END UPDATE_MYTABLE;

CALL UPDATE_MYTABLE(-1, 1000);
CALL UPDATE_MYTABLE(0, 1000);
CALL UPDATE_MYTABLE(1, 1000);
CALL UPDATE_MYTABLE(10000, 1000);
CALL UPDATE_MYTABLE(10001, 1000);