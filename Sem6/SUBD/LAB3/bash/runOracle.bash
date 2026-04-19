docker run -d --name oracle-xe-lr3 \
  -p 1521:1521 \
  -p 5500:5500 \
  -e ORACLE_PWD=123 \
  -e ORACLE_CHARACTERSET=AL32UTF8 \
  container-registry.oracle.com/database/express:21.3.0-xe

# docker exec -it oracle-xe-lr3 sqlplus sys/123@XE as sysdba

# CREATE USER c##dev_schema IDENTIFIED BY dev123;
# CREATE USER c##prod_schema IDENTIFIED BY prod123;
#
# GRANT CONNECT, RESOURCE TO c##dev_schema, c##prod_schema;
# ALTER USER c##dev_schema QUOTA UNLIMITED ON USERS;
# ALTER USER c##prod_schema QUOTA UNLIMITED ON USERS;