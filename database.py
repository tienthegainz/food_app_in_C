import sqlite3
from sqlite3 import Error


def create_connection(db_file):
    """ create a database connection to a SQLite database """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        print("Version: ", sqlite3.version)
    except Error as e:
        print(e)
    return conn


def create_table(conn, create_table_sql):
    """ create a table from the create_table_sql statement
    :param conn: Connection object
    :param create_table_sql: a CREATE TABLE statement
    :return:
    """
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
    except Error as e:
        print(e)


def insert_food(conn, food):
    sql = ''' INSERT INTO food(id, food)
              VALUES(?,?) '''
    cur = conn.cursor()
    cur.execute(sql, food)
    return


def insert_restaurant(conn, restaurant):
    sql = ''' INSERT INTO restaurant(name, domain, address)
              VALUES(?,?,?) '''
    cur = conn.cursor()
    cur.execute(sql, restaurant)
    return cur.lastrowid


if __name__ == '__main__':
    conn = create_connection("./data.db")
    restaurant_table = """ CREATE TABLE IF NOT EXISTS restaurant (
                            id integer PRIMARY KEY,
                            name text NOT NULL,
                            domain text NOT NULL,
                            address text NOT NULL
                        ); """

    food_table = """CREATE TABLE IF NOT EXISTS food (
                        id integer ,
                        food text NOT NULL,
                        FOREIGN KEY (id) REFERENCES projects (id)
                    );"""
    if conn is not None:
        # create projects table
        create_table(conn, restaurant_table)

        # create tasks table
        create_table(conn, food_table)
    else:
        print("Error! cannot create the database connection.")

    with conn:
        # insert restaurant
        restaurant = ('Tran Dan', '127.0.0.1:3500', 'Bolsa Street, California')
        restaurant_id = insert_restaurant(conn, restaurant)

        # insert food
        food1 = (restaurant_id, 'Tra tac')
        food2 = (restaurant_id, 'Buffet Com')
        food3 = (restaurant_id, 'Ga ran tam bot')
        food4 = (restaurant_id, 'Bun dau mam tom')
        food5 = (restaurant_id, 'Dimsum')
        food_list = [food1, food2, food3, food4, food5]
        for food in food_list:
            insert_food(conn, food)

         # insert restaurant
        restaurant = ('Kha BanK', '127.0.0.1:3501', 'Tu Son, Bac Ninh')
        restaurant_id = insert_restaurant(conn, restaurant)

        # insert food
        food1 = (restaurant_id, 'Com ga')
        food2 = (restaurant_id, 'Com trung')
        food3 = (restaurant_id, 'Com nha tu')
        food4 = (restaurant_id, 'Com rang')
        food5 = (restaurant_id, 'Com mam')
        food_list = [food1, food2, food3, food4, food5]
        for food in food_list:
            insert_food(conn, food)

         # insert restaurant
        restaurant = ('Living in 2077', '127.0.0.1:3502', 'Facebook')
        restaurant_id = insert_restaurant(conn, restaurant)

        # insert food
        food1 = (restaurant_id, 'Mi tom')
        food2 = (restaurant_id, 'Bun ca')
        food3 = (restaurant_id, 'Tra tac sieu to')
        food4 = (restaurant_id, 'Bun dau mam tom via he')
        food5 = (restaurant_id, 'Com rang dua bo')
        food_list = [food1, food2, food3, food4, food5]
        for food in food_list:
            insert_food(conn, food)

         # insert restaurant
        restaurant = ('Cai gi cung co, chi thieu moi em',
                      '127.0.0.1:3503', 'Trai tim anh')
        restaurant_id = insert_restaurant(conn, restaurant)

        # insert food
        food1 = (restaurant_id, 'Tra tac con ca 15k')
        food2 = (restaurant_id, 'Tra tac dac biet, nhieu mau')
        food3 = (restaurant_id, 'Ga tron bot chien')
        food4 = (restaurant_id, 'Banh da ca')
        food5 = (restaurant_id, 'Bun ca')
        food_list = [food1, food2, food3, food4, food5]
        for food in food_list:
            insert_food(conn, food)
