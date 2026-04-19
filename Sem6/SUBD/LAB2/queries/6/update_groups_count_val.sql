CREATE OR REPLACE TRIGGER UPDATE_GROUPS_COUNT_VAL
AFTER INSERT OR UPDATE OR DELETE ON STUDENTS
BEGIN
    FOR grp IN (SELECT id FROM groups) LOOP
        UPDATE groups
        SET count_val = (
            SELECT COUNT(*)
            FROM students s
            WHERE s.group_id = grp.id
        )
        WHERE groups.id = grp.id;
    END LOOP;
END UPDATE_GROUPS_COUNT_VAL;