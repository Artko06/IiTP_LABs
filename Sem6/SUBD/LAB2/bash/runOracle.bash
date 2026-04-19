docker run -d --name oracle-xe \
  -p 1521:1521 \
  -e ORACLE_PWD=123 \
  container-registry.oracle.com/database/express:21.3.0-xe
