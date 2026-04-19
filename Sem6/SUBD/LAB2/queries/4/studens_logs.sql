CREATE TABLE students_logs (
    audit_id NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    student_id NUMBER,
    action_type VARCHAR2(10),
    old_name VARCHAR2(50),
    new_name VARCHAR2(50),
    old_group_id NUMBER,
    new_group_id NUMBER,
    changed_by VARCHAR2(100),
    change_date TIMESTAMP DEFAULT SYSTIMESTAMP
);

CREATE OR REPLACE TRIGGER STUDENTS_LOGS
AFTER INSERT OR UPDATE OR DELETE ON students
FOR EACH ROW
BEGIN
    IF INSERTING THEN
        INSERT INTO students_logs (
            student_id, action_type,
            old_name, new_name,
            old_group_id, new_group_id,
            changed_by
        ) VALUES (
            :NEW.id, 'INSERT',
            NULL, :NEW.student_name,
            NULL, :NEW.group_id,
            USER
        );
    ELSIF UPDATING THEN
        INSERT INTO students_logs (
            student_id, action_type,
            old_name, new_name,
            old_group_id, new_group_id,
            changed_by
        ) VALUES (
            :NEW.id, 'UPDATE',
            :OLD.student_name, :NEW.student_name,
            :OLD.group_id, :NEW.group_id,
            USER
        );
    ELSE
        INSERT INTO students_logs (
            student_id, action_type,
            old_name, new_name,
            old_group_id, new_group_id,
            changed_by
        ) VALUES (
            :OLD.id, 'DELETE',
            :OLD.student_name, NULL,
            :OLD.group_id, NULL,
            USER
        );
    END IF;
END STUDENTS_LOGS;

