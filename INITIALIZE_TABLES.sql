# initializing tables.

# project 4 on 17 april 2017 by delroy fong , cleston sanders , jim bui.

DROP TABLE IF EXISTS EMPLOYEE_TABLE ;
DROP TABLE IF EXISTS LOCATION_TABLE ;
DROP TABLE IF EXISTS ITEM_TABLE ;
DROP TABLE IF EXISTS RANK_TABLE ;

CREATE TABLE EMPLOYEE_TABLE (
        employee_username VARCHAR (50) NOT NULL ,
        first_name VARCHAR (50) NOT NULL ,
        last_name VARCHAR (50) NOT NULL ,
        rank_id VARCHAR (50) NOT NULL ,
        location_id VARCHAR (50) NOT NULL ,
        race VARCHAR (50) NOT NULL ,
        PRIMARY KEY (employee_username)
) ;

CREATE TABLE LOCATION_TABLE (
        location_id VARCHAR (50) NOT NULL ,
        location_name VARCHAR (50) NOT NULL ,
        location_type VARCHAR (50) NOT NULL ,
        location_address VARCHAR (50) NOT NULL ,
        PRIMARY KEY (location_id)
) ;

CREATE TABLE ITEM_TABLE (
        item_id VARCHAR (50) NOT NULL ,
        location_id VARCHAR (50) NOT NULL ,
        item_name VARCHAR (50) NOT NULL ,
        item_type VARCHAR (50) NOT NULL ,
        item_sell_price INT NOT NULL ,
        item_buy_price INT NOT NULL ,
        item_stock_quantity INT NOT NULL , 
        item_description VARCHAR (100) NOT NULL ,
        PRIMARY KEY (item_id , location_id)
) ;

CREATE TABLE RANK_TABLE (
    rank_id VARCHAR (50) NOT NULL ,
    rank_name VARCHAR (50) NOT NULL ,
    rank_wage INT NOT NULL ,
    PRIMARY KEY (rank_id)
) ;