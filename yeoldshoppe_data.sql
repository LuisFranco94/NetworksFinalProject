DROP TABLE IF EXISTS CUSTOMER;
DROP TABLE IF EXISTS EMPLOYEE;
DROP TABLE IF EXISTS LOCATION;
DROP TABLE IF EXISTS ITEM_SELL;
DROP TABLE IF EXISTS ITEM_BUY;
DROP TABLE IF EXISTS RANK;


CREATE TABLE CUSTOMER (
       customer_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
       first_name VARCHAR (50),
       last_name VARCHAR (50),
       total_purchase DOUBLE,
#       member_since DATE DEFAULT '0000-00-00',
	   shipping_address VARCHAR(200),
       PRIMARY KEY (customer_id)
);

CREATE TABLE EMPLOYEE (
       employee_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
       first_name VARCHAR (50),
       last_name VARCHAR (50),
       rank_id SMALLINT UNSIGNED NOT NULL,
       location_id SMALLINT UNSIGNED NOT NULL,
       race VARCHAR(50),
       PRIMARY KEY (employee_id)
);

CREATE TABLE LOCATION (
       location_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
       owner_id SMALLINT UNSIGNED NOT NULL, 
       location_name VARCHAR (50),
       location_type VARCHAR (50),
       total_stock_worth DOUBLE PRECISION(10,2),
       location_address VARCHAR(50),
       PRIMARY KEY (location_id)
);

CREATE TABLE ITEM_SELL (
       item_sell_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
       location_id SMALLINT UNSIGNED NOT NULL,
       item_sell_name VARCHAR (50),
       item_sell_type VARCHAR (50),
       item_sell_price DOUBLE PRECISION(10,2),
       item_sell_lvl_req SMALLINT UNSIGNED, #max lvl 999?
       item_sell_quantity SMALLINT UNSIGNED, #can go up to 65535? or less lol
       PRIMARY KEY (item_sell_id)
);

CREATE TABLE ITEM_BUY (
       item_buy_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
       location_id SMALLINT UNSIGNED NOT NULL,
       item_buy_name VARCHAR (50),
       item_buy_type VARCHAR (50),
       item_buy_price DOUBLE PRECISION(10,2),
       item_buy_lvl_req SMALLINT UNSIGNED, #max lvl 999?
       item_buy_quantity SMALLINT UNSIGNED, #can go up to 65535? or less lol
       PRIMARY KEY (item_buy_id)
);

CREATE TABLE RANK (
	rank_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
	rank_name VARCHAR (50),
	#description TEXT, 		#have a block of text there really
	rank_wage DOUBLE PRECISION (10,2), #10 digits total, 2 AFTER decimal pt
	PRIMARY KEY (rank_id)
);

#FOR NOW MOST IF NOT ALL ID ARE AUTO-GENERATED STARTING FROM VALUE 1

##CUSTOMER TABLE
INSERT INTO CUSTOMER(first_name , last_name , total_purchase , shipping_address) 
       VALUES  	(	'Harrison' , 'Ford' 	, '9001.50' 	, '6969 Kessel Station'),
				(	'Garen'    , 'Tank' 	, '51.26'   	, '12 Demacia Drive'),
                (	'Genji'    , 'Bui'  	, '200'     	, '6969 Kessel Station'),
                (	'Cleston'  , 'Trace'	, '650'     	, 'Somewhere In Hollywood');

### WITH ID AS STRING                
#				('P00001' , 'Harrison' , 'Ford' , '9001.50' , '6969 Kessel Station'),
#				('P00002' , 'Garen'    , 'Tank' , '51.26'   , '12 Demacia Drive'),
#                ('P00003' , 'Genji'    , 'Bui'  , '200'     , '6969 Kessel Station'),
#                ('P00004' , 'Cleston'  , 'Trace', '650'     , 'Somewhere In Hollywood');

##EMPLOYEE TABLE
INSERT INTO EMPLOYEE(first_name , last_name , rank_id , location_id, race) 
       VALUES  	(	'Bummer'   , 'Mei'      , '1' 	  , '1'		, 'Wolf'),
				(	'Cloud'    , 'Sun' 	 	, '2'     , '2'		, 'Giant'),
                (	'Link'     , 'isZelda'  , '3'     , '2'		, 'Human'),
                (	'Bowser'   , 'Jr'   	, '2'     , '1'		, 'Cat');

##LOCATION TABLE
INSERT INTO LOCATION(owner_id , location_name , location_type , total_stock_worth, location_address) 
       VALUES  	(	'1'   		, 'Solaridus'  	, 'Store' 	   , '10000000'			, 'The Sun'		),
				(	'2'    		, 'Lunaria' 	, 'Warehouse'  , '200'				, 'The Moon'	),
                (	'3'     	, 'Oceanasia'   , 'Store'      , '50102401'			, 'The Ocean'	),
                (	'4'   		, 'Lavasand'   	, 'Warehouse'  , '12'				, 'The Volcano' );

##ITEM_SELL TABLE
INSERT INTO ITEM_SELL(location_id, item_sell_name , item_sell_type  , item_sell_price, item_sell_level_requirement , item_sell_quantity) 
       VALUES  	(		'1'   	, 'Rusted Spear'  , 'Spear' 	    , '2500'		 , '20'						   , '99'),
				(		'2'    	, 'Golden Bow' 	  , 'Bow'  			, '5000'		 , '25'						   , '25'),
                (		'3'     , 'GPU XXX'   	  , 'Miscellaneous' , '99999'		 , '99'						   , '5'),
                (		'4'   	, 'Doritos'   	  , 'Miscellaneous' , '36969'		 , '69'						   , '1');

##ITEM_BUY TABLE
INSERT INTO ITEM_BUY(location_id, item_buy_name   , item_buy_type   , item_buy_price , item_buy_level_requirement  , item_buy_quantity) 
       VALUES  	(		'1'   	, 'Diamond Sword' , 'Sword' 	    , '89000'		 , '90'						   , '10'),
				(		'2'    	, 'Vampire Whip'  , 'Whip'  		, '72486'		 , '82'						   , '15'),
                (		'3'     , 'Nail Hammer'   , 'Hammer' 		, '1000'		 , '15'						   , '17'),
                (		'4'   	, 'Happy Dagger'  , 'Dagger' 		, '135'		 	 , '5'						   , '12');

##RANK TABLE
INSERT INTO RANK(rank_name 			, rank_wage) 
       VALUES  	('Grunt'  			, '100'),
				('Shelf Stacker'	, '200'),
                ('Cashier'  		, '400'),
                ('Dragon Milker'  	, '10000'),
                ('Orc Kisser '  	, '20000');
