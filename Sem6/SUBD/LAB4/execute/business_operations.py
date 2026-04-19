import json

import redis

r = redis.Redis(
    host='localhost',
    port=6379,
    db=0,
    decode_responses=True
)


def print_menu():
    print("0.  Вывести последние заказы пользователя")
    print("1.  Посчитать топ N популярных товаров по количеству продаж")
    print("2.  Посчитать топ N товаров по выручке")
    print("3.  Найти все товары определённой категории")
    print("4.  Отфильтровать товары по категории и цене")
    print("5.  Посчитать выручку по пользователю")
    print("6.  Найти пересечение покупок двух пользователей")
    print("7.  Найти объединение покупок двух пользователей")
    print("8.  Вывод списка пользователей")
    print("9.  Вывод всех категорий товаров")
    print("q = Выход")


def get_last_user_orders(user_id, limit=10):
    """0. Вывести последние заказы пользователя"""
    # Используем Sorted Set для получения по времени
    order_ids = r.zrevrange(f"user:{user_id}:orders", 0, limit-1, withscores=True)

    if not order_ids:
        print(f"Пользователь {user_id} не найден или нет заказов")
        return []

    orders = []
    for order_id, timestamp in order_ids:
        order_data = r.hgetall(f"order:{order_id}")
        if order_data:
            orders.append({
                'order_id': order_id,
                'created_at': order_data['created_at'],
                'status': order_data['status'],
                'timestamp': timestamp
            })

    print(f"\nПоследние {len(orders)} заказов пользователя {user_id}:")
    for i, order in enumerate(orders, 1):
        print(f"{i}. Заказ #{order['order_id']} - {order['status']} - {order['created_at']}")

    return orders


def get_top_products_by_sales(N):
    top_products = r.zrevrange('products:by_sales', 0, N-1, withscores=True)

    result = []
    for product_id, sales in top_products:
        product_data = r.hgetall(f"product:{product_id}")
        if product_data:
            result.append({
                'id': product_id,
                'name': product_data['name'],
                'sales': float(sales),
                'price': float(product_data['price']),
                'category_id': product_data['category_id']
            })
    return result


def get_top_products_by_revenue(N):
    """Вычисление топа товаров по выручке"""
    top_products = r.zrevrange('products:by_revenue', 0, N-1, withscores=True)

    result = []
    for product_id, revenue in top_products:
        product_data = r.hgetall(f"product:{product_id}")
        if product_data:
            result.append({
                'id': product_id,
                'name': product_data['name'],
                'revenue': float(revenue),
                'price': float(product_data['price']),
                'category_id': product_data['category_id']
            })
    return result


def get_cached_data(cache_key, execute_function, ttl=60):
    """Универсальная функция для получения данных из кэша"""
    cached_data = r.get(cache_key)

    if cached_data is not None:
        print(f"Данные получены из кэша (ключ: {cache_key})")
        return json.loads(cached_data)
    else:
        print(f"Кэш пуст, вычисляем данные...")
        data = execute_function()

        if data:
            r.setex(cache_key, ttl, json.dumps(data))
            print(f"Данные сохранены в кэш на {ttl} секунд")

        return data


def get_top_products_by_sales_cached(N):
    """1. Посчитать топ N популярных товаров по количеству продаж (с кэшированием)"""
    cache_key = f"cache:top_products:by_sales:{N}"

    result = get_cached_data(
        cache_key=cache_key,
        execute_function=lambda: get_top_products_by_sales(N),
        ttl=60
    )

    print(f"\nТоп-{N} товаров по продажам:")
    for i, product in enumerate(result, 1):
        print(f"{i}. {product['name']} - {product['sales']} шт. - {product['price']} руб.")

    return result


def get_top_products_by_revenue_cached(N):
    """2. Посчитать топ N товаров по выручке (с кэшированием)"""
    cache_key = f"cache:top_products:by_revenue:{N}"

    # Получаем данные из кэша или вычисляем
    result = get_cached_data(
        cache_key=cache_key,
        execute_function=lambda: get_top_products_by_revenue(N),
        ttl=60
    )

    print(f"\nТоп-{N} товаров по выручке:")
    for i, product in enumerate(result, 1):
        print(f"{i}. {product['name']} - {product['revenue']:.2f} руб.")

    return result


def get_products_by_category(category_id):
    """3. Найти все товары определённой категории"""
    product_ids = r.smembers(f"category:{category_id}:products")

    if not product_ids:
        print(f"Категория {category_id} не найдена или пуста")
        return []

    category_name = r.hget(f"category:{category_id}", "name")

    products = []
    for p_id in product_ids:
        product_data = r.hgetall(f"product:{p_id}")
        if product_data:
            products.append({
                'id': p_id,
                'name': product_data['name'],
                'price': float(product_data['price'])
            })

    print(f"\nТовары категории '{category_name}' ({category_id}):")
    for i, product in enumerate(products, 1):
        print(f"{i}. {product['name']} - {product['price']} руб.")

    return products


def filter_products_by_category_and_price(category_id, max_price):
    """4. Отфильтровать товары по категории и цене"""
    category_products = r.smembers(f"category:{category_id}:products")

    if not category_products:
        print(f"Категория {category_id} не найдена")
        return []

    filtered = []
    for pid in category_products:
        price = r.zscore('products:by_price', pid)
        if price is not None and float(price) <= max_price:
            product_data = r.hgetall(f"product:{pid}")
            if product_data:
                filtered.append({
                    'id': pid,
                    'name': product_data['name'],
                    'price': float(price),
                    'category_id': category_id
                })

    category_name = r.hget(f"category:{category_id}", "name")
    print(f"\nТовары категории '{category_name}' до {max_price} руб.:")
    for i, product in enumerate(filtered, 1):
        print(f"{i}. {product['name']} - {product['price']} руб.")

    return filtered


def get_user_revenue(user_id):
    """5. Посчитать выручку по пользователю"""
    order_ids = r.zrange(f"user:{user_id}:orders", 0, -1)

    total_revenue = 0.0
    for order_id in order_ids:
        item_ids = r.smembers(f"order:{order_id}:items")

        for item_id in item_ids:
            item_data = r.hgetall(f"order_item:{item_id}")
            if item_data:
                quantity = float(item_data['quantity'])
                price = float(item_data['price'])
                total_revenue += quantity * price

    user_name = r.hget(f"user:{user_id}", "name")

    print(f"\nВыручка по пользователю {user_name} ({user_id}): {total_revenue:.2f} руб.")

    return total_revenue


def get_intersection_purchases(user1_id, user2_id):
    """6. Найти пересечение покупок двух пользователей"""
    keys = [f"user:{user1_id}:purchased", f"user:{user2_id}:purchased"]

    user1_exists = r.exists(f"user:{user1_id}")
    user2_exists = r.exists(f"user:{user2_id}")

    if not user1_exists:
        print(f"Пользователь {user1_id} не найден")
    if not user2_exists:
        print(f"Пользователь {user2_id} не найден")

    if not user1_exists or not user2_exists:
        return []

    intersection = r.sinter(keys)
    print(f"Пересечение покупок user1_id={user1_id} и user2_id={user2_id}:")

    result = []
    for product_id in intersection:
        product_data = r.hgetall(f"product:{product_id}")
        if product_data:
            result.append({
                'id': product_id,
                'name': product_data['name'],
                'price': float(product_data['price'])
            })

            print(f"ID: {product_id}, {product_data['name']} - {product_data['price']}")

    return result


def get_union_purchases(user1_id, user2_id):
    """7. Найти объединение покупок двух пользователей"""
    keys = [f"user:{user1_id}:purchased", f"user:{user2_id}:purchased"]

    user1_exists = r.exists(f"user:{user1_id}")
    user2_exists = r.exists(f"user:{user2_id}")

    if not user1_exists:
        print(f"Пользователь {user1_id} не найден")
    if not user2_exists:
        print(f"Пользователь {user2_id} не найден")

    if not user1_exists or not user2_exists:
        return []

    union = r.sunion(keys)
    print(f"Общие покупки user1_id={user1_id} и user2_id={user2_id}:")

    result = []
    for product_id in union:
        product_data = r.hgetall(f"product:{product_id}")
        if product_data:
            result.append({
                'id': product_id,
                'name': product_data['name'],
                'price': float(product_data['price'])
            })

            print(f"ID: {product_id}, {product_data['name']} - {product_data['price']}")

    return result

def get_all_categories():
    """8. Вывод всех категорий товаров"""
    category_ids = r.lrange('categories:all_categories', 0, -1)

    if not category_ids:
        print("Список категорий пуст")

    print(f"\nВсе категории ({len(category_ids)}):")

    for i, cat_id in enumerate(category_ids, 1):
        category_data = r.hget(f"category:{cat_id}", 'name')
        if category_data:
            print(f"{i}. {category_data} (ID: {cat_id})")


def get_all_users():
    """9. Получить всех пользователей"""
    user_ids = r.lrange('users:all_users', 0, -1)

    if not user_ids:
        print("Нет пользователей")

    print(f"\nВсе пользователи ({len(user_ids)}):")

    for i, uid in enumerate(user_ids, 1):
        user = r.hgetall(f"user:{uid}")
        if user:
            print(f"{i}. {user['name']} - {user['email']} (ID: {uid})")


def main():
    try:
        r.ping()
        print("✓ Подключение к Redis успешно")
    except:
        print("✗ Ошибка подключения к Redis")
        return

    while True:
        print_menu()

        try:
            choice = input("\nВыберите операцию (0-9) or q to exit: ").strip()

            if choice == 'q':
                break

            elif choice == '0':
                user_id = input("Введите ID пользователя: ").strip()
                limit = input("Сколько заказов показать: ").strip()
                limit = int(limit) if limit else 10
                get_last_user_orders(user_id, limit)

            elif choice == '1':
                N = input("Введите N для топа: ").strip()
                N = int(N) if N else 10
                get_top_products_by_sales_cached(N)

            elif choice == '2':
                N = input("Введите N для топа: ").strip()
                N = int(N) if N else 10
                get_top_products_by_revenue_cached(N)

            elif choice == '3':
                category_id = input("Введите ID категории: ").strip()
                get_products_by_category(category_id)

            elif choice == '4':
                category_id = input("Введите ID категории: ").strip()
                max_price = float(input("Введите максимальную цену: ").strip())
                filter_products_by_category_and_price(category_id, max_price)

            elif choice == '5':
                user_id = input("Введите ID пользователя: ").strip()
                get_user_revenue(user_id)

            elif choice == '6':
                user1_id = input("Введите ID первого пользователя: ").strip()
                user2_id = input("Введите ID второго пользователя: ").strip()
                get_intersection_purchases(user1_id, user2_id)

            elif choice == '7':
                user1_id = input("Введите ID первого пользователя: ").strip()
                user2_id = input("Введите ID второго пользователя: ").strip()
                get_union_purchases(user1_id, user2_id)

            elif choice == '8':
                get_all_users()

            elif choice == '9':
                get_all_categories()

            else:
                print("Неверный выбор. Попробуйте снова.", end="\n\n")

            print(end='\n')

        except ValueError as e:
            print(f"Ошибка ввода: {e}", end="\n\n")
        except Exception as e:
            print(f"Ошибка: {e}", end="\n\n")

if __name__ == "__main__":
    main()