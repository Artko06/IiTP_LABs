import csv
from datetime import datetime
from pymongo import MongoClient

client = MongoClient('mongodb://localhost:27017/')
db = client['shop_db']

def parse_csv(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return list(csv.DictReader(file))

def migrate_data():
    collections = ['users', 'categories', 'products', 'orders', 'order_items']
    for coll in collections:
        db[coll].drop()

    categories = parse_csv('../data_csv/categoryid-name.csv')
    for c in categories:
        c['category_id'] = int(c['category_id'])
    db.categories.insert_many(categories)

    products = parse_csv('../data_csv/productid-name-categoryid-price.csv')
    for p in products:
        p['product_id'] = int(p['product_id'])
        p['category_id'] = int(p['category_id'])
        p['price'] = float(p['price'])
    db.products.insert_many(products)

    users = parse_csv('../data_csv/userid-name-email-createdat.csv')
    for u in users:
        u['user_id'] = int(u['user_id'])
        u['created_at'] = datetime.strptime(u['created_at'], '%Y-%m-%d %H:%M:%S')
    db.users.insert_many(users)

    orders = parse_csv('../data_csv/orderid-userid-createdat-status.csv')
    for o in orders:
        o['order_id'] = int(o['order_id'])
        o['user_id'] = int(o['user_id'])
        o['created_at'] = datetime.strptime(o['created_at'], '%Y-%m-%d %H:%M:%S')
    db.orders.insert_many(orders)

    items = parse_csv('../data_csv/orderitemid-orderid-productid-quantity-price.csv')
    for i in items:
        i['order_item_id'] = int(i['order_item_id'])
        i['order_id'] = int(i['order_id'])
        i['product_id'] = int(i['product_id'])
        i['quantity'] = int(i['quantity'])
        i['price'] = float(i['price'])
    db.order_items.insert_many(items)

    print("Миграция завершена успешно")

if __name__ == "__main__":
    migrate_data()