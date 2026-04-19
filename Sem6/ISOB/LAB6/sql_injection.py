import sqlite3

connection = sqlite3.connect(':memory:')
cursor = connection.cursor()

cursor.execute("CREATE TABLE users (id INTEGER, username TEXT, password TEXT)")
cursor.execute("INSERT INTO users VALUES (1, 'admin', 'password123')")
connection.commit()

def vulnerable_login(user_input):
    query = f"SELECT * FROM users WHERE username = '{user_input}'"
    print(f"Выполняемый SQL: {query}")
    return cursor.execute(query).fetchall()

def secure_login(user_input):
    query = "SELECT * FROM users WHERE username = ?"
    print(f"Выполняемый SQL: {query} | Параметр: {user_input}")
    return cursor.execute(query, (user_input,)).fetchall()

hacker_input = "' OR '1'='1"
hacker_input2 = "admin'--"

print("--- Попытка через уязвимый код ---")
result_unsafe = vulnerable_login(hacker_input2)
print(f"Результат (найдено строк): {len(result_unsafe)}")
print(f"Полученные данные: {result_unsafe}")
print("\nДанные пользователей:")
for row in result_unsafe:
    print(f"  ID: {row[0]}, Username: {row[1]}, Password: {row[2]}")

print("\n--- Попытка через защищенный код ---")
result_safe = secure_login(hacker_input)
print(f"Результат (найдено строк): {len(result_safe)}")
print(f"Полученные данные: {result_safe}")

connection.close()