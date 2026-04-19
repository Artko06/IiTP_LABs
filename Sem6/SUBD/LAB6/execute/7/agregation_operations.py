import threading
import time
from datetime import datetime

from pymongo import MongoClient, errors

client = MongoClient('mongodb://localhost:27017/')
db = client['shop_db']

def get_monthly_revenue():
    """1.a Ежемесячная выручка по категориям"""
    print("\n--- Ежемесячная выручка по категориям ---")
    pipeline = [
        {"$lookup": {
            "from": "products",
            "localField": "product_id",
            "foreignField": "product_id",
            "as": "p"
        }},
        {"$unwind": "$p"},
        {"$lookup": {
            "from": "orders",
            "localField": "order_id",
            "foreignField": "order_id",
            "as": "o"
        }},
        {"$unwind": "$o"},
        {"$group": {
            "_id": {
                "year": {"$year": "$o.created_at"},
                "month": {"$month": "$o.created_at"},
                "cat": "$p.category_id"
            },
            "revenue": {"$sum": {"$multiply": ["$quantity", "$price"]}}
        }},
        {"$sort": {"_id.year": -1, "_id.month": -1, "revenue": -1}}
    ]
    for res in db.order_items.aggregate(pipeline):
        print(f"Период: {res['_id']['month']}/{res['_id']['year']} | Кат: {res['_id']['cat']} | Сумма: {res['revenue']:.2f}")

def market_basket_analysis():
    """1.b Поиск пар товаров, которые часто покупают вместе"""
    print("\n--- Корзиночный анализ (Топ пар товаров) ---")
    pipeline = [
        {"$group": {"_id": "$order_id", "items": {"$push": "$product_id"}}},
        {"$match": {"items.1": {"$exists": True}}},
        {"$unwind": "$items"},
        {"$lookup": {
            "from": "order_items",
            "localField": "_id",
            "foreignField": "order_id",
            "as": "others"
        }},
        {"$unwind": "$others"},
        {"$match": {"$expr": {"$lt": ["$items", "$others.product_id"]}}},
        {"$group": {
            "_id": {"p1": "$items", "p2": "$others.product_id"},
            "count": {"$sum": 1}
        }},
        {"$sort": {"count": -1}},
        {"$limit": 5}
    ]
    for res in db.order_items.aggregate(pipeline):
        print(f"Пара товаров: {res['_id']['p1']} и {res['_id']['p2']} | Куплено вместе: {res['count']} раз")

def rfm_analysis():
    """1.c RFM-анализ (Recency, Frequency, Monetary)"""
    print("\n--- RFM-анализ клиентов ---")
    now = datetime.now()
    pipeline = [
        {"$lookup": {
            "from": "order_items",
            "localField": "order_id",
            "foreignField": "order_id",
            "as": "items"
        }},
        {"$unwind": "$items"},
        {"$group": {
            "_id": "$user_id",
            "last_order": {"$max": "$created_at"},
            "order_count": {"$addToSet": "$order_id"},
            "total_spent": {"$sum": {"$multiply": ["$items.quantity", "$items.price"]}}
        }},
        {"$project": {
            "user_id": "$_id",
            "recency": {"$dateDiff": {"startDate": "$last_order", "endDate": now, "unit": "day"}},
            "frequency": {"$size": "$order_count"},
            "monetary": "$total_spent"
        }},
        {"$sort": {"monetary": -1}}
    ]
    for res in db.orders.aggregate(pipeline):
        print(f"User {res['user_id']}: R={res['recency']} дн, F={res['frequency']} зак, M={res['monetary']:.2f}")


def performance_mv_test():
    """2. Анализ производительности: Живая агрегация vs Materialized View"""
    print("\n--- Тест Materialized View ($merge) ---")

    mv_pipeline = [
        {"$group": {"_id": "$product_id", "total_qty": {"$sum": "$quantity"}}},
        {"$merge": {"into": "product_sales_report", "whenMatched": "replace"}}
    ]

    start = time.perf_counter()
    list(db.order_items.aggregate([{"$group": {"_id": "$product_id", "total_qty": {"$sum": "$quantity"}}}]))
    live_time = time.perf_counter() - start

    db.order_items.aggregate(mv_pipeline)

    start = time.perf_counter()
    list(db.product_sales_report.find())
    mv_time = time.perf_counter() - start

    print(f"Время 'живого' расчета: {live_time*1000:.2f} ms")
    print(f"Время чтения из коллекции (MV): {mv_time*1000:.2f} ms")


def safe_order_processing():
    """3. Реализация бизнес-процесса: Подтверждение заказа и списание остатка"""
    print("\n--- Бизнес-процесс: Обработка заказа (Атомарно) ---")

    db.inventory.update_one(
        {"p_id": 101},
        {"$setOnInsert": {"product_name": "Ноутбук", "stock": 5}},
        upsert=True
    )

    order_id = 1
    product_id = 101
    needed_qty = 1

    stock_update = db.inventory.update_one(
        {"p_id": product_id, "stock": {"$gte": needed_qty}},
        {"$inc": {"stock": -needed_qty}}
    )

    if stock_update.modified_count > 0:
        db.orders.update_one(
            {"order_id": order_id},
            {"$set": {"status": "shipped", "shipped_at": datetime.now()}}
        )

        db.order_history.insert_one({
            "order_id": order_id,
            "event": "STOCK_RESERVED_AND_SHIPPED",
            "qty": needed_qty,
            "ts": datetime.now()
        })
        print(f" ✅ Товар зарезервирован, заказ №{order_id} переведен в доставку.")
    else:
        print(f" ❌ Ошибка: Недостаточно товара на складе для заказа №{order_id}.")


def simulate_race():
    """4. Моделирование гонки между транзакциями"""
    print("\n--- Моделирование Race Condition (WriteConflict) ---")
    db.inventory.update_one({"p_id": 999}, {"$set": {"stock": 1}}, upsert=True)

    def attempt_purchase(buyer):
        try:
            with client.start_session() as session:
                with session.start_transaction():
                    item = db.inventory.find_one({"p_id": 999}, session=session)
                    time.sleep(1)
                    if item and item['stock'] > 0:
                        db.inventory.update_one({"p_id": 999}, {"$inc": {"stock": -1}}, session=session)
                        print(f"[{buyer}] Успешно купил последний товар!")
                    else:
                        print(f"[{buyer}] Товар уже закончился.")
        except errors.PyMongoError as e:
            print(f"[{buyer}] Ошибка: {e}")

    threads = [
        threading.Thread(target=attempt_purchase, args=("User_A",)),
        threading.Thread(target=attempt_purchase, args=("User_B",))
    ]
    for t in threads: t.start()
    for t in threads: t.join()

def main():
    while True:
        print("\n=== ЛАБОРАТОРНАЯ РАБОТА №7: АГРЕГАЦИЯ И ТРАНЗАКЦИИ ===")
        print("1. Ежемесячная выручка по категориям")
        print("2. Корзиночный анализ (MBA)")
        print("3. RFM-анализ клиентов")
        print("4. Тест производительности (Materialized View)")
        print("5. Перемещение между складами (ACID)")
        print("6. Моделирование гонки (Изоляция)")
        print("0. Выход")

        choice = input("\nДействие: ")
        match choice:
            case "1": get_monthly_revenue()
            case "2": market_basket_analysis()
            case "3": rfm_analysis()
            case "4": performance_mv_test()
            case "5": safe_order_processing()
            case "6": simulate_race()
            case "0": break
            case _: print("Неверный ввод")

if __name__ == "__main__":
    main()