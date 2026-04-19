from pymongo import MongoClient

client = MongoClient('mongodb://localhost:27017/')
db = client['shop_db']

collections = db.list_collection_names()

for collection in collections:
    db[collection].drop()
    print(f"Удалена коллекция: {collection}")

print(f"Всего удалено коллекций: {len(collections)}")