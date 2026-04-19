import random
from datetime import datetime
from pymongo import MongoClient

client = MongoClient('mongodb://localhost:27017/')
db = client['shop_db']

def check_integrity():
    print("\nПроверка целостности:")
    for coll in ['users', 'categories', 'products', 'orders', 'order_items']:
        print(f" - Документов в {coll}: {db[coll].count_documents({})}")

    invalid_orders = list(db.orders.aggregate([
        {'$lookup': {'from': 'users', 'localField': 'user_id', 'foreignField': 'user_id', 'as': 'u'}},
        {'$match': {'u': {'$size': 0}}}
    ]))
    print(f" - Заказов с некорректными user_id: {len(invalid_orders)}")

def get_all_users():
    """Аналог SELECT * FROM users"""
    users = list(db.users.find())
    print(f"\nВсего пользователей: {len(users)}")
    for u in users:
        print(f" - {u['user_id']}: {u['name']} ({u['email']})")

def get_user_orders(user_id: int):
    """Аналог SELECT * FROM orders WHERE user_id = ?"""
    orders = list(db.orders.find({'user_id': user_id}))
    print(f"\nНайдено заказов для пользователя {user_id}: {len(orders)}")
    for o in orders:
        print(f" - Заказ №{o['order_id']} от {o['created_at']} | Статус: {o['status']}")

def get_order_with_details(order_id: int):
    """Аналог JOIN orders + order_items + products"""
    pipeline = [
        {'$match': {'order_id': order_id}},
        {'$lookup': {
            'from': 'order_items',
            'localField': 'order_id',
            'foreignField': 'order_id',
            'as': 'items'
        }},
        {'$unwind': '$items'},
        {'$lookup': {
            'from': 'products',
            'localField': 'items.product_id',
            'foreignField': 'product_id',
            'as': 'items.product_details'
        }},
        {'$unwind': '$items.product_details'},
        {'$group': {
            '_id': '$_id',
            'order_id': {'$first': '$order_id'},
            'status': {'$first': '$status'},
            'full_items': {'$push': {
                'name': '$items.product_details.name',
                'qty': '$items.quantity',
                'price': '$items.price'
            }}
        }}
    ]

    result = list(db.orders.aggregate(pipeline))
    if not result:
        print(f"Заказ {order_id} не найден.")
        return

    res = result[0]
    print(f"\nДетали заказа №{res['order_id']} (Статус: {res['status']}):")
    for item in res['full_items']:
        print(f" - {item['name']}: {item['qty']} шт. x {item['price']}")

def find_expensive_orders():
    print("\nЗаказы на сумму > 1000:")
    pipeline = [
        {'$group': {'_id': '$order_id', 'total': {'$sum': {'$multiply': ['$quantity', '$price']}}}},
        {'$match': {'total': {'$gt': 1000}}}
    ]
    for res in db.order_items.aggregate(pipeline):
        print(f" - ID Заказа: {res['_id']}, Сумма: {res['total']:.2f}")

def show_top_products():
    """Аналог GROUP BY product_id ORDER BY SUM(quantity*price) DESC"""
    limit = int(input("Введите лимит (например, 5): "))
    pipeline = [
        {'$group': {'_id': '$product_id', 'rev': {'$sum': {'$multiply': ['$quantity', '$price']}}}},
        {'$sort': {'rev': -1}},
        {'$limit': limit},
        {'$lookup': {'from': 'products', 'localField': '_id', 'foreignField': 'product_id', 'as': 'info'}},
        {'$unwind': '$info'}
    ]
    for p in db.order_items.aggregate(pipeline):
        print(f"Товар: {p['info']['name']} | Выручка: {p['rev']:.2f}")

def performance_test():
    print("\nТест производительности индекса:")
    count = 100000
    print(f"Генерация {count} тестовых документов...")

    test_orders = []
    for i in range(count):
        test_orders.append({
            "order_id": 10000 + i,
            "user_id": random.randint(1, 10),
            "created_at": datetime.now(),
            "status": "completed",
            "is_test": True
        })

    db.orders.insert_many(test_orders)
    user_to_find = 1

    def run_explain(label):
        print(f"\n--- {label} ---")
        expl = db.orders.find({'user_id': user_to_find}).sort('created_at', -1).explain()
        stats = expl['executionStats']

        print(f"   Время выполнения: {stats['executionTimeMillis']} ms")
        print(f"   Изучено документов: {stats['totalDocsExamined']}")
        print(f"   Тип сканирования: {stats['executionStages']['stage']}")

    db.orders.drop_indexes()
    run_explain("БЕЗ ИНДЕКСА")

    print(f"\nСоздание индекса...")
    db.orders.create_index([('user_id', 1), ('created_at', -1)])
    run_explain("С ИНДЕКСОМ")

    print("\nУдаление тестовых данных...")
    db.orders.delete_many({"is_test": True})
    db.orders.drop_indexes()
    print("Готово. База приведена в исходное состояние.")

def menu():
    while True:
        print("\n1. Проверить целостность и количество")
        print("2. Найти заказы на сумму > 1000")
        print("3. Топ товаров по выручке")
        print("4. Тест производительности (индексы)")
        print("5. Найти все заказы пользователя (по ID)")
        print("6. Показать всех пользователей")
        print("7. Детали заказа (по ID)")
        print("0. Выход")

        choice = input("\nВыберите действие: ")

        match choice:
            case "1":
                check_integrity()

            case "2":
                find_expensive_orders()

            case "3":
                show_top_products()

            case "4":
                performance_test()

            case "5":
                uid = input("Введите ID пользователя: ")

                try:
                    user_id = int(uid)
                    get_user_orders(user_id)
                except ValueError:
                    print("Ошибка: ID должен быть числом")

            case "6":
                get_all_users()

            case "7":
                oid = input("Введете ID заказа: ")

                try:
                    order_id = int(oid)
                    get_order_with_details(order_id)
                except ValueError:
                    print("Ошибка: ID должен быть числом")

            case "0":
                break

            case _:
                print("❌ Неверный ввод! Попробуйте снова.")

if __name__ == "__main__":
    menu()