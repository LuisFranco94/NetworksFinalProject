# adding test data into database.

# EMPLOYEE TABLE.
INSERT INTO EMPLOYEE_TABLE (employee_username , first_name , last_name , rank_id , location_id , race , employee_password) 
VALUES
    # store 1.
    ('tclancy' , 'Tom' , 'Clancy' , 'R0003' , 'L0000' , 'Human' , 'password') ,
    ('swukong' , 'Sun' , 'Wukong' , 'R0001' , 'L0000' , 'Monkey-God' , 'password') ,
    ('srichards' , 'Sarah' , 'Richards' , 'R0002' , 'L0000' , 'Human' , 'password') ,
    ('bapache' , 'Boeing AH-64' , 'Apache' , 'R0002' , 'L0000' , 'Attack Helicopter' , 'password') ,
    ('kperry' , 'Katy' , 'Perry' , 'R0003' , 'L0002' , 'Pop Star' , 'password') ,
    ('klamar' , 'Kendrick' , 'Lamar' , 'R0000' , 'L0001' , 'Rap-God' , 'password') ,
    # store 2.
    ('jspringer' , 'Jerry' , 'Springer' , 'R0001' , 'L0001' , 'Human' , 'password') ,
    ('gforeman' , 'George' , 'Foreman' , 'R0002' , 'L0001' , 'Human' , 'password') ,
    ('jcaesar' , 'Julius' , 'Caesar' , 'R0002' , 'L0001' , 'Roman' , 'password') ,
    # store 3.
    ('hford' , 'Henry' , 'Ford' , 'R0001' , 'L0002' , 'Human' , 'password') ,
    ('etrump' , 'Eric' , 'Trump' , 'R0002' , 'L0002' , 'Human' , 'password') ,
    ('mkronovich' , 'Mile' , 'Kronovich' , 'R0002' , 'L0002' , 'Human' , 'password') ,
    # store 4.
    ('ajolie' , 'Angela' , 'Jolie' , 'R0001' , 'L0003' , 'Movie Star' , 'password') ,
    ('jdepp' , 'Johnny' , 'Depp' , 'R0002' , 'L0003' , 'Pirate' , 'password') ,
    ('wturner' , 'William' , 'Turner' , 'R0002' , 'L0003' , 'Pirate' , 'password') ,
    ('rbird' , 'Rakan' , 'Bird' , 'R0002' , 'L0003' , 'Bird-person' , 'password') ,
    # store 5.
    ('mchief' , 'Master' , 'Chief' , 'R0001' , 'L0004' , 'Super Human' , 'password') ,
    ('gorianna' , 'Garen' , 'Orianna' , 'R0002' , 'L0004' , 'Android' , 'password') ,
    ('rvolibear' , 'Rengar' , 'Volibear' , 'R0002' , 'L0004' , 'Dog-Bear' , 'password') ,
    ('ddomino' , 'Dice' , 'Domino' , 'R0002' , 'L0004' , 'Gangster' , 'password') ,
    ('epanda' , 'Express' , 'Panda' , 'R0003' , 'L0004' , 'Panda' , 'password') ,
    ('fcaptain' , 'Flint' , 'Captain' , 'R0003' , 'L0004' , 'Pirate' , 'password') ;

# LOCATION TABLE.
INSERT INTO LOCATION_TABLE (location_id , location_name , location_type , location_address) 
VALUES
    ('L0000' , 'Ye Olde Store' , 'Store' , '1234 On The Block Drive, Mega City 21999') ,
    ('L0001' , 'Ye Olde Mall' , 'Mall' , '4554 First Street, Mega City 21922') ,
    ('L0002' , 'Ye Olde Store Expansion Pack' , 'Store' , '42 Wallaby Way, Sydney') ,
    ('L0003' , 'Ye Olde Store Mini' , 'Store' , '23 Jedi Knight Temple, Galactic City, Coruscant') ,
    ('L0004' , 'Ye Olde Warehouse' , 'Warehouse' , '22 Jump Street , Hollywood') ;

# ITEM_BUY TABLE.
INSERT INTO ITEM_TABLE (item_id , location_id , item_name , item_type , item_sell_price , item_buy_price , item_stock_quantity , item_description) 
VALUES
    ('I0001' , 'L0000' , 'Really Big Sword' , 'Great Sword' , 2400 , 1900 , 3 , 'This is a really big sword.') ,
    ('I0002' , 'L0000' , 'Big Sword' , 'Sword' , 2200 , 1700 , 5 , 'This is a big sword.') ,
    ('I0003' , 'L0000' , 'Sword' , 'Sword' , 2000 , 1500 , 5 , 'This is a sword.') ,
    ('I0004' , 'L0000' , 'Small Sword' , 'Sword' , 1800 , 1300 , 5 , 'This is a small sword.') ,
    ('I0005' , 'L0000' , 'Really Small Sword' , 'Dagger' , 1600 , 1100 , 14 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0006' , 'L0000' , 'Armor' , 'Armor' , 2500 , 2000 , 5 , 'Normal armor.') ,
    ('I0007' , 'L0000' , 'Banana' , 'Food' , 10 , 5 , 52 , 'Monkey food.') ,
    ('I0008' , 'L0000' , 'Water' , 'Food' , 10 , 5 , 50 , 'Water.') ,
    ('I0009' , 'L0000' , 'Beer' , 'Food' , 15 , 5 , 50 , 'Better water.') ,

    ('I0010' , 'L0000' , 'Hammer of Thor' , 'Hammer' , 45000 , 30000 , 1 , 'The one and only.') ,
    ('I0011' , 'L0000' , 'Voltaic Spear' , 'Spear' , 9000 , 5000 , 1 , 'A spear with a point of obsidian.') ,

    # store 2.

    ('I0001' , 'L0001' , 'Really Big Sword' , 'Great Sword' , 2400 , 1900 , 3 , 'This is a really big sword.') ,
    ('I0002' , 'L0001' , 'Big Sword' , 'Sword' , 2200 , 1700 , 5 , 'This is a big sword.') ,
    ('I0003' , 'L0001' , 'Sword' , 'Sword' , 2000 , 1500 , 5 , 'This is a sword.') ,
    ('I0007' , 'L0001' , 'Banana' , 'Food' , 10 , 5 , 52 , 'Monkey food.') ,
    ('I0008' , 'L0001' , 'Water' , 'Food' , 10 , 5 , 50 , 'Water.') ,
    ('I0009' , 'L0001' , 'Beer' , 'Food' , 15 , 5 , 50 , 'Better water.') ,

    ('I0012' , 'L0001' , 'Edge of Infinity' , 'Great Sword' , 50000 , 25000 , 2 , 'A very lucky sword.') ,
    ('I0013' , 'L0001' , 'Flamberge' , 'Great Sword' , 45000 , 30000 , 1 , 'A blade with a edge of waves.') ,

    # store 3.

    ('I0005' , 'L0002' , 'Really Small Sword' , 'Dagger' , 1600 , 1100 , 14 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0006' , 'L0002' , 'Armor' , 'Armor' , 2500 , 2000 , 5 , 'Normal armor.') ,
    ('I0007' , 'L0002' , 'Banana' , 'Food' , 10 , 5 , 52 , 'Monkey food.') ,
    ('I0008' , 'L0002' , 'Water' , 'Food' , 10 , 5 , 50 , 'Water.') ,
    ('I0009' , 'L0002' , 'Beer' , 'Food' , 15 , 5 , 50 , 'Better water.') ,

    ('I0014' , 'L0002' , 'Great Helm' , 'Armor' , 1300 , 1000 , 6 , 'A huge helmet for protection.') ,
    ('I0015' , 'L0002' , 'Armor of Warmog' , 'Armor' , 7000 , 5000 , 2 , 'An enchanted set of armor worn by Warmog himself.') ,

    # store 4.

    ('I0001' , 'L0003' , 'Really Big Sword' , 'Great Sword' , 2400 , 1900 , 3 , 'This is a really big sword.') ,
    ('I0002' , 'L0003' , 'Big Sword' , 'Sword' , 2200 , 1700 , 5 , 'This is a big sword.') ,
    ('I0003' , 'L0003' , 'Sword' , 'Sword' , 2000 , 1500 , 5 , 'This is a sword.') ,
    ('I0004' , 'L0003' , 'Small Sword' , 'Sword' , 1800 , 1300 , 5 , 'This is a small sword.') ,
    ('I0005' , 'L0003' , 'Really Small Sword' , 'Dagger' , 1600 , 1100 , 14 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0009' , 'L0003' , 'Beer' , 'Food' , 15 , 5 , 50 , 'Better water.') ,

    ('I0016' , 'L0003' , 'Spear' , 'Spear' , 300 , 100 , 17 , 'Your generic spear.') ,

    # store 5.

    ('I0003' , 'L0004' , 'Sword' , 'Sword' , 2000 , 1500 , 5 , 'This is a sword.') ,
    ('I0004' , 'L0004' , 'Small Sword' , 'Sword' , 1800 , 1300 , 5 , 'This is a small sword.') ,
    ('I0005' , 'L0004' , 'Really Small Sword' , 'Dagger' , 1600 , 1100 , 14 , 'This is a sword so small that it is actually a dagger.') ,
    ('I0009' , 'L0004' , 'Beer' , 'Food' , 15 , 5 , 50 , 'Better water.') ,

    ('I0017' , 'L0004' , 'Sandwich' , 'Food' , 10 , 5 , 24 , 'Meat between bread.') ,
    ('I0018' , 'L0004' , 'Potatoes' , 'Food' , 2 , 1 , 32 , 'Uncooked mashed potatoes.') ,
    ('I0019' , 'L0004' , 'Onions' , 'Food' , 4 , 3 , 23 , 'Onions have layers.') ,
    ('I0020' , 'L0004' , 'Rice' , 'Food' , 3 , 2 , 55 , 'The sticky kind.') ,
    ('I0021' , 'L0004' , 'Sushi' , 'Food' , 15 , 10 , 41 , 'A delicacy from a far away land of Nippon.') ;

# RANK TABLE.
INSERT INTO RANK_TABLE (rank_id , rank_name , rank_wage) 
VALUES
    ('R0000' , 'CEO' , 999) ,
    ('R0001' , 'Manager' , 100) ,
    ('R0002' , 'Worker' , 75) ,
    ('R0003' , 'Intern' , 0) ;