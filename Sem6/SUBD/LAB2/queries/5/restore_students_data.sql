CREATE OR REPLACE PROCEDURE RESTORE_TO_TIMESTAMP(
    p_time TIMESTAMP
)
IS
BEGIN
    DBMS_OUTPUT.PUT_LINE('Students at the ' || TO_CHAR(p_time, 'DD.MM.YYYY HH24:MI:SS') || ':');

    FOR record IN (
        WITH last_states AS (
            SELECT
                student_id,
                MAX(change_date) as last_change
            FROM students_logs
            WHERE change_date <= p_time
            GROUP BY student_id
        )
        SELECT
            l.student_id,
            s.new_name as name,
            s.new_group_id as group_id
        FROM last_states l
        JOIN students_logs s ON l.student_id = s.student_id
            AND l.last_change = s.change_date
        WHERE s.action_type != 'DELETE'
        ORDER BY l.student_id
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('ID: ' || record.student_id ||
                           ', name: ' || record.name ||
                           ', group: ' || NVL(TO_CHAR(record.group_id), 'NULL'));
    END LOOP;
END restore_to_timestamp;

CREATE OR REPLACE PROCEDURE RESTORE_BY_OFFSET(
    p_minutes NUMBER
)
IS
    v_time TIMESTAMP;
BEGIN
    v_time := SYSTIMESTAMP - NUMTODSINTERVAL(p_minutes, 'MINUTE');

    RESTORE_TO_TIMESTAMP(v_time);
END RESTORE_BY_OFFSET;
