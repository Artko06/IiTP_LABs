CREATE TABLE groups (
    id NUMBER NOT NULL,
    name VARCHAR2(50) NOT NULL,
    count_val NUMBER DEFAULT 0
);

CREATE TABLE students (
    id NUMBER NOT NULL,
    student_name VARCHAR2(50) NOT NULL,
    group_id NUMBER
);