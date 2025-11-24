                       +--------------------+
                       |      Order         |
                       +--------------------+
                       | - id: string       |
                       | - userId: string   |
                       | - symbol: string   |
                       | - quantity: int    |
                       | - status: OrderStatus |
                       | - exchangeId: string |
                       +--------------------+
                       | + Order(...)       |
                       +--------------------+

                         Enum OrderStatus
                         { ACTIVE, PENDING, CONFIRMED, FAILED }

      «interface»                                     «implements»
+--------------------+                           +------------------------+
|     Database       |<-------------------------|   PostgresRepository   |
+--------------------+                           +------------------------+
| + saveOrder(o)     |                           | + saveOrder(o)         |
| + getOrder(id)     |                           | + getOrder(id)         |
+--------------------+                           +------------------------+
                           

                                  «implements»
                                       |
                                       v
                                 +--------------------+
                                 |  MongoRepository   |
                                 +--------------------+
                                 | + saveOrder(o)     |
                                 | + getOrder(id)     |
                                 +--------------------+


+---------------------+      aggregation      +--------------------+
|   PriceService      |---------------------->|     Exchange       |
+---------------------+                       +--------------------+
| - exchange:Exchange*|                       | - priceMap:map     |
+---------------------+                       +--------------------+
| + getPrice(symbol)  |                       | + getPrice(symbol) |
+---------------------+                       | + placeOrder(order)|
                                              +--------------------+


                   +-----------------------------+
                   |        OrderService         |
                   +-----------------------------+
                   | - db: Database*             |
                   | - exchange: Exchange*       |
                   +-----------------------------+
                   | + createAndSaveOrder(...)   |
                   | + getOrder(id)              |
                   +-----------------------------+

<img width="1004" height="714" alt="image" src="https://github.com/user-attachments/assets/a686d9bc-0346-4420-8a4f-87419d0cbc0d" />
