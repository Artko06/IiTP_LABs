import csv
import redis
from datetime import datetime

r = redis.Redis(
    host='localhost',
    port=6379,
    db=0,
    decode_responses=True
)

def parse_csv(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        return list(reader)

def migrate_categories():
    categories = parse_csv('../data_csv/categoryid-name.csv')
    for category in categories:
        category_id = category['category_id']

        key = f"category:{category['category_id']}"
        r.hset(key, mapping={
            'name': category['name']
        })

        r.lpush('categories:all_categories', category_id)

def migrate_products():
    products = parse_csv('../data_csv/productid-name-categoryid-price.csv')
    for product in products:
        key = f"product:{product['product_id']}"
        r.hset(key, mapping={
            'name': product['name'],
            'category_id': product['category_id'],
            'price': product['price']
        })

        r.sadd(f"category:{product['category_id']}:products", product['product_id'])

        r.zadd('products:by_price', {product['product_id']: float(product['price'])})

        r.zadd('products:by_sales', {product['product_id']: 0})
        r.zadd('products:by_revenue', {product['product_id']: 0})

def migrate_users():
    users = parse_csv('../data_csv/userid-name-email-createdat.csv')
    for user in users:
        user_id = user['user_id']

        key = f"user:{user_id}"
        r.hset(key, mapping={
            'name': user['name'],
            'email': user['email'],
            'created_at': user['created_at']
        })

        r.lpush('users:all_users', user_id)

def migrate_orders():
    orders = parse_csv('../data_csv/orderid-userid-createdat-status.csv')
    sorted_orders = sorted(orders, key=lambda order: order['created_at'])
    for sort_order in sorted_orders:
        key = f"order:{sort_order['order_id']}"
        r.hset(key, mapping={
            'user_id': sort_order['user_id'],
            'created_at': sort_order['created_at'],
            'status': sort_order['status']
        })

        try:
            dt = datetime.strptime(sort_order['created_at'], '%Y-%m-%d %H:%M:%S')
            timestamp = int(dt.timestamp())
        except:
            timestamp = 0

        r.lpush(f"orders:latest_orders", sort_order['order_id'])

        r.zadd(f"user:{sort_order['user_id']}:orders", {sort_order['order_id']: timestamp})

def migrate_order_items():
    order_items = parse_csv('../data_csv/orderitemid-orderid-productid-quantity-price.csv')

    for item in order_items:
        key = f"order_item:{item['order_item_id']}"
        r.hset(key, mapping={
            'order_id': item['order_id'],
            'product_id': item['product_id'],
            'quantity': item['quantity'],
            'price': item['price']
        })

        r.sadd(f"order:{item['order_id']}:items", item['order_item_id'])

        order_key = f"order:{item['order_id']}"
        user_id = r.hget(order_key, 'user_id')
        if user_id:
            r.sadd(f"user:{user_id}:purchased", item['product_id'])

        r.zincrby('products:by_sales', float(item['quantity']), item['product_id'])

        revenue = float(item['quantity']) * float(item['price'])
        r.zincrby('products:by_revenue', revenue, item['product_id'])

def main():
    r.flushdb()

    migrate_categories()
    migrate_products()
    migrate_users()
    migrate_orders()
    migrate_order_items()

    print(f"Количество пользователей: {len(r.keys('user:*'))}")
    print(f"Количество товаров: {len(r.keys('product:*'))}")
    print(f"Количество заказов: {len(r.keys('order:*'))}")
    print(f"Количество заказанных товаров: {len(r.keys('order_item:*'))}")
    print(f"Количество категорий: {len(r.keys('category:*'))}")

if __name__ == "__main__":
    main()