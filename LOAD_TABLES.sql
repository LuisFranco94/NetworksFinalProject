# adding test data into database.

# EMPLOYEE TABLE.
INSERT INTO EMPLOYEE_TABLE (employee_username , first_name , last_name , rank_id , location_id , race) 
VALUES
    ('tclancy' , 'Tom' , 'Clancy' , 'R0001' , 'L0000' , 'Human') ,
    ('swukong' , 'Sun' , 'Wukong' , 'R0002' , 'L0000' , 'Monkey') ,
    ('srichards' , 'Sarah' , 'Richards' , 'R0003' , 'L0000' , 'Human') ,
    ('bapache' , 'Boeing AH-64' , 'Apache' , 'R0004' , 'L0000' , 'Attack Helicopter') ,
    ('kperry' , 'Katy' , 'Perry' , 'R0003' , 'L0001' , 'Pop Star') ,
    ('klamar' , 'Kendrick' , 'Lamar' , 'R0000' , 'L0001' , 'God') ;

# LOCATION TABLE.
INSERT INTO LOCATION_TABLE (location_id , location_name , location_type , location_address) 
VALUES
    ('L0000' , 'Ye Olde Store' , 'Store' , '1234 On The Block Drive , Mega City 21999') ,
    ('L0001' , 'Ye Olde Warehouse' , 'Warehouse' , '1235 On the Block Drive , Mega City 21999') ;

# ITEM_BUY TABLE.
INSERT INTO ITEM_TABLE (item_id , location_id , item_name , item_type , item_sell_price , item_buy_price , item_stock_quantity , item_description) 
VALUES
    ('I0001' , 200000 , 'Big Fucking Sword' , 'Sword' , 2400 , 1900 , 3 , 'This is a really big sword.') ,
    ('I0002' , 200000 , 'Big Sword' , 'Sword' , 2200 , 1700 , 5 , 'This is a big sword.') ,
    ('I0003' , 200000 , 'Small Sword' , 'Dagger' , 1500 , 1000 , 14 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0004' , 200000 , 'Hammer of Thor' , 'Hammer ' , 45000 , 30000 , 1 , 'The one and only.') ,
    ('I0001' , 200001 , 'Big Fucking Sword' , 'Sword' , 2400 , 1900 , 99 , 'This is a really big sword.') ,
    ('I0002' , 200001 , 'Big Sword' , 'Sword' , 2200 , 1700 , 99 , 'This is a big sword.') ,
    ('I0003' , 200001 , 'Small Sword' , 'Dagger' , 1500 , 1000 , 99 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0005' , 200001 , 'Voltaic Spear' , 'Spear' , 21000 , 15000 , 1 , 'A really cool and really rare spear.') ;

# RANK TABLE.
INSERT INTO RANK_TABLE (rank_id , rank_name , rank_wage) 
VALUES
    ('R0000' , 'Super Grand Imperial Dragon of the Moon' , 999) ,
    ('R0001' , 'Grand Imperial Dragon' , 100) ,
    ('R0002' , 'Almost Grand Imperial Dragon' , 75) ,
    ('R0003' , 'Not Very Grand Imperial Dragon' , 50) ,
    ('R0004' , 'Not Even An Imperial Dragon' , 25) ;